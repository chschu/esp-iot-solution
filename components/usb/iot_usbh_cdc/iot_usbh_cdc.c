/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <sys/queue.h>
#include "esp_err.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"
#include "freertos/event_groups.h"
#include "iot_usbh_cdc.h"

#include "usb/usb_host.h"
#include "iot_usbh_descriptor.h"

static const char *TAG = "USBH_CDC";

// CDC spinlock
static portMUX_TYPE cdc_lock =  portMUX_INITIALIZER_UNLOCKED;
#define CDC_ENTER_CRITICAL()    portENTER_CRITICAL(&cdc_lock)
#define CDC_EXIT_CRITICAL()     portEXIT_CRITICAL(&cdc_lock)

// CDC-ACM events
#define CDC_TEARDOWN            BIT0
#define CDC_TEARDOWN_COMPLETE   BIT1

#define TIMEOUT_USB_RINGBUF_MS  200                      /*! Timeout for ring buffer operate */

typedef struct {
    usb_host_client_handle_t cdc_client_hdl;             /*!< USB Host handle reused for all CDC-ACM devices in the system */
    EventGroupHandle_t event_group;
    SemaphoreHandle_t mutex;
    usbh_cdc_new_dev_cb_t new_dev_cb;
    SLIST_HEAD(list_dev, usbh_cdc_s) cdc_devices_list;   /*!< List of open pseudo devices */
} usbh_cdc_obj_t;

typedef struct usbh_cdc_s {
    usb_device_handle_t dev_hdl;           // USB device handle
    usbh_cdc_state_t state;                // State of the cdc device
    uint8_t dev_addr;                      // Device address
    uint8_t intf_idx;                      // Interface index
    uint16_t vid;                          // Vendor ID
    uint16_t pid;                          // Product ID
    struct {
        usb_transfer_t *out_xfer;          // OUT data transfer
        SemaphoreHandle_t out_xfer_free_sem;
        usb_transfer_t *in_xfer;           // IN data transfer
        uint16_t in_mps;                   // IN endpoint Maximum Packet Size
        uint8_t *in_data_buffer_base;      // Pointer to IN data buffer in usb_transfer_t
        usb_intf_desc_t *intf_desc;  // Pointer to data interface descriptor
    } data;
    usbh_cdc_event_callbacks_t cbs;         // Callbacks for the pseudo device
    RingbufHandle_t in_ringbuf_handle;   /*!< in ringbuffer handle of corresponding interface */
    size_t in_ringbuf_size;
    RingbufHandle_t out_ringbuf_handle;  /*!< if interface is ready */
    size_t out_ringbuf_size;
    cdc_parsed_info_t info;                // Parsed interface descriptor
    SLIST_ENTRY(usbh_cdc_s) list_entry;
} usbh_cdc_t;

static usbh_cdc_obj_t *p_usbh_cdc_obj = NULL;

static esp_err_t _cdc_open(usbh_cdc_t *cdc);

static esp_err_t _cdc_close(usbh_cdc_t *cdc);

static void _cdc_transfers_free(usbh_cdc_t *cdc);

static void _cdc_tx_xfer_submit(usb_transfer_t *out_xfer);

/*--------------------------------- CDC Buffer Handle Code --------------------------------------*/
static size_t _get_ringbuf_len(RingbufHandle_t ringbuf_hdl)
{
    size_t uxItemsWaiting = 0;
    vRingbufferGetInfo(ringbuf_hdl, NULL, NULL, NULL, NULL, &uxItemsWaiting);
    return uxItemsWaiting;
}

static esp_err_t _ringbuf_pop(RingbufHandle_t ringbuf_hdl, uint8_t *buf, size_t req_bytes, size_t *read_bytes, TickType_t ticks_to_wait)
{
    uint8_t *buf_rcv = xRingbufferReceiveUpTo(ringbuf_hdl, read_bytes, ticks_to_wait, req_bytes);
    if (!buf_rcv) {
        return ESP_FAIL;
    }

    memcpy(buf, buf_rcv, *read_bytes);
    vRingbufferReturnItem(ringbuf_hdl, (void *)(buf_rcv));

    size_t read_bytes2 = 0;
    if (*read_bytes < req_bytes) {
        buf_rcv = xRingbufferReceiveUpTo(ringbuf_hdl, &read_bytes2, 0, req_bytes - *read_bytes);
        if (buf_rcv) {
            memcpy(buf + *read_bytes, buf_rcv, read_bytes2);
            *read_bytes += read_bytes2;
            vRingbufferReturnItem(ringbuf_hdl, (void *)(buf_rcv));
        }
    }

    return ESP_OK;
}

static esp_err_t _ringbuf_push(RingbufHandle_t ringbuf_hdl, const uint8_t *buf, size_t write_bytes, TickType_t ticks_to_wait)
{
    int res = xRingbufferSend(ringbuf_hdl, buf, write_bytes, ticks_to_wait);

    if (res != pdTRUE) {
        ESP_LOGW(TAG, "The in buffer is too small, the data has been lost");
        return ESP_FAIL;
    }
    return ESP_OK;
}

static void _ring_buffer_flush(RingbufHandle_t ringbuf_hdl)
{
    assert(ringbuf_hdl);
    size_t read_bytes = 0;
    size_t uxItemsWaiting = 0;
    vRingbufferGetInfo(ringbuf_hdl, NULL, NULL, NULL, NULL, &uxItemsWaiting);
    uint8_t *buf_rcv = xRingbufferReceiveUpTo(ringbuf_hdl, &read_bytes, 0, uxItemsWaiting);

    if (buf_rcv) {
        vRingbufferReturnItem(ringbuf_hdl, (void *)(buf_rcv));
    }

    if (uxItemsWaiting > read_bytes) {
        // read the second time to flush all data
        vRingbufferGetInfo(ringbuf_hdl, NULL, NULL, NULL, NULL, &uxItemsWaiting);
        buf_rcv = xRingbufferReceiveUpTo(ringbuf_hdl, &read_bytes, 0, uxItemsWaiting);
        if (buf_rcv) {
            vRingbufferReturnItem(ringbuf_hdl, (void *)(buf_rcv));
        }
    }
}

static void usb_lib_task(void *arg)
{
    // Install USB Host driver. Should only be called once in entire application
    const usb_host_config_t host_config = {
        .skip_phy_setup = false,
        .intr_flags = ESP_INTR_FLAG_LEVEL1,
    };
    ESP_ERROR_CHECK(usb_host_install(&host_config));

    //Signalize the usbh_cdc_driver_install, the USB host library has been installed
    xTaskNotifyGive(arg);

    bool has_clients = true;
    bool has_devices = false;
    while (has_clients) {
        uint32_t event_flags;
        ESP_ERROR_CHECK(usb_host_lib_handle_events(portMAX_DELAY, &event_flags));
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
            ESP_LOGI(TAG, "Get FLAGS_NO_CLIENTS");
            if (ESP_OK == usb_host_device_free_all()) {
                ESP_LOGI(TAG, "All devices marked as free, no need to wait FLAGS_ALL_FREE event");
                has_clients = false;
            } else {
                ESP_LOGI(TAG, "Wait for the FLAGS_ALL_FREE");
                has_devices = true;
            }
        }
        if (has_devices && event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
            ESP_LOGI(TAG, "Get FLAGS_ALL_FREE");
            has_clients = false;
        }
    }
    ESP_LOGI(TAG, "No more clients and devices, uninstall USB Host library");

    // Clean up USB Host
    vTaskDelay(100); // Short delay to allow clients clean-up
    usb_host_uninstall();
    ESP_LOGD(TAG, "USB Host library is uninstalled");
    vTaskDelete(NULL);
}

/**
 * @brief CDC-ACM driver handling task
 *
 * USB host client registration and deregistration is handled here.
 *
 * @param[in] arg User's argument. Handle of a task that started this task.
 */
static void usbh_cdc_client_task(void *arg)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // Task will be resumed from cdc_acm_host_install()
    usbh_cdc_obj_t *usbh_cdc_obj = p_usbh_cdc_obj; // Make local copy of the driver's handle
    assert(usbh_cdc_obj->cdc_client_hdl);
    // Start handling client's events
    while (1) {
        usb_host_client_handle_events(usbh_cdc_obj->cdc_client_hdl, portMAX_DELAY);
        EventBits_t events = xEventGroupGetBits(usbh_cdc_obj->event_group);
        if (events & CDC_TEARDOWN) {
            break;
        }
    }

    ESP_LOGD(TAG, "Deregistering client");
    ESP_ERROR_CHECK(usb_host_client_deregister(usbh_cdc_obj->cdc_client_hdl));
    xEventGroupSetBits(usbh_cdc_obj->event_group, CDC_TEARDOWN_COMPLETE);
    vTaskDelete(NULL);
}

static void usb_event_cb(const usb_host_client_event_msg_t *event_msg, void *arg)
{
    switch (event_msg->event) {
    case USB_HOST_CLIENT_EVENT_NEW_DEV: {
        usb_device_handle_t current_device = NULL;
        if (usb_host_device_open(p_usbh_cdc_obj->cdc_client_hdl, event_msg->new_dev.address, &current_device) != ESP_OK) {
            ESP_LOGE(TAG, "Could not open device");
            return;
        }
        assert(current_device);

        /*!< Call new dev callback */
        if (p_usbh_cdc_obj->new_dev_cb) {
            p_usbh_cdc_obj->new_dev_cb(current_device);
        }

        const usb_device_desc_t *device_desc;
        ESP_ERROR_CHECK(usb_host_get_device_descriptor(current_device, &device_desc));

        usbh_cdc_t *cdc;
        bool if_opened = false;
        SLIST_FOREACH(cdc, &p_usbh_cdc_obj->cdc_devices_list, list_entry) {
            if ((cdc->vid != CDC_HOST_ANY_VID && cdc->vid != device_desc->idVendor) ||
                    (cdc->pid != CDC_HOST_ANY_PID && cdc->pid != device_desc->idProduct)) {
                continue;
            }

            cdc->vid = device_desc->idVendor;
            cdc->pid = device_desc->idProduct;
            cdc->dev_addr = event_msg->new_dev.address;
            cdc->dev_hdl = current_device;

            if (_cdc_open(cdc) != ESP_OK) {
                ESP_LOGE(TAG, "Failed to open cdc device: %d", event_msg->new_dev.address);
            } else {
                if_opened = true;
                ESP_LOGI(TAG, "Opened cdc device: %d", event_msg->new_dev.address);
            }
        }

        if (!if_opened) {
            usb_host_device_close(p_usbh_cdc_obj->cdc_client_hdl, current_device); // Gracefully continue on error
        }
        ESP_LOGI(TAG, "New device connected, address: %d", event_msg->new_dev.address);
        break;
    }
    case USB_HOST_CLIENT_EVENT_DEV_GONE: {
        ESP_LOGD(TAG, "Device suddenly disconnected");
        // Find CDC pseudo-devices associated with this USB device and close them

        usbh_cdc_t *cdc;
        SLIST_FOREACH(cdc, &p_usbh_cdc_obj->cdc_devices_list, list_entry) {
            if (cdc->dev_hdl != event_msg->dev_gone.dev_hdl) {
                continue;
            }

            _cdc_close(cdc);
            /*!< TODO remove all interface */
            // break;
        }
        break;
    }
    default:
        assert(false);
        break;
    }
}

esp_err_t usbh_cdc_driver_install(const usbh_cdc_driver_config_t *config)
{
    esp_err_t ret = ESP_OK;
    ESP_RETURN_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, TAG, "config is NULL");
    ESP_RETURN_ON_FALSE(p_usbh_cdc_obj == NULL, ESP_ERR_INVALID_STATE, TAG, "usbh_cdc already installed");

    ESP_LOGI(TAG, "iot usbh cdc version: %d.%d.%d", IOT_USBH_CDC_VER_MAJOR, IOT_USBH_CDC_VER_MINOR, IOT_USBH_CDC_VER_PATCH);

    if (!config->skip_init_usb_host_driver) {
        BaseType_t core_id = (CONFIG_USBH_TASK_CORE_ID < 0) ? tskNO_AFFINITY : CONFIG_USBH_TASK_CORE_ID;
        BaseType_t task_created = xTaskCreatePinnedToCore(usb_lib_task, "usb_lib", 4096, xTaskGetCurrentTaskHandle(), CONFIG_USBH_TASK_BASE_PRIORITY, NULL, core_id);
        ESP_RETURN_ON_FALSE(task_created == pdPASS, ESP_FAIL, TAG, "xTaskCreatePinnedToCore failed");
        // Wait unit the USB host library is installed
        uint32_t notify_value = ulTaskNotifyTake(false, pdMS_TO_TICKS(1000));
        if (notify_value == 0) {
            ESP_LOGE(TAG, "USB host library not installed");
            return ESP_FAIL;
        }
    }

    p_usbh_cdc_obj = (usbh_cdc_obj_t *) calloc(1, sizeof(usbh_cdc_obj_t));
    ESP_RETURN_ON_FALSE(p_usbh_cdc_obj != NULL, ESP_ERR_NO_MEM, TAG, "calloc failed");
    EventGroupHandle_t event_group = xEventGroupCreate();
    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    TaskHandle_t driver_task_h = NULL;

    BaseType_t core_id = (config->task_coreid < 0) ? tskNO_AFFINITY : config->task_coreid;
    xTaskCreatePinnedToCore(usbh_cdc_client_task, "usbh_cdc", config->task_stack_size, NULL, config->task_priority, &driver_task_h, core_id);
    if (p_usbh_cdc_obj == NULL || driver_task_h == NULL || event_group == NULL || mutex == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto err;
    }

    // Register USB Host client
    usb_host_client_handle_t usb_client = NULL;
    const usb_host_client_config_t client_config = {
        .is_synchronous = false,
        .max_num_event_msg = 3,
        .async.client_event_callback = usb_event_cb,
        .async.callback_arg = NULL
    };
    ESP_GOTO_ON_ERROR(usb_host_client_register(&client_config, &usb_client), err, TAG, "Failed to register USB host client");

    // Initialize CDC driver structure
    SLIST_INIT(&(p_usbh_cdc_obj->cdc_devices_list));

    p_usbh_cdc_obj->cdc_client_hdl = usb_client;
    p_usbh_cdc_obj->event_group = event_group;
    p_usbh_cdc_obj->mutex = mutex;
    p_usbh_cdc_obj->new_dev_cb = config->new_dev_cb;

    xTaskNotifyGive(driver_task_h);
    return ESP_OK;

err: // Clean-up
    if (p_usbh_cdc_obj) {
        free(p_usbh_cdc_obj);
    }
    if (event_group) {
        vEventGroupDelete(event_group);
    }
    if (driver_task_h) {
        vTaskDelete(driver_task_h);
    }
    if (mutex) {
        vSemaphoreDelete(mutex);
    }
    return ret;
}

esp_err_t usbh_cdc_driver_uninstall(void)
{
    esp_err_t ret = ESP_OK;
    ESP_RETURN_ON_FALSE(p_usbh_cdc_obj, ESP_ERR_INVALID_STATE, TAG, "usbh cdc not installed");

    CDC_ENTER_CRITICAL();
    if (SLIST_EMPTY(&p_usbh_cdc_obj->cdc_devices_list)) {
        // Check that device list is empty (all devices closed)
    } else {
        ret = ESP_ERR_INVALID_STATE;
        CDC_EXIT_CRITICAL();
        goto unblock;
    }
    CDC_EXIT_CRITICAL();

    // Signal to CDC task to stop, unblock it and wait for its deletion
    xEventGroupSetBits(p_usbh_cdc_obj->event_group, CDC_TEARDOWN);
    usb_host_client_unblock(p_usbh_cdc_obj->cdc_client_hdl);
    ESP_GOTO_ON_FALSE(
        xEventGroupWaitBits(p_usbh_cdc_obj->event_group, CDC_TEARDOWN_COMPLETE, pdFALSE, pdFALSE, pdMS_TO_TICKS(100)),
        ESP_ERR_NOT_FINISHED, unblock, TAG,);
    vEventGroupDelete(p_usbh_cdc_obj->event_group);
    vSemaphoreDelete(p_usbh_cdc_obj->mutex);

    free(p_usbh_cdc_obj);
    p_usbh_cdc_obj = NULL;

unblock:
    return ret;
}

static void in_xfer_cb(usb_transfer_t *in_xfer)
{
    assert(in_xfer);

    usbh_cdc_t *cdc = (usbh_cdc_t *)in_xfer->context;
    if (cdc->state != USBH_CDC_OPEN) {
        in_xfer->status = USB_TRANSFER_STATUS_CANCELED;
    }

    switch (in_xfer->status) {
    case USB_TRANSFER_STATUS_COMPLETED: {
        size_t data_len = _get_ringbuf_len(cdc->in_ringbuf_handle);
        if (data_len + in_xfer->actual_num_bytes >= cdc->in_ringbuf_size) {
            // TODO: add notify cb for user
            // if ringbuffer overflow, drop the data
            ESP_LOGD(TAG, "in ringbuf full");
        } else {
            if (_ringbuf_push(cdc->in_ringbuf_handle, in_xfer->data_buffer, in_xfer->actual_num_bytes, pdMS_TO_TICKS(TIMEOUT_USB_RINGBUF_MS)) != ESP_OK) {
                ESP_LOGE(TAG, "in ringbuf push failed");
            }
        }

        usb_host_transfer_submit(in_xfer);

        if (cdc->cbs.revc_data) {
            cdc->cbs.revc_data(cdc->cbs.user_data, cdc->cbs.user_data);
        }

        return;
    }
    case USB_TRANSFER_STATUS_NO_DEVICE:
    case USB_TRANSFER_STATUS_CANCELED:
        return;
    default:
        // Any other error
        break;
    }

    ESP_LOGE(TAG, "RX xfer failed, status %d", in_xfer->status);
}

static void out_xfer_cb(usb_transfer_t *out_xfer)
{
    assert(out_xfer);
    usbh_cdc_t *cdc = (usbh_cdc_t *)out_xfer->context;

    if (cdc->state != USBH_CDC_OPEN) {
        out_xfer->status = USB_TRANSFER_STATUS_CANCELED;
    }

    switch (out_xfer->status) {
    case USB_TRANSFER_STATUS_COMPLETED: {
        _cdc_tx_xfer_submit(out_xfer);
        return;
    }
    case USB_TRANSFER_STATUS_NO_DEVICE:
    case USB_TRANSFER_STATUS_CANCELED:
        // User is notified about device disconnection from usb_event_cb
        // No need to do anything
        return;
    default:
        // Any other error, add the transfer to free list
        break;
    }
    ESP_LOGE(TAG, "TX Transfer failed, status %d", out_xfer->status);
}

static void _cdc_tx_xfer_submit(usb_transfer_t *out_xfer)
{
    usbh_cdc_t *cdc = (usbh_cdc_t *) out_xfer->context;
    assert(cdc);

    size_t data_len = _get_ringbuf_len(cdc->out_ringbuf_handle);
    if (data_len > 0) {
        if (data_len > out_xfer->data_buffer_size) {
            data_len = out_xfer->data_buffer_size;
        }
        size_t actual_num_bytes = 0;
        _ringbuf_pop(cdc->out_ringbuf_handle, out_xfer->data_buffer, data_len, &actual_num_bytes, 0);
        assert(actual_num_bytes == data_len);
        out_xfer->num_bytes = actual_num_bytes;
        usb_host_transfer_submit(out_xfer);
    } else {
        xSemaphoreGive(cdc->data.out_xfer_free_sem);
    }
}

static esp_err_t _cdc_transfers_allocate(usbh_cdc_t *cdc, const usb_ep_desc_t *in_ep_desc, const usb_ep_desc_t *out_ep_desc)
{
    esp_err_t ret = ESP_OK;

    const size_t in_buf_len = CONFIG_IN_TRANSFER_BUFFER_SIZE;
    if (in_ep_desc) {
        ESP_LOGD(TAG, "in ep mps: %d", USB_EP_DESC_GET_MPS(in_ep_desc));
        if (in_buf_len > 0) {
            ESP_GOTO_ON_ERROR(
                usb_host_transfer_alloc(in_buf_len, 0, &cdc->data.in_xfer),
                err, TAG,
            );
            assert(cdc->data.in_xfer);
            cdc->data.in_xfer->callback = in_xfer_cb;
            cdc->data.in_xfer->num_bytes = in_buf_len;
            cdc->data.in_xfer->bEndpointAddress = in_ep_desc->bEndpointAddress;
            cdc->data.in_xfer->device_handle = cdc->dev_hdl;
            cdc->data.in_xfer->context = cdc;
            cdc->data.in_mps = USB_EP_DESC_GET_MPS(in_ep_desc);
            cdc->data.in_data_buffer_base = cdc->data.in_xfer->data_buffer;
        }
    }

    if (out_ep_desc) {
        const size_t out_buf_len = CONFIG_OUT_TRANSFER_BUFFER_SIZE;
        if (out_buf_len > 0) {
            ESP_GOTO_ON_ERROR(
                usb_host_transfer_alloc(out_buf_len, 0, &cdc->data.out_xfer),
                err, TAG,
            );
            assert(cdc->data.out_xfer);
            cdc->data.out_xfer->callback = out_xfer_cb;
            cdc->data.out_xfer->bEndpointAddress = out_ep_desc->bEndpointAddress;
            cdc->data.out_xfer->device_handle = cdc->dev_hdl;
            cdc->data.out_xfer->context = cdc;
            cdc->data.out_xfer->num_bytes = out_buf_len;
        }
    }

    return ret;
err:
    _cdc_transfers_free(cdc);
    return ret;
}

static void _cdc_transfers_free(usbh_cdc_t *cdc)
{
    assert(cdc);
    if (cdc->data.in_xfer) {
        usb_host_transfer_free(cdc->data.in_xfer);
    }
    if (cdc->data.out_xfer) {
        usb_host_transfer_free(cdc->data.out_xfer);
    }
}

static esp_err_t _cdc_reset_transfer_endpoint(usb_device_handle_t dev_hdl, usb_transfer_t *transfer)
{
    assert(dev_hdl);
    assert(transfer);

    ESP_RETURN_ON_ERROR(usb_host_endpoint_halt(dev_hdl, transfer->bEndpointAddress), TAG,);
    ESP_RETURN_ON_ERROR(usb_host_endpoint_flush(dev_hdl, transfer->bEndpointAddress), TAG,);
    usb_host_endpoint_clear(dev_hdl, transfer->bEndpointAddress);
    return ESP_OK;
}

static esp_err_t _cdc_start(usbh_cdc_t *cdc)
{
    esp_err_t ret = ESP_OK;
    assert(cdc);
    ESP_GOTO_ON_ERROR(
        usb_host_interface_claim(
            p_usbh_cdc_obj->cdc_client_hdl,
            cdc->dev_hdl,
            cdc->data.intf_desc->bInterfaceNumber,
            cdc->data.intf_desc->bAlternateSetting),
        err, TAG, "Could not claim interface");
    if (cdc->data.in_xfer) {
        ESP_LOGD(TAG, "Submitting poll for BULK IN transfer");
        ESP_ERROR_CHECK(usb_host_transfer_submit(cdc->data.in_xfer));
    }

    return ESP_OK;

err:
    return ret;
}

static esp_err_t _cdc_find_and_open_usb_device(usbh_cdc_t *cdc)
{
    assert(cdc);

    // First, check list of already opened CDC devices
    ESP_LOGD(TAG, "Checking list of opened USB devices");

    usbh_cdc_t *dev;
    SLIST_FOREACH(dev, &p_usbh_cdc_obj->cdc_devices_list, list_entry) {
        const usb_device_desc_t *device_desc;
        if (dev->state == USBH_CDC_CLOSE) {
            continue;
        }
        ESP_ERROR_CHECK(usb_host_get_device_descriptor(dev->dev_hdl, &device_desc));
        if ((cdc->vid == device_desc->idVendor || cdc->vid == CDC_HOST_ANY_VID) &&
                (cdc->pid == device_desc->idProduct || cdc->pid == CDC_HOST_ANY_PID)) {
            // Return path 1:
            ESP_LOGD(TAG, "Found already opened device %d:%d", device_desc->idVendor, device_desc->idProduct);
            cdc->dev_hdl = dev->dev_hdl;
            return ESP_OK;
        }
    }

    uint8_t dev_addr_list[CONFIG_DEVICE_ADDRESS_LIST_NUM];
    int num_of_devices;
    ESP_ERROR_CHECK(usb_host_device_addr_list_fill(sizeof(dev_addr_list), dev_addr_list, &num_of_devices));

    for (int i = 0; i < num_of_devices; i++) {
        usb_device_handle_t current_device;
        if (usb_host_device_open(p_usbh_cdc_obj->cdc_client_hdl, dev_addr_list[i], &current_device) != ESP_OK) {
            ESP_LOGE(TAG, "Could not open device %d", dev_addr_list[i]);
            continue;
        }
        assert(current_device);
        const usb_device_desc_t *device_desc;
        ESP_ERROR_CHECK(usb_host_get_device_descriptor(current_device, &device_desc));

        if ((cdc->vid == device_desc->idVendor || cdc->vid == CDC_HOST_ANY_VID) &&
                (cdc->pid == device_desc->idProduct || cdc->pid == CDC_HOST_ANY_PID)) {
            // Return path 2:
            ESP_LOGD(TAG, "Found none opened device %d:%d", device_desc->idVendor, device_desc->idProduct);
            cdc->dev_hdl = current_device;
            return ESP_OK;
        }

        usb_host_device_close(p_usbh_cdc_obj->cdc_client_hdl, current_device); // Gracefully continue on error
    }
    return ESP_ERR_NOT_FOUND;
}

static esp_err_t _cdc_open(usbh_cdc_t *cdc)
{
    esp_err_t ret = ESP_OK;
    assert(cdc);
    ESP_RETURN_ON_FALSE(cdc->state == USBH_CDC_CLOSE, ESP_OK, TAG,);

    if (cdc->cbs.connect) {
        cdc->cbs.connect((usbh_cdc_handle_t)cdc, cdc->cbs.user_data);
    }

    // Get Device and Configuration descriptors
    const usb_config_desc_t *config_desc;
    const usb_device_desc_t *device_desc;
    ESP_ERROR_CHECK(usb_host_get_active_config_descriptor(cdc->dev_hdl, &config_desc));
    ESP_ERROR_CHECK(usb_host_get_device_descriptor(cdc->dev_hdl, &device_desc));

    cdc_parsed_info_t cdc_info;
    ret = cdc_parse_interface_descriptor(device_desc, config_desc, cdc->intf_idx, &cdc->data.intf_desc, &cdc_info);
    if (ret != ESP_OK) {
        goto err;
    }

    _ring_buffer_flush(cdc->in_ringbuf_handle);
    _ring_buffer_flush(cdc->out_ringbuf_handle);

    ESP_GOTO_ON_ERROR(
        _cdc_transfers_allocate(cdc, cdc_info.in_ep, cdc_info.out_ep),
        err, TAG,);

    ESP_GOTO_ON_ERROR(_cdc_start(cdc), err, TAG,);
    cdc->state = USBH_CDC_OPEN;

    cdc->vid = device_desc->idVendor;
    cdc->pid = device_desc->idProduct;

    return ESP_OK;

err:
    return ret;
}

static esp_err_t _cdc_close(usbh_cdc_t *cdc)
{
    esp_err_t ret = ESP_OK;
    assert(cdc);
    if (cdc->state != USBH_CDC_OPEN) {
        return ESP_ERR_INVALID_STATE;
    }

    // Cancel polling of BULK IN
    if (cdc->data.in_xfer) {
        ESP_ERROR_CHECK(_cdc_reset_transfer_endpoint(cdc->dev_hdl, cdc->data.in_xfer));
    }

    // Release all interfaces
    ESP_ERROR_CHECK(usb_host_interface_release(p_usbh_cdc_obj->cdc_client_hdl, cdc->dev_hdl, cdc->data.intf_desc->bInterfaceNumber));

    _cdc_transfers_free(cdc);
    usb_host_device_close(p_usbh_cdc_obj->cdc_client_hdl, cdc->dev_hdl);
    cdc->state = USBH_CDC_CLOSE;

    if (cdc->cbs.disconnect) {
        cdc->cbs.disconnect((usbh_cdc_handle_t)cdc, cdc->cbs.user_data);
    }

    return ret;
}

esp_err_t usbh_cdc_create(const usbh_cdc_device_config_t *config, usbh_cdc_handle_t *cdc_handle)
{
    esp_err_t ret = ESP_OK;
    ESP_RETURN_ON_FALSE(p_usbh_cdc_obj != NULL, ESP_ERR_INVALID_STATE, TAG, "usbh_cdc is not installed");
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    ESP_RETURN_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, TAG, "config is NULL");

    usbh_cdc_t *cdc = (usbh_cdc_t *) calloc(1, sizeof(usbh_cdc_t));
    ESP_RETURN_ON_FALSE(cdc != NULL, ESP_ERR_NO_MEM, TAG, "calloc failed");

    cdc->vid = config->vid;
    cdc->pid = config->pid;
    cdc->intf_idx = config->itf_num;

    cdc->in_ringbuf_size = config->rx_buffer_size ? config->rx_buffer_size : CONFIG_IN_RINGBUFFER_SIZE;
    cdc->in_ringbuf_handle = xRingbufferCreate(cdc->in_ringbuf_size, RINGBUF_TYPE_BYTEBUF);
    ESP_GOTO_ON_FALSE(cdc->in_ringbuf_handle != NULL, ESP_ERR_NO_MEM, fail, TAG, "Failed to create ring buffer");

    cdc->out_ringbuf_size = config->tx_buffer_size ? config->tx_buffer_size : CONFIG_OUT_RINGBUFFER_SIZE;
    cdc->out_ringbuf_handle = xRingbufferCreate(cdc->out_ringbuf_size, RINGBUF_TYPE_BYTEBUF);
    ESP_GOTO_ON_FALSE(cdc->out_ringbuf_handle != NULL, ESP_ERR_NO_MEM, fail, TAG, "Failed to create ring buffer");

    cdc->cbs = config->cbs;

    ret = _cdc_find_and_open_usb_device(cdc);
    if (ret == ESP_OK) {
        ESP_GOTO_ON_FALSE(_cdc_open(cdc) == ESP_OK, ESP_FAIL, fail, TAG, "Failed to open cdc device: %d", cdc->dev_addr);
    }

    cdc->data.out_xfer_free_sem = xSemaphoreCreateBinary();
    ESP_GOTO_ON_FALSE(cdc->data.out_xfer_free_sem != NULL, ESP_ERR_NO_MEM, fail, TAG, "Failed to create mutex");
    xSemaphoreGive(cdc->data.out_xfer_free_sem);

    CDC_ENTER_CRITICAL();
    SLIST_INSERT_HEAD(&p_usbh_cdc_obj->cdc_devices_list, cdc, list_entry);
    CDC_EXIT_CRITICAL();

    *cdc_handle = (usbh_cdc_handle_t)cdc;
    return ret;
fail:
    if (cdc->in_ringbuf_handle) {
        vRingbufferDelete(cdc->in_ringbuf_handle);
    }
    if (cdc->out_ringbuf_handle) {
        vRingbufferDelete(cdc->out_ringbuf_handle);
    }
    if (cdc->data.out_xfer_free_sem) {
        vSemaphoreDelete(cdc->data.out_xfer_free_sem);
    }
    if (cdc->dev_hdl) {
        usb_host_device_close(p_usbh_cdc_obj->cdc_client_hdl, cdc->dev_hdl);
    }
    if (cdc) {
        free(cdc);
    }
    return ret;
}

esp_err_t usbh_cdc_delete(usbh_cdc_handle_t cdc_handle)
{
    ESP_RETURN_ON_FALSE(p_usbh_cdc_obj != NULL, ESP_ERR_INVALID_STATE, TAG, "p_usbh_cdc_obj is NULL");
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;

    _cdc_close(cdc);

    CDC_ENTER_CRITICAL();
    SLIST_REMOVE(&p_usbh_cdc_obj->cdc_devices_list, cdc, usbh_cdc_s, list_entry);
    CDC_EXIT_CRITICAL();

    if (cdc->in_ringbuf_handle) {
        vRingbufferDelete(cdc->in_ringbuf_handle);
    }
    if (cdc->out_ringbuf_handle) {
        vRingbufferDelete(cdc->out_ringbuf_handle);
    }
    if (cdc->data.out_xfer_free_sem) {
        vSemaphoreDelete(cdc->data.out_xfer_free_sem);
    }
    if (cdc) {
        free(cdc);
    }
    return ESP_OK;
}

esp_err_t usbh_cdc_write_bytes(usbh_cdc_handle_t cdc_handle, const uint8_t *buf, size_t length, TickType_t ticks_to_wait)
{
    esp_err_t ret = ESP_OK;
    ESP_GOTO_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "cdc_handle is NULL");
    ESP_GOTO_ON_FALSE(buf != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "buf is NULL");
    ESP_GOTO_ON_FALSE(length > 0, ESP_ERR_INVALID_ARG, fail, TAG, "length is 0");

    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    ESP_GOTO_ON_FALSE(cdc->state == USBH_CDC_OPEN, ESP_ERR_INVALID_STATE, fail, TAG, "Device is not connected");

    ret = _ringbuf_push(cdc->out_ringbuf_handle, buf, length, ticks_to_wait);
    if (ret != ESP_OK) {
        ESP_LOGD(TAG, "cdc write buf = %p, len = %d", buf, length);
        return ret;
    }

    if (xSemaphoreTake(cdc->data.out_xfer_free_sem, 0) == pdTRUE) {
        // here need a critical
        _cdc_tx_xfer_submit(cdc->data.out_xfer);
    }

    return ESP_OK;
fail:
    return ret;
}

esp_err_t usbh_cdc_read_bytes(usbh_cdc_handle_t cdc_handle, const uint8_t *buf, size_t *length, TickType_t ticks_to_wait)
{
    esp_err_t ret = ESP_OK;
    ESP_GOTO_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "cdc_handle is NULL");
    ESP_GOTO_ON_FALSE(buf != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "buf is NULL");
    ESP_GOTO_ON_FALSE(length != NULL, ESP_ERR_INVALID_ARG, fail, TAG, "length is NULL");

    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    ESP_GOTO_ON_FALSE(cdc->state == USBH_CDC_OPEN, ESP_ERR_INVALID_STATE, fail, TAG, "Device is not connected");

    size_t data_len = _get_ringbuf_len(cdc->in_ringbuf_handle);
    if (data_len > *length) {
        data_len = *length;
    }

    ret = _ringbuf_pop(cdc->in_ringbuf_handle, (uint8_t *)buf, data_len, length, pdMS_TO_TICKS(TIMEOUT_USB_RINGBUF_MS));
    if (ret != ESP_OK) {
        ESP_LOGD(TAG, "cdc read failed");
        *length = 0;
        return ret;
    }

    return ESP_OK;
fail:
    *length = 0;
    return ret;
}

esp_err_t usbh_cdc_flush_rx_buffer(usbh_cdc_handle_t cdc_handle)
{
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    _ring_buffer_flush(cdc->in_ringbuf_handle);
    return ESP_OK;
}

esp_err_t usbh_cdc_flush_tx_buffer(usbh_cdc_handle_t cdc_handle)
{
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    _ring_buffer_flush(cdc->out_ringbuf_handle);
    return ESP_OK;
}

esp_err_t usbh_cdc_get_rx_buffer_size(usbh_cdc_handle_t cdc_handle, size_t *size)
{
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    *size = _get_ringbuf_len(cdc->in_ringbuf_handle);
    return ESP_OK;
}

esp_err_t usbh_cdc_get_state(usbh_cdc_handle_t cdc_handle, usbh_cdc_state_t *state)
{
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    *state = cdc->state;
    return ESP_OK;
}

esp_err_t usbh_cdc_desc_print(usbh_cdc_handle_t cdc_handle)
{
    ESP_RETURN_ON_FALSE(cdc_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "cdc_handle is NULL");
    usbh_cdc_t *cdc = (usbh_cdc_t *) cdc_handle;
    ESP_RETURN_ON_FALSE(cdc->dev_hdl != NULL, ESP_ERR_INVALID_STATE, TAG, "Device is not open yet");

    const usb_device_desc_t *device_desc;
    const usb_config_desc_t *config_desc;
    ESP_ERROR_CHECK_WITHOUT_ABORT(usb_host_get_device_descriptor(cdc->dev_hdl, &device_desc));
    ESP_ERROR_CHECK_WITHOUT_ABORT(usb_host_get_active_config_descriptor(cdc->dev_hdl, &config_desc));
    usb_print_device_descriptor(device_desc);
    usb_print_config_descriptor(config_desc, NULL);
    return ESP_OK;
}
