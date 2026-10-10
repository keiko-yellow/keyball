/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
*/

#pragma once

#ifdef RGBLIGHT_ENABLE
// #define RGBLIGHT_EFFECT_BREATHING
// #define RGBLIGHT_EFFECT_RAINBOW_MOOD
// #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
// #define RGBLIGHT_EFFECT_SNAKE
// #define RGBLIGHT_EFFECT_KNIGHT
// #define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
// #define RGBLIGHT_EFFECT_RGB_TEST
// #define RGBLIGHT_EFFECT_ALTERNATING
// #define RGBLIGHT_EFFECT_TWINKLE
#endif

#define TAP_CODE_DELAY 5

/*
 * Layers:
 *   0 = BASE (roBa-style typing layout)
 *   1 = AUTO_MOUSE / manual mouse layer
 *   2 = existing Keyball61 mouse/number/F-key layer
 *   3 = existing Keyball61 config/scroll layer
 */
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 1
#define AUTO_MOUSE_LAYER_KEEP_TIME 30000
#define AUTO_MOUSE_THRESHOLD 30

#define COMBO_ALLOW_ACTION_KEYS
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER

#define COMBO_COUNT 29
#define COMBO_TERM 80
