/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 9.1.0
// Project name: RGB565

#include "../ui.h"

void ui_ScreenRGB565_screen_init(void)
{
    ui_ScreenRGB565 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_ScreenRGB565, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelRGB565 = lv_label_create(ui_ScreenRGB565);
    lv_obj_set_width(ui_LabelRGB565, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelRGB565, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelRGB565, lv_pct(-1));
    lv_obj_set_y(ui_LabelRGB565, lv_pct(-5));
    lv_obj_set_align(ui_LabelRGB565, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelRGB565, "This is an LVGL RGB565 non-overlay scenario.");
    lv_obj_set_style_text_font(ui_LabelRGB565, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

}
