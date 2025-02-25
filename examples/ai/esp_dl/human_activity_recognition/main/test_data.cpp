/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "test_data.h"

std::array<std::string, 6> index_to_category = {
    "LAYING",
    "SITTING",
    "STANDING",
    "WALKING",
    "WALKING_DOWNSTAIRS",
    "WALKING_UPSTAIRS"
};

const float mean_array[561] = {
    0.2745, -0.0177, -0.1091, -0.6054, -0.5109, -0.6048, -0.6305, -0.5269,
    -0.6062, -0.4686, -0.3060, -0.5571,  0.5236,  0.3874,  0.5944, -0.5476,
    -0.8200, -0.9019, -0.8458, -0.6843, -0.6438, -0.6311, -0.1030, -0.1379,
    -0.1639, -0.1166,  0.1028, -0.0378,  0.1305, -0.0262,  0.0263,  0.1600,
    -0.0196,  0.0094,  0.0333,  0.0366, -0.0786, -0.1251, -0.1938,  0.1050,
    0.6641,  0.0110,  0.0939, -0.9645, -0.9537, -0.9369, -0.9653, -0.9547,
    -0.9383,  0.6042, -0.0033,  0.0990,  0.6785,  0.0232,  0.0806, -0.0783,
    0.4367, -0.7304, -0.7447, -0.9676, -0.9580, -0.9430, -0.6675, -0.8691,
    -0.6847, -0.5158,  0.5532, -0.5899,  0.6260, -0.3453,  0.3309, -0.3604,
    0.4074, -0.4311,  0.4563, -0.4808,  0.5021,  0.1868, -0.1053,  0.0887,
    0.0791,  0.0086, -0.0047, -0.6436, -0.6107, -0.7605, -0.6400, -0.5951,
    -0.7543, -0.7003, -0.7507, -0.8183,  0.6236,  0.6882,  0.7364, -0.6478,
    -0.8504, -0.8280, -0.9275, -0.6288, -0.6578, -0.7690, -0.0994, -0.0997,
    -0.1337, -0.1067,  0.1620,  0.0636,  0.1207, -0.0693,  0.0644,  0.1730,
    0.3220, -0.0300,  0.0836, -0.0036,  0.1395, -0.1448,  0.0086,  0.0883,
    -0.0272, -0.0762,  0.0866, -0.7209, -0.6759, -0.6545, -0.7264, -0.6886,
    -0.6672, -0.6428, -0.7334, -0.4858,  0.6343,  0.7321,  0.5611, -0.6004,
    -0.9002, -0.8770, -0.8712, -0.7296, -0.7132, -0.7186, -0.1468, -0.1101,
    -0.0757, -0.2199,  0.1417,  0.1391, -0.0854, -0.2031,  0.1645, -0.0355,
    0.1382, -0.0806,  0.0622, -0.0041,  0.1482, -0.1731,  0.0010, -0.1175,
    -0.0979, -0.0425, -0.0543, -0.7300, -0.7839, -0.7421, -0.7277, -0.7941,
    -0.7482, -0.7510, -0.8084, -0.7472,  0.7596,  0.8304,  0.8007, -0.7653,
    -0.9142, -0.9364, -0.9194, -0.7340, -0.8057, -0.7666, -0.0275,  0.0015,
    -0.0291, -0.0708,  0.0343,  0.1610,  0.1736, -0.1601,  0.1940,  0.0851,
    0.0828, -0.0259,  0.0494,  0.0873,  0.0397,  0.0415,  0.0491, -0.1286,
    -0.5439, -0.5889, -0.6394, -0.5571, -0.8386, -0.5439, -0.7706, -0.6964,
    0.1243, -0.0725,  0.0250,  0.0607, -0.0587, -0.5439, -0.5889, -0.6394,
    -0.5571, -0.8386, -0.5439, -0.7706, -0.6964,  0.1243, -0.0725,  0.0250,
    0.0607, -0.0587, -0.6505, -0.6327, -0.6514, -0.6426, -0.7857, -0.6505,
    -0.8500, -0.7020, -0.0741,  0.0794, -0.0297, -0.0843, -0.0494, -0.6029,
    -0.6558, -0.6226, -0.6922, -0.7350, -0.6029, -0.8295, -0.6468,  0.2101,
    -0.0300, -0.0666,  0.1115, -0.0587, -0.7603, -0.7778, -0.7920, -0.7848,
    -0.8023, -0.7603, -0.9292, -0.8052,  0.1104,  0.2797, -0.2224, -0.0486,
    -0.1165, -0.6226, -0.5390, -0.6604, -0.6002, -0.5287, -0.6086, -0.5902,
    -0.5228, -0.6266, -0.6473, -0.6595, -0.6255, -0.8528, -0.8801, -0.9126,
    -0.5541, -0.8206, -0.7516, -0.8304, -0.6558, -0.6531, -0.7467, -0.2108,
    -0.1933, -0.2104, -0.7663, -0.7963, -0.8443, -0.2273,  0.0136,  0.0453,
    -0.1286, -0.4575, -0.2638, -0.5781, -0.2383, -0.4770, -0.8102, -0.8884,
    -0.8584, -0.8963, -0.9135, -0.9105, -0.9442, -0.9533, -0.8156, -0.8476,
    -0.9124, -0.9472, -0.8186, -0.8839, -0.7873, -0.8454, -0.8615, -0.9064,
    -0.8973, -0.8816, -0.9017, -0.9517, -0.7575, -0.8392, -0.8795, -0.9196,
    -0.7540, -0.8933, -0.8480, -0.8953, -0.9219, -0.9598, -0.9616, -0.9337,
    -0.9388, -0.9559, -0.8447, -0.9357, -0.9517, -0.9434, -0.8341, -0.9575,
    -0.6596, -0.6319, -0.7411, -0.6593, -0.6145, -0.7788, -0.6000, -0.6273,
    -0.7625, -0.7147, -0.6811, -0.7982, -0.8820, -0.8615, -0.8828, -0.6216,
    -0.8502, -0.8281, -0.9275, -0.6370, -0.7228, -0.7694, -0.2787, -0.2743,
    -0.3707, -0.4221, -0.3988, -0.3305, -0.0465, -0.2116, -0.1226, -0.3210,
    -0.7156, -0.3961, -0.8197, -0.4867, -0.8133, -0.8623, -0.8893, -0.8745,
    -0.9007, -0.9209, -0.9013, -0.9441, -0.9841, -0.8675, -0.8567, -0.9063,
    -0.9420, -0.8459, -0.8637, -0.8380, -0.8669, -0.8386, -0.9114, -0.9155,
    -0.8759, -0.9222, -0.9710, -0.8412, -0.8404, -0.8783, -0.9283, -0.8134,
    -0.8974, -0.8968, -0.8963, -0.9280, -0.9613, -0.9652, -0.9386, -0.9290,
    -0.9695, -0.8746, -0.9443, -0.9548, -0.9291, -0.8951, -0.9588, -0.6730,
    -0.7019, -0.6459, -0.7380, -0.6663, -0.6909, -0.6873, -0.7068, -0.6418,
    -0.7346, -0.7255, -0.7647, -0.9321, -0.8986, -0.9078, -0.6591, -0.9136,
    -0.8780, -0.8682, -0.7094, -0.7519, -0.6993, -0.1078, -0.0591, -0.1619,
    -0.8745, -0.8109, -0.8053, -0.1012, -0.1771, -0.0485, -0.1747, -0.4914,
    -0.1728, -0.5282, -0.2167, -0.5360, -0.9283, -0.9013, -0.9179, -0.9595,
    -0.9500, -0.9517, -0.9635, -0.9756, -0.9181, -0.9174, -0.9456, -0.9689,
    -0.9150, -0.9550, -0.8699, -0.9553, -0.9570, -0.9659, -0.9766, -0.9559,
    -0.9499, -0.9748, -0.8834, -0.9494, -0.9721, -0.9539, -0.8710, -0.9652,
    -0.8982, -0.9320, -0.9297, -0.9670, -0.9716, -0.9606, -0.9516, -0.9687,
    -0.8804, -0.9170, -0.9686, -0.9590, -0.8715, -0.9675, -0.5855, -0.6565,
    -0.5940, -0.7532, -0.8908, -0.5855, -0.8174, -0.6950, -0.1979, -0.7512,
    0.0731, -0.3473, -0.6266, -0.6243, -0.6469, -0.6187, -0.6912, -0.8063,
    -0.6243, -0.8422, -0.6786, -0.3477, -0.8780,  0.1782, -0.3130, -0.6154,
    -0.6932, -0.6929, -0.6748, -0.7266, -0.8851, -0.6932, -0.8743, -0.7198,
    -0.0879, -0.8894, -0.0465, -0.2536, -0.5654, -0.7794, -0.7924, -0.7728,
    -0.8114, -0.8719, -0.7794, -0.9358, -0.7715, -0.2846, -0.8989,  0.1253,
    -0.3070, -0.6253,  0.0087,  0.0022,  0.0087, -0.0060, -0.4895,  0.0586,
    -0.0565
};

const float std_array[561] = {
    0.0703, 0.0408, 0.0566, 0.4487, 0.5026, 0.4187, 0.4241, 0.4859, 0.4141,
    0.5445, 0.2822, 0.2939, 0.3636, 0.3436, 0.2978, 0.4718, 0.2596, 0.1263,
    0.2220, 0.3716, 0.3716, 0.3866, 0.4690, 0.4373, 0.3714, 0.3065, 0.2466,
    0.2436, 0.2301, 0.2574, 0.2150, 0.2088, 0.2214, 0.2861, 0.2163, 0.2362,
    0.2328, 0.3632, 0.3311, 0.3854, 0.5168, 0.3722, 0.3478, 0.0780, 0.0915,
    0.1072, 0.0767, 0.0904, 0.1061, 0.5101, 0.3605, 0.3433, 0.5087, 0.3697,
    0.3500, 0.4010, 0.6979, 0.4370, 0.4359, 0.0733, 0.0870, 0.1015, 0.3651,
    0.2814, 0.3965, 0.2136, 0.2077, 0.2064, 0.2095, 0.2927, 0.2991, 0.2918,
    0.2834, 0.2699, 0.2603, 0.2534, 0.2491, 0.6966, 0.7077, 0.7042, 0.1809,
    0.1637, 0.1599, 0.4119, 0.4337, 0.2874, 0.4176, 0.4500, 0.2934, 0.3664,
    0.2912, 0.2290, 0.4331, 0.3579, 0.3174, 0.3993, 0.2193, 0.2399, 0.1276,
    0.4359, 0.3834, 0.2755, 0.6533, 0.6372, 0.6143, 0.3019, 0.1924, 0.2408,
    0.1943, 0.2716, 0.2184, 0.2413, 0.2029, 0.2740, 0.1968, 0.2253, 0.2382,
    0.2559, 0.2878, 0.2722, 0.1822, 0.1324, 0.1393, 0.3067, 0.3713, 0.3773,
    0.3030, 0.3571, 0.3679, 0.2830, 0.2770, 0.3046, 0.2551, 0.2177, 0.3087,
    0.4103, 0.1472, 0.2043, 0.1981, 0.3069, 0.3344, 0.3244, 0.4570, 0.3740,
    0.4607, 0.2843, 0.2317, 0.2271, 0.2437, 0.2122, 0.1944, 0.2254, 0.2089,
    0.3177, 0.2873, 0.2696, 0.2615, 0.3897, 0.3930, 0.4288, 0.1275, 0.1170,
    0.1302, 0.3116, 0.2825, 0.3044, 0.3158, 0.2676, 0.2962, 0.2923, 0.2622,
    0.3018, 0.2854, 0.2315, 0.2515, 0.2770, 0.1413, 0.1445, 0.1374, 0.3128,
    0.2518, 0.2760, 0.5737, 0.5556, 0.5874, 0.2571, 0.1875, 0.2133, 0.2060,
    0.2168, 0.1671, 0.1926, 0.2427, 0.3487, 0.2351, 0.2519, 0.2448, 0.2752,
    0.2643, 0.2580, 0.4777, 0.4375, 0.3832, 0.4689, 0.1916, 0.4777, 0.2924,
    0.3251, 0.6767, 0.2950, 0.2385, 0.2547, 0.2651, 0.4777, 0.4375, 0.3832,
    0.4689, 0.1916, 0.4777, 0.2924, 0.3251, 0.6767, 0.2950, 0.2385, 0.2547,
    0.2651, 0.3956, 0.4188, 0.3992, 0.4063, 0.2625, 0.3956, 0.2123, 0.3453,
    0.7273, 0.2437, 0.2468, 0.2361, 0.2560, 0.4078, 0.3625, 0.3949, 0.3285,
    0.3099, 0.4078, 0.2306, 0.3721, 0.4865, 0.3045, 0.2730, 0.2501, 0.2456,
    0.2850, 0.2791, 0.2581, 0.2748, 0.2530, 0.2850, 0.1306, 0.2389, 0.7037,
    0.2428, 0.2109, 0.2258, 0.2451, 0.4282, 0.4837, 0.3701, 0.4578, 0.4823,
    0.4150, 0.4655, 0.4992, 0.4016, 0.4169, 0.3531, 0.4069, 0.2345, 0.1805,
    0.1334, 0.4801, 0.2595, 0.3090, 0.2436, 0.4013, 0.3914, 0.2965, 0.7251,
    0.6696, 0.6167, 0.2593, 0.2464, 0.2421, 0.2647, 0.2398, 0.2892, 0.4037,
    0.4462, 0.3585, 0.3888, 0.4046, 0.4145, 0.2833, 0.1789, 0.2287, 0.1807,
    0.1455, 0.1567, 0.1074, 0.1240, 0.2707, 0.2409, 0.1447, 0.1094, 0.2628,
    0.1907, 0.2713, 0.2419, 0.2163, 0.1564, 0.1741, 0.1913, 0.1694, 0.1276,
    0.3043, 0.2452, 0.1923, 0.1497, 0.3063, 0.1692, 0.2336, 0.1845, 0.1565,
    0.0882, 0.0846, 0.1288, 0.1186, 0.1227, 0.2285, 0.1263, 0.0984, 0.1142,
    0.2394, 0.0885, 0.3937, 0.4077, 0.3079, 0.3954, 0.4347, 0.2683, 0.4620,
    0.4172, 0.2855, 0.3410, 0.3704, 0.2530, 0.1910, 0.2139, 0.1847, 0.4276,
    0.2196, 0.2398, 0.1276, 0.4254, 0.3115, 0.2760, 0.7517, 0.7365, 0.6443,
    0.3267, 0.2680, 0.2977, 0.2944, 0.2764, 0.2754, 0.2552, 0.2104, 0.1905,
    0.1435, 0.2027, 0.1559, 0.2221, 0.1730, 0.2054, 0.1761, 0.1342, 0.1703,
    0.1047, 0.0580, 0.1994, 0.2295, 0.1538, 0.1087, 0.2247, 0.2236, 0.2234,
    0.2105, 0.2516, 0.1506, 0.1466, 0.2024, 0.1320, 0.0945, 0.2317, 0.2430,
    0.1963, 0.1217, 0.2620, 0.1636, 0.1761, 0.1860, 0.1435, 0.0865, 0.0807,
    0.1233, 0.1404, 0.1058, 0.2092, 0.1104, 0.0953, 0.1399, 0.1796, 0.0874,
    0.3574, 0.3473, 0.3857, 0.2914, 0.3857, 0.3418, 0.3410, 0.3388, 0.3894,
    0.3098, 0.3412, 0.2777, 0.1117, 0.1626, 0.1425, 0.3694, 0.1354, 0.2058,
    0.2059, 0.3415, 0.3149, 0.3543, 0.6149, 0.6188, 0.6085, 0.1982, 0.2823,
    0.2398, 0.2593, 0.2760, 0.2660, 0.3237, 0.3408, 0.3467, 0.3769, 0.3304,
    0.3436, 0.1217, 0.1703, 0.1447, 0.0873, 0.1095, 0.0972, 0.0852, 0.0738,
    0.1307, 0.1450, 0.1115, 0.0783, 0.1336, 0.0927, 0.2343, 0.1103, 0.1246,
    0.0938, 0.0647, 0.1036, 0.1205, 0.0800, 0.1993, 0.1373, 0.0715, 0.1125,
    0.2177, 0.0917, 0.1783, 0.1380, 0.1432, 0.0764, 0.0655, 0.0816, 0.1044,
    0.0943, 0.1946, 0.1655, 0.0683, 0.0972, 0.2022, 0.0719, 0.4527, 0.3626,
    0.4366, 0.2651, 0.1615, 0.4527, 0.2561, 0.3504, 0.6827, 0.2687, 0.2672,
    0.3286, 0.3274, 0.4288, 0.4047, 0.4337, 0.3660, 0.2695, 0.4288, 0.2306,
    0.3706, 0.6701, 0.1886, 0.2538, 0.3586, 0.3453, 0.3350, 0.3229, 0.3435,
    0.2931, 0.1736, 0.3350, 0.1927, 0.3187, 0.6118, 0.1577, 0.2827, 0.3266,
    0.3266, 0.2757, 0.2654, 0.2876, 0.2467, 0.1933, 0.2757, 0.1387, 0.2876,
    0.6309, 0.1431, 0.2510, 0.3210, 0.3076, 0.3368, 0.4483, 0.6083, 0.4780,
    0.5118, 0.2975, 0.2791
};

const float test_inputs[3][561] = {
    {0.25717778, -0.02328523, -0.014653762, -0.938404, -0.92009078, -0.66768331, -0.95250112, -0.92524867, -0.67430222, -0.89408755, -0.55457721, -0.46622295, 0.71720847, 0.6355024, 0.78949666, -0.87776423, -0.99776606, -0.99841381, -0.93434525, -0.97566897, -0.94982365, -0.8304778, -0.16808416, -0.37899553, 0.24621698, 0.52120364, -0.48779311, 0.48228047, -0.045462113, 0.21195505, -0.13489443, 0.13085848, -0.014176313, -0.10597085, 0.073544013, -0.17151642, 0.040062978, 0.076988933, -0.49054573, -0.70900265, 0.93648925, -0.28271916, 0.11528825, -0.92542727, -0.93701413, -0.56428842, -0.93001992, -0.93782195, -0.6055877, 0.90608259, -0.27924413, 0.15289519, 0.9444614, -0.26215956, -0.076161676, -0.01782692, 0.82929682, -0.8646206, -0.96779531, -0.94972666, -0.9461192, -0.75971815, -0.42497535, -1.0, 0.21922731, -0.43025357, 0.43104828, -0.43183892, 0.4327738, -0.79546772, 0.78131389, -0.78039147, 0.78527158, -0.98441024, 0.98717986, -0.98941477, 0.98768613, 0.98058028, -0.99635177, -0.96011706, 0.072046007, 0.045754401, -0.10604266, -0.90668276, -0.93801639, -0.93593583, -0.91608093, -0.93672546, -0.94905379, -0.90322415, -0.94981833, -0.89140347, 0.89847935, 0.95018164, 0.94615279, -0.93067288, -0.99504593, -0.99749551, -0.9970156, -0.936416, -0.94687413, -0.96877461, -0.085174151, -0.31026304, -0.51028758, 0.52148173, -0.22588966, 0.49172843, 0.31275555, 0.2297968, 0.11395925, 0.21987861, 0.42297454, -0.082633177, 0.14042653, -0.19623228, 0.072357939, -0.26486023, 0.03585215, -0.34973525, 0.11997616, -0.091792335, 0.18962854, -0.88308911, -0.8161636, -0.94088123, -0.88861231, -0.85780102, -0.94581827, -0.66341057, -0.71343663, -0.64867861, 0.83710039, 0.82525677, 0.81097714, -0.79649994, -0.97961636, -0.98290006, -0.99403684, -0.88655788, -0.90610426, -0.95804876, 0.77403279, -0.26770588, 0.45224806, -0.078451267, -0.012578616, 0.23598156, -0.19904751, 0.03391784, -0.080780533, 0.0069987153, 0.24488551, 0.21651661, -0.27968077, 0.24973875, 0.017719752, 0.64846454, -0.23693109, -0.30173469, -0.20489621, -0.17448771, -0.09338934, -0.90122415, -0.91086005, -0.93925042, -0.91036271, -0.92735675, -0.95355413, -0.86791431, -0.91349778, -0.89757791, 0.90493669, 0.91730839, 0.9476122, -0.92960905, -0.99468622, -0.99579057, -0.99781265, -0.93654081, -0.95887957, -0.97034831, 0.03661912, 0.076459933, -0.19712605, 0.10651426, -0.020811895, 0.19325784, 0.30447875, 0.11572923, 0.0541496, 0.068951237, 0.1970496, 0.30992826, -0.21265711, 0.17317814, 0.14584454, 0.12400875, -0.15534634, -0.32343727, -0.86692938, -0.70519112, -0.74402172, -0.76079564, -0.9816487, -0.86692938, -0.98016578, -0.85947423, 0.25510436, 0.053779695, -0.20414449, 0.61052755, -0.56444932, -0.86692938, -0.70519112, -0.74402172, -0.76079564, -0.9816487, -0.86692938, -0.98016578, -0.85947423, 0.25510436, 0.053779695, -0.20414449, 0.61052755, -0.56444932, -0.92976655, -0.89599425, -0.90041731, -0.90300439, -0.97501109, -0.92976655, -0.99560772, -0.91412066, -0.12955231, 0.23891093, -0.34559715, 0.32646236, -0.263048, -0.79554393, -0.76207322, -0.78267232, -0.71659365, -0.76419261, -0.79554393, -0.97415212, -0.83958101, 0.66756269, 0.035621137, -0.16189398, 0.15325006, -0.006759604, -0.92519489, -0.89434361, -0.90014668, -0.91673708, -0.97636665, -0.92519489, -0.99582422, -0.9118375, 0.33165431, 0.51695316, -0.513504, 0.041319806, 0.011835012, -0.91850969, -0.91821319, -0.78909145, -0.94829035, -0.92513687, -0.63631674, -0.93068029, -0.92443848, -0.72490255, -0.96842407, -0.94013675, -0.59718873, -0.96613713, -0.98445054, -0.95208707, -0.86506318, -0.99784371, -0.99603625, -0.94019503, -0.90484043, -0.93381245, -0.86938291, -0.33967327, -0.48580324, -0.16625765, -1.0, -1.0, -1.0, 0.011116947, 0.12125069, -0.52294869, -0.5719995, -0.89461236, -0.33826592, -0.68679745, 0.1895525, -0.11359571, -0.99850627, -0.99794262, -0.99559788, -0.99503559, -0.9959769, -0.99148743, -0.99213116, -0.99977512, -0.99820395, -0.99474673, -0.99430903, -0.99469323, -0.99802046, -0.99372747, -0.99620895, -0.99797615, -0.99892091, -0.99640106, -0.99755992, -0.99506803, -0.99735126, -0.99823805, -0.99607053, -0.99792459, -0.99635664, -0.9976398, -0.99622004, -0.99622539, -0.9247055, -0.99409728, -0.99532347, -0.99896944, -0.99790162, -0.99492962, -0.98926072, -0.9800854, -0.93793669, -0.99667757, -0.9970254, -0.98642738, -0.93920079, -0.99843184, -0.89963316, -0.937485, -0.9235514, -0.92442913, -0.94321038, -0.94789152, -0.89661455, -0.93830911, -0.9425757, -0.94863426, -0.95832542, -0.95881686, -0.94388241, -0.98730333, -0.97846559, -0.90527425, -0.99503609, -0.99749931, -0.9970307, -0.88707736, -0.93581956, -0.95365327, -0.4706616, -0.6721718, -0.59627404, -0.52, 0.08, 0.32, 0.45100539, 0.13716703, -0.18029913, -0.58008614, -0.90807003, -0.62552686, -0.94275716, -0.66191006, -0.91153211, -0.99901148, -0.99763006, -0.99598477, -0.99470846, -0.99557158, -0.98787651, -0.98644523, -0.99820517, -0.99804041, -0.99449682, -0.99204962, -0.98633303, -0.99689353, -0.99076269, -0.99940916, -0.99864806, -0.99874875, -0.99640627, -0.99717222, -0.9945302, -0.99791979, -0.9999697, -0.99864167, -0.9974017, -0.99541085, -0.9981824, -0.99846306, -0.99599581, -0.99363885, -0.9978378, -0.99712931, -0.9987495, -0.99670876, -0.99510966, -0.99848581, -0.99909354, -0.99592463, -0.99795094, -0.9960129, -0.99846008, -0.99626705, -0.99770046, -0.82355788, -0.80791598, -0.91791256, -0.90326274, -0.822677, -0.95616508, -0.86512704, -0.83180082, -0.94105617, -0.90479801, -0.87925663, -0.96778849, -0.87859934, -0.94832915, -0.91968517, -0.82847203, -0.99294953, -0.98266311, -0.99799327, -0.8788944, -0.83815197, -0.92913995, 0.00075814806, 0.20014368, -0.25338416, -1.0, -0.93548387, -0.93103448, 0.18403457, -0.059322857, 0.43810716, -0.39542276, -0.6987616, -0.38745724, -0.78639417, -0.48565359, -0.78681512, -0.99462908, -0.99046914, -0.99277782, -0.99567969, -0.98707772, -0.98665097, -0.98411445, -0.98525021, -0.99355255, -0.99243025, -0.98560582, -0.98461702, -0.99326921, -0.99272545, -0.97792782, -0.99485223, -0.99781915, -0.99484295, -0.99240991, -0.98812267, -0.99043323, -0.98796127, -0.98179096, -0.99639919, -0.99145229, -0.98805999, -0.98204835, -0.99337999, -0.99887769, -0.99836665, -0.9984673, -0.99837462, -0.99890591, -0.99589361, -0.99312836, -0.99547231, -0.9983871, -0.9977543, -0.99809593, -0.99414765, -0.99819058, -0.99829005, -0.79094643, -0.711074, -0.72670699, -0.77769715, -0.94488134, -0.79094643, -0.95398356, -0.87354261, -0.17459288, -1.0, -0.48345254, 0.011040681, -0.38451662, -0.89506118, -0.89635958, -0.8881974, -0.92846566, -0.89809981, -0.89506118, -0.99347143, -0.92147669, -0.48461929, -1.0, -0.035355792, -0.2542483, -0.70032573, -0.77061, -0.79711285, -0.76448457, -0.8201876, -0.93795935, -0.77061, -0.97095802, -0.79838652, 0.17943523, -1.0, -0.047391298, -0.46784901, -0.76132577, -0.89016545, -0.90730756, -0.89530057, -0.91788296, -0.90982876, -0.89016545, -0.99410543, -0.89802151, -0.23481529, -1.0, 0.071645446, -0.33037044, -0.70597388, 0.0064624029, 0.16291982, -0.82588562, 0.27115145, -0.72000927, 0.27680104, -0.057978304},
    {0.24042087, 0.0010854188, -0.11888144, -0.4333206, -0.097815183, -0.39734057, -0.47512068, -0.14434012, -0.37872009, -0.1826169, -0.19282589, -0.37873277, 0.42833295, 0.054422023, 0.52344457, -0.28991195, -0.83783062, -0.84160933, -0.83357654, -0.58105876, -0.40435334, -0.39912662, 0.35491625, 0.44395984, 0.15316495, -0.4850793, 0.63664968, -0.67823583, 0.4052375, -0.49317973, 0.68460872, -0.57185596, 0.40531836, -0.32001254, 0.34497695, -0.24627671, 0.069808711, -0.2681181, -0.055639614, 0.33506145, 0.91880373, -0.35224337, 0.023289529, -0.98868188, -0.98806868, -0.97528476, -0.98983981, -0.98957114, -0.97516803, 0.84937665, -0.36582442, 0.019917355, 0.93550186, -0.32413545, 0.020014926, -0.13314213, 0.78349389, -0.78457298, -0.9993557, -0.99196175, -0.99263712, -0.97700758, -0.68881138, -1.0, -0.41871547, -0.30888832, 0.37119377, -0.42946451, 0.48358559, 0.22858113, -0.22704366, 0.14595104, -0.037026195, -0.37539559, 0.4268184, -0.47695081, 0.52304598, 0.62467851, 0.090942591, 0.064100983, 0.097059424, 0.33122146, 0.17005394, -0.29554297, -0.031869747, -0.59671071, -0.31873717, -0.1190393, -0.58765111, -0.42098969, -0.36471768, -0.79875861, 0.018043989, 0.030425874, 0.40476205, -0.31619816, -0.74921778, -0.52623728, -0.91387988, -0.35390259, -0.39536306, -0.59337812, 0.6384856, 0.71420248, 0.59748854, -0.4229259, 0.59975241, -0.17462781, -0.071647604, -0.63189662, 0.8702384, -0.39073266, 0.279648, -0.38283377, 0.30425484, -0.19897764, 0.069053232, -0.39567623, -0.082576835, 0.15479888, 0.023527608, -0.1542316, 0.11124867, -0.55491993, -0.53569402, -0.51301861, -0.55738219, -0.54448696, -0.54627296, -0.46546555, -0.6563163, -0.36413875, 0.43246054, 0.69106846, 0.38471693, -0.43718104, -0.90768843, -0.89198278, -0.88908561, -0.52099366, -0.57643357, -0.64726079, 0.36393924, -0.0084202916, 0.28996862, -0.49749786, 0.49361995, -0.18933673, -0.047315858, -0.2263949, 0.18843076, -0.12072671, 0.21071902, -0.24994507, 0.23067723, -0.14842034, 0.26585236, -0.3625547, 0.11170802, -0.016086376, -0.18774428, -0.053269404, 0.063810809, -0.45848865, -0.73030852, -0.61377927, -0.47822119, -0.72670082, -0.61037132, -0.50029383, -0.77338186, -0.60917208, 0.4385881, 0.76490132, 0.7992804, -0.6315277, -0.85176587, -0.96307053, -0.92358983, -0.54451757, -0.69567309, -0.63907677, 0.48250308, 0.60468833, 0.66274005, -0.30120601, 0.28436389, 0.21591966, -0.12070834, -0.13833299, 0.10033949, 0.22315627, -0.46419744, -0.20387042, 0.054018903, 0.20889367, -0.37596278, 0.23217902, 0.077587378, -0.081949634, -0.29734115, -0.42895452, -0.5211776, -0.1798334, -0.65776317, -0.29734115, -0.74794623, -0.61711726, 0.68333267, 0.16215378, -0.095874794, -0.19758234, 0.27252924, -0.29734115, -0.42895452, -0.5211776, -0.1798334, -0.65776317, -0.29734115, -0.74794623, -0.61711726, 0.68333267, 0.16215378, -0.095874794, -0.19758234, 0.27252924, -0.32370844, -0.13166261, -0.20623225, -0.11715151, -0.80323066, -0.32370844, -0.7169947, -0.39557677, 0.70369631, 0.38764674, -0.21111204, -0.69926631, 0.47551163, -0.45105168, -0.5746539, -0.53617733, -0.60845107, -0.57337437, -0.45105168, -0.8472381, -0.56038559, 0.88304946, -0.10634228, 0.072441606, -0.04082884, -0.0020605491, -0.63319332, -0.68112708, -0.70316195, -0.730352, -0.62795086, -0.63319332, -0.93446491, -0.71494508, 0.81574796, 0.34423496, -0.21336435, -0.10038215, -0.16391015, -0.35473941, -0.05344303, -0.40440953, -0.46740777, -0.17976544, -0.44173508, -0.34879068, -0.013813707, -0.35993424, -0.61604134, -0.52651288, -0.58087509, -0.79563652, -0.81939988, -0.9130926, -0.18166718, -0.83769259, -0.58802844, -0.81142843, -0.17829225, -0.09846522, -0.47998143, 0.61817518, 0.49693722, 0.48522238, -0.74193548, -0.73333333, -0.69230769, -0.27471654, 0.18644497, -0.054066434, -0.33913267, -0.72629106, -0.63875637, -0.92248891, -0.60930329, -0.84580476, -0.84731151, -0.9345147, -0.64871349, -0.57649799, -0.92405355, -0.91586333, -0.98077698, -0.9933768, -0.85880887, -0.5721545, -0.92099788, -0.98500012, -0.84387762, -0.65969421, -0.75452899, -0.72438542, -0.50916079, -0.44835559, -0.8508413, -0.89799223, -0.81760412, -0.84499271, -0.67013338, -0.36626187, -0.85186025, -0.82323589, -0.61196708, -0.54581174, -0.84400145, -0.84561257, -0.92279716, -0.9501467, -0.96646778, -0.90069803, -0.98545948, -0.96269095, -0.82554141, -0.93273695, -0.94702573, -0.97851805, -0.81574877, -0.94927308, -0.31830435, -0.15643063, -0.55297127, -0.33425488, 0.037859011, -0.63900985, -0.14180068, 0.061814035, -0.58353903, -0.50505808, -0.2431743, -0.71436714, -0.68958123, -0.92244517, -0.92777356, -0.23215345, -0.74874788, -0.52657651, -0.91391753, -0.12385306, -0.22181561, -0.54075003, 0.59695706, 0.59897224, 0.27665714, -0.32, -0.24, -0.24, -0.13842892, -0.48324767, -0.48604047, -0.36385447, -0.80029567, -0.34532011, -0.85811345, -0.62410938, -0.91748477, -0.89525357, -0.91807272, -0.65658089, -0.54532121, -0.9226741, -0.87783399, -0.97306601, -0.9836057, -0.90068108, -0.52751031, -0.89779663, -0.97036295, -0.78845623, -0.59565651, -0.8148649, -0.72702589, -0.38586254, -0.48524424, -0.89049163, -0.92868762, -0.94067943, -0.99439882, -0.70885813, -0.30917255, -0.88050308, -0.9474602, -0.51780002, -0.6309184, -0.88180801, -0.84086328, -0.92324075, -0.95506379, -0.96669194, -0.93567397, -0.98054481, -0.99691183, -0.82190205, -0.93882856, -0.95509305, -0.98118654, -0.86847395, -0.95510081, -0.47634143, -0.60573635, -0.45105837, -0.58049721, -0.50041274, -0.57961476, -0.49564752, -0.60029622, -0.44355215, -0.5212868, -0.54268593, -0.74700203, -0.87961637, -0.98839065, -0.91320088, -0.49583681, -0.90027737, -0.89127792, -0.87976002, -0.44470815, -0.6927259, -0.51355793, 0.43748233, 0.53025005, 0.37642503, -0.93333333, -0.93548387, -0.79310345, -0.087272348, -0.31908176, -0.087324629, 0.0013437845, -0.28235867, -0.026467389, -0.41971431, -0.47360708, -0.81262489, -0.93082254, -0.83372575, -0.82877214, -0.90969008, -0.96144182, -0.96701192, -0.96702113, -0.99462955, -0.91059641, -0.82542897, -0.95983377, -0.97923743, -0.9027839, -0.9231795, -0.84177298, -0.98312135, -0.98829527, -0.96355636, -0.98871424, -0.97027426, -0.96499421, -0.99993012, -0.88248377, -0.97802044, -0.98481224, -0.97536909, -0.88528063, -0.96739754, -0.90832298, -0.93596595, -0.9371809, -0.96008731, -0.98101048, -0.94836541, -0.95240224, -0.97250893, -0.89133126, -0.92077846, -0.97214936, -0.96114416, -0.88340333, -0.96383175, -0.34492915, -0.57103699, -0.49633677, -0.72904217, -0.86719274, -0.34492915, -0.83279552, -0.60660094, 0.430933, -0.44827586, 0.27013392, -0.53134742, -0.82050342, -0.10964598, -0.16676346, -0.10018567, -0.31033982, -0.57183652, -0.10964598, -0.61742225, -0.20284748, 0.5066423, -0.9047619, 0.28044601, -0.15887811, -0.54737596, -0.58143931, -0.64377465, -0.57004107, -0.72684996, -0.85956355, -0.58143931, -0.90832954, -0.53574143, 0.47395774, -0.38461538, 0.14978806, -0.58719762, -0.86259817, -0.67408982, -0.71347279, -0.70618411, -0.7126573, -0.77432613, -0.67408982, -0.94834815, -0.73988632, 0.21063057, -0.9047619, 0.16540701, -0.10455486, -0.46917722, 0.61709609, 0.13446432, -0.80296989, 0.47905143, -0.67087681, 0.32519581, 0.0056664328},
    {0.2642178, -0.012400398, -0.092979284, -0.98013229, -0.90345164, -0.94542955, -0.9812281, -0.91381476, -0.94139804, -0.93092106, -0.45898872, -0.78570579, 0.82568797, 0.65682465, 0.82771598, -0.95036114, -0.99968255, -0.99782074, -0.99757431, -0.98261583, -0.93318527, -0.93168039, -0.50264069, -0.18991514, -0.12475449, 0.19230776, -0.11958131, -0.014045198, 0.028428416, -0.16801643, 0.23046714, -0.015558961, 0.070794021, 0.099550164, -0.011100113, -0.1138929, 0.12646763, -0.34489091, -0.49295135, 0.12931765, 0.70593038, 0.52395614, 0.064284463, -0.98456578, -0.98435629, -0.97747105, -0.98527631, -0.98496187, -0.9780714, 0.63511391, 0.48904635, 0.062679238, 0.72363842, 0.54062909, 0.062543905, 0.13155035, 0.28022123, -0.4332041, -0.99317178, -0.98934809, -0.98900852, -0.97930208, -0.71299878, -0.51341858, -0.4999555, -0.41838596, 0.41737351, -0.41614002, 0.41483058, -0.19305941, 0.15022836, -0.16053898, 0.19363643, -0.33253254, 0.33493007, -0.3370838, 0.33668477, -0.99740515, -0.79695963, 0.75842032, 0.070220307, 0.016470488, 0.017890424, -0.97610281, -0.90820341, -0.96520777, -0.97882496, -0.91309714, -0.95793022, -0.97515398, -0.90255689, -0.98088338, 0.97208517, 0.92970289, 0.97096601, -0.95370846, -0.99955348, -0.99494114, -0.9988887, -0.98105204, -0.93251632, -0.94224119, -0.53544181, -0.22975548, -0.38627947, 0.22479668, 0.27168086, 0.097158863, 0.41040622, -0.24266289, 0.21769323, 0.21077206, 0.082187105, -0.0033205124, 0.17005008, -0.23366668, 0.36155823, 0.10520344, 0.08116087, -0.043946552, -0.030707815, -0.053496314, 0.067016666, -0.97344807, -0.95802339, -0.93040298, -0.97715593, -0.96104039, -0.94676756, -0.85684224, -0.91593837, -0.69974213, 0.81779634, 0.89838248, 0.73237607, -0.95670647, -0.99962797, -0.99895309, -0.99734818, -0.98291578, -0.96822133, -0.97120848, -0.28909685, -0.032800429, -0.4082277, -0.015147034, 0.13398033, -0.037967603, 0.098583149, -0.29931049, 0.21421838, 0.012704544, 0.06342852, -0.20292906, 0.17054822, -0.057113402, 0.13300519, -0.10037189, 0.08117216, -0.35723406, -0.098347385, -0.031376219, -0.051145878, -0.95890571, -0.9747058, -0.94661604, -0.9617873, -0.97699369, -0.95314945, -0.96101695, -0.97739502, -0.91252285, 0.94164262, 0.98000274, 0.96443184, -0.9680824, -0.9989952, -0.9996192, -0.99827745, -0.9665989, -0.98273904, -0.9577083, -0.12913341, -0.079567897, -0.13621177, 0.055879499, 0.2322873, 0.073444134, 0.31500355, -0.23774467, 0.15986581, 0.14190055, -0.015881456, -0.16344372, 0.16906398, -0.048411729, 0.17504504, 0.22772462, 0.18502374, 0.305868, -0.95062036, -0.943394, -0.95307944, -0.92274311, -0.9841409, -0.95062036, -0.99796542, -0.95707721, -0.181809, 0.12920974, -0.090630846, 0.081742985, -0.12175694, -0.95062036, -0.943394, -0.95307944, -0.92274311, -0.9841409, -0.95062036, -0.99796542, -0.95707721, -0.181809, 0.12920974, -0.090630846, 0.081742985, -0.12175694, -0.95168305, -0.94567412, -0.95493649, -0.9271579, -0.9496825, -0.95168305, -0.9980592, -0.96202141, -0.39553515, 0.31404019, -0.095697722, -0.36965094, 0.020594803, -0.95707033, -0.94007081, -0.93722367, -0.94807592, -0.99375142, -0.95707033, -0.99855343, -0.95735884, 0.24071849, 0.077300637, -0.11984314, -0.10258874, 0.28257274, -0.96858916, -0.96458564, -0.96661783, -0.96524348, -0.98917331, -0.96858916, -0.9993089, -0.96689283, -0.002130926, 0.11042825, -0.14855452, 0.11647213, -0.23306961, -0.97877267, -0.8978991, -0.95131994, -0.98057108, -0.91169, -0.94538222, -0.98036974, -0.8982889, -0.94967609, -0.97923889, -0.94797695, -0.93674277, -0.99399753, -0.99145138, -0.98426889, -0.94455144, -0.99971716, -0.99441611, -0.99763262, -0.97696116, -0.90399254, -0.95621306, -0.8207533, -0.37068446, -0.5435891, -1.0, -1.0, -1.0, -0.072440784, -0.010785566, 0.12000399, 0.11809902, -0.16020494, -0.5973486, -0.89687928, -0.15382794, -0.40953696, -0.99973599, -0.9998159, -0.9996186, -0.9994986, -0.99917467, -0.99957853, -0.99983436, -0.99998017, -0.99973736, -0.99952909, -0.99934112, -0.99988324, -0.99973066, -0.99933391, -0.99604775, -0.99550896, -0.99690042, -0.99526857, -0.99773116, -0.9973666, -0.99808307, -0.99997882, -0.99467774, -0.99564485, -0.99736373, -0.99882123, -0.99459081, -0.99570083, -0.99804369, -0.99791626, -0.99922185, -0.99914604, -0.99878574, -0.99787884, -0.9990731, -0.9996509, -0.99779547, -0.99922775, -0.99850944, -0.99924762, -0.99773232, -0.99898127, -0.97533763, -0.91019986, -0.9606675, -0.9792304, -0.91230985, -0.96828419, -0.97300698, -0.91089461, -0.96157315, -0.98688248, -0.92991582, -0.97597843, -0.9876707, -0.98486934, -0.97626772, -0.94588448, -0.99955274, -0.99494148, -0.99888913, -0.97306573, -0.92956985, -0.94959909, -1.0, -0.56214426, -0.75216047, -0.44, -0.44, -0.6, 0.30734129, -0.32078892, 0.0062066333, -0.58750807, -0.89693027, -0.46601518, -0.88252193, -0.66813551, -0.9523061, -0.99995753, -0.99975136, -0.99960918, -0.99943129, -0.99906117, -0.99933739, -0.99973989, -0.99997942, -0.99982302, -0.99945019, -0.99911049, -0.99974045, -0.99971286, -0.99899442, -0.99657698, -0.9960854, -0.99585151, -0.99529971, -0.99788402, -0.99657213, -0.99765492, -0.99995719, -0.99565074, -0.99476584, -0.99686197, -0.99794937, -0.99501177, -0.99586165, -0.99945757, -0.99819744, -0.9993796, -0.99911909, -0.99875779, -0.998134, -0.9987129, -0.99984727, -0.99829838, -0.99928172, -0.99849402, -0.99876188, -0.99886164, -0.99889961, -0.9600885, -0.95804214, -0.92626622, -0.97795576, -0.95821209, -0.93803909, -0.96731793, -0.95846047, -0.9219046, -0.98226586, -0.97135643, -0.95798848, -0.99714857, -0.96964124, -0.99604065, -0.94943444, -0.99959648, -0.99901129, -0.99727534, -0.9461619, -0.96633199, -0.93350262, -0.37190144, -0.3016243, -0.27081281, -0.8, -0.80645161, -1.0, 0.093539499, -0.249202, -0.20718196, -0.56010161, -0.82013042, -0.46636168, -0.8409971, -0.40297808, -0.75393066, -0.99973165, -0.99944531, -0.9992099, -0.99920337, -0.99878751, -0.99914332, -0.99987282, -0.99999711, -0.99966666, -0.99904153, -0.99881187, -0.99992783, -0.99962953, -0.99908261, -0.99883367, -0.99957608, -0.99981346, -0.99987973, -0.99987339, -0.99964464, -0.99960611, -0.99981781, -0.99893978, -0.99978934, -0.99982553, -0.99964427, -0.99892674, -0.99985389, -0.99800914, -0.99822772, -0.99858343, -0.99936847, -0.99925672, -0.99932181, -0.99974598, -0.99998537, -0.99751423, -0.99834613, -0.99927753, -0.99985027, -0.99733742, -0.99934215, -0.93710979, -0.95518287, -0.94369947, -0.96273559, -0.98204644, -0.93710979, -0.99783609, -0.9413113, -0.56237257, -1.0, 0.1495591, -0.43240941, -0.67362695, -0.94332695, -0.9476624, -0.94053302, -0.9580242, -0.96252796, -0.94332695, -0.99792228, -0.95038885, -0.79890145, -0.96825397, 0.22164382, -0.34082175, -0.71921809, -0.95511057, -0.94057771, -0.94619622, -0.94461146, -0.98828991, -0.95511057, -0.99799938, -0.95884571, -0.43366076, -0.94871795, -0.22501364, -0.10846569, -0.51596642, -0.96823882, -0.9621074, -0.96372142, -0.96390495, -0.97643794, -0.96823882, -0.99926771, -0.96710214, -0.54749067, -0.96825397, -0.21767428, 0.031153401, -0.41405422, 0.07422563, 0.10726642, -0.097186097, -0.36316283, -0.41608825, -0.30625588, -0.024348708}
};
