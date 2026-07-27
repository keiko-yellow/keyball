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
 * Remap上の実際のレイヤー構成:
 *   0 = 通常
 *   1 = マウス
 *   2 = Fキー
 *   3 = テンキー・記号
 */
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 1
#define AUTO_MOUSE_LAYER_KEEP_TIME 30000
#define AUTO_MOUSE_THRESHOLD 30

/*
 * Mod-Tapキーを含むComboとレイヤー限定Comboに必要
 */
#define COMBO_ALLOW_ACTION_KEYS
#define COMBO_TERM_PER_COMBO
#define COMBO_SHOULD_TRIGGER

#define COMBO_COUNT 25
#define COMBO_TERM 80
