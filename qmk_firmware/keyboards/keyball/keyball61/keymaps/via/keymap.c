/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
*/

#include QMK_KEYBOARD_H
#include "quantum.h"

/*
 * Keyball44と同じレイヤー0長押し設定
 */
#define E_CTL LCTL_T(KC_E)
#define I_ALT LALT_T(KC_I)
#define Z_GUI LGUI_T(KC_Z)
#define S_ALT RALT_T(KC_S)
#define H_CTL RCTL_T(KC_H)
#define B_GUI LGUI_T(KC_B)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Keyball61の物理配列・レイヤー配置は維持
     */
    [0] = LAYOUT_universal(
        KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_DEL ,
        KC_Q   , KC_W   , E_CTL  , KC_R   , KC_T   , KC_Y   , KC_U   , I_ALT  , KC_O   , KC_P   , KC_INT3,
        KC_TAB , KC_A   , S_ALT  , KC_D   , KC_F   , KC_G   , H_CTL  , KC_J   , KC_K   , KC_L   , KC_SCLN, S(KC_7),
        MO(1)  , Z_GUI  , KC_X   , KC_C   , KC_V   , B_GUI  , KC_RBRC, KC_NUHS, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
        _______, KC_LCTL, KC_LALT, KC_LGUI, LT(1,KC_LNG2), LT(2,KC_SPC), LT(3,KC_LNG1), KC_BSPC, LT(2,KC_ENT), LT(1,KC_LNG2), KC_RGUI, _______, KC_RALT, KC_PSCR
    ),

    [1] = LAYOUT_universal(
        S(KC_ESC), S(KC_1), KC_LBRC, S(KC_3), S(KC_4), S(KC_5), KC_EQL, S(KC_6), S(KC_QUOT), S(KC_8), S(KC_9), S(KC_INT1), S(KC_DEL),
        S(KC_Q), S(KC_W), S(KC_E), S(KC_R), S(KC_T), S(KC_Y), S(KC_U), S(KC_I), S(KC_O), S(KC_P), S(KC_INT3),
        S(KC_TAB), S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), KC_QUOT, S(KC_2),
        _______, S(KC_Z), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_RBRC), S(KC_NUHS), S(KC_N), S(KC_M), S(KC_COMM), S(KC_DOT), S(KC_SLSH), S(KC_RSFT),
        _______, S(KC_LCTL), S(KC_LALT), S(KC_LGUI), _______, _______, _______, _______, _______, _______, S(KC_RGUI), _______, S(KC_RALT), _______
    ),

    /*
     * Keyball61のマウス兼数字レイヤー
     */
    [2] = LAYOUT_universal(
        SSNP_FRE, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, SSNP_VRT,
        _______, KC_7, KC_8, KC_9, _______, _______, KC_LEFT, KC_UP, KC_RGHT, _______, KC_F12,
        SSNP_HOR, _______, KC_4, KC_5, KC_6, S(KC_SCLN), KC_PGUP, KC_BTN1, KC_DOWN, KC_BTN2, KC_BTN3, _______,
        _______, _______, KC_1, KC_2, KC_3, S(KC_MINS), S(KC_8), S(KC_9), KC_PGDN, _______, _______, _______, _______, _______, _______,
        _______, KC_0, KC_DOT, _______, _______, _______, KC_DEL, _______, _______, _______, _______, _______, _______
    ),

    [3] = LAYOUT_universal(
        RGB_TOG, AML_TO, AML_I50, AML_D50, _______, _______, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_MOD,
        RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, RGB_M_X, RGB_M_G, RGB_M_T, RGB_M_TW, _______, _______, RGB_RMOD,
        RGB_HUD, RGB_SAD, RGB_VAD, _______, _______, CPI_D1K, CPI_D100, CPI_I100, CPI_I1K, KBC_SAVE, KBC_RST,
        _______, _______, SCRL_DVD, SCRL_DVI, SCRL_MO, SCRL_TO, EE_CLR, EE_CLR, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______, _______, QK_BOOT,
        _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______, _______, KC_BSPC, _______, _______, _______, _______, QK_BOOT
    ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    /*
     * Keyball61では設定・スクロールレイヤーが3
     */
    keyball_set_scroll_mode(get_highest_layer(state) == 3);

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    keyball_keep_auto_mouse_layer_if_needed(state);
#endif

    return state;
}

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef COMBO_ENABLE

/*
 * Keyball44 nowの25コンボをKeyball61へ移植
 */
#define CURRENT_COMBO_COUNT 25

#if COMBO_COUNT != CURRENT_COMBO_COUNT
#    error "Set COMBO_COUNT to 25 in config.h"
#endif

enum combos {
    QL_ESC,
    LU_UP,
    XC_DOWN,
    IA_LEFT,
    AO_RIGHT,

    CV_CTRL_BSPC,
    DM_CTRL_DEL,
    MJ_DEL,

    WR_BTN4,
    RY_BTN5,

    TS_ESC,
    SH_ZKHK,
    EI_F10,
    ZX_F7,
    XV_HOME,
    DJ_END,
    IO_WIN_H,

    BTN1_BTN2_TO_BTN3,

    YP_ESC,
    JB_F4,

    NUM_46_EQUAL,
    NUM_78_MULTIPLY,
    NUM_89_DIVIDE,
    NUM_45_PLUS,
    NUM_56_MINUS,
};

/*
 * 通常レイヤーのコンボ
 */
const uint16_t PROGMEM my_ql[] = {KC_Q, KC_L, COMBO_END};
const uint16_t PROGMEM my_lu[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM my_xc[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM my_ia[] = {I_ALT, KC_A, COMBO_END};
const uint16_t PROGMEM my_ao[] = {KC_A, KC_O, COMBO_END};

const uint16_t PROGMEM my_cv[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM my_dm[] = {KC_D, KC_M, COMBO_END};
const uint16_t PROGMEM my_mj[] = {KC_M, KC_J, COMBO_END};

const uint16_t PROGMEM my_wr[] = {KC_W, KC_R, COMBO_END};
const uint16_t PROGMEM my_ry[] = {KC_R, KC_Y, COMBO_END};

const uint16_t PROGMEM my_ts[] = {KC_T, S_ALT, COMBO_END};
const uint16_t PROGMEM my_sh[] = {S_ALT, H_CTL, COMBO_END};
const uint16_t PROGMEM my_ei[] = {E_CTL, I_ALT, COMBO_END};
const uint16_t PROGMEM my_zx[] = {Z_GUI, KC_X, COMBO_END};
const uint16_t PROGMEM my_xv[] = {KC_X, KC_V, COMBO_END};
const uint16_t PROGMEM my_dj[] = {KC_D, KC_J, COMBO_END};
const uint16_t PROGMEM my_io[] = {I_ALT, KC_O, COMBO_END};

/*
 * Remapレイヤー1の左クリック＋右クリック
 */
const uint16_t PROGMEM btn3_combo[] = {KC_BTN1, KC_BTN2, COMBO_END};

/*
 * Keyball44で追加したコンボ
 */
const uint16_t PROGMEM my_yp[] = {KC_Y, KC_P, COMBO_END};

/*
 * BはMod-Tapなので、KC_BではなくB_GUIを指定
 */
const uint16_t PROGMEM my_jb[] = {KC_J, B_GUI, COMBO_END};

/*
 * Remapレイヤー3のNum 4～Num 9はテンキーキーコード。
 */
const uint16_t PROGMEM my_num_46[] = {KC_P4, KC_P6, COMBO_END};
const uint16_t PROGMEM my_num_78[] = {KC_P7, KC_P8, COMBO_END};
const uint16_t PROGMEM my_num_89[] = {KC_P8, KC_P9, COMBO_END};
const uint16_t PROGMEM my_num_45[] = {KC_P4, KC_P5, COMBO_END};
const uint16_t PROGMEM my_num_56[] = {KC_P5, KC_P6, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [QL_ESC] = COMBO(my_ql, KC_ESC),
    [LU_UP] = COMBO(my_lu, KC_UP),
    [XC_DOWN] = COMBO(my_xc, KC_DOWN),
    [IA_LEFT] = COMBO(my_ia, KC_LEFT),
    [AO_RIGHT] = COMBO(my_ao, KC_RIGHT),

    [CV_CTRL_BSPC] = COMBO(my_cv, LCTL(KC_BSPC)),
    [DM_CTRL_DEL] = COMBO(my_dm, LCTL(KC_DEL)),
    [MJ_DEL] = COMBO(my_mj, KC_DEL),

    [WR_BTN4] = COMBO(my_wr, KC_BTN4),
    [RY_BTN5] = COMBO(my_ry, KC_BTN5),

    [TS_ESC] = COMBO(my_ts, KC_ESC),
    [SH_ZKHK] = COMBO(my_sh, KC_GRV),
    [EI_F10] = COMBO(my_ei, KC_F10),
    [ZX_F7] = COMBO(my_zx, KC_F7),
    [XV_HOME] = COMBO(my_xv, KC_HOME),
    [DJ_END] = COMBO(my_dj, KC_END),
    [IO_WIN_H] = COMBO(my_io, LGUI(KC_H)),

    [BTN1_BTN2_TO_BTN3] = COMBO(btn3_combo, KC_BTN3),

    [YP_ESC] = COMBO(my_yp, KC_ESC),
    [JB_F4] = COMBO(my_jb, KC_F4),

    /*
     * Windows側が日本語配列の環境でroBa・Keyball44と
     * 同じ記号を出すキーコード
     */
    [NUM_46_EQUAL] = COMBO(my_num_46, S(KC_MINS)),
    [NUM_78_MULTIPLY] = COMBO(my_num_78, S(KC_QUOT)),
    [NUM_89_DIVIDE] = COMBO(my_num_89, KC_SLSH),
    [NUM_45_PLUS] = COMBO(my_num_45, S(KC_SCLN)),
    [NUM_56_MINUS] = COMBO(my_num_56, KC_MINS),
};

/*
 * Remap上の実際のレイヤー構成:
 *   1 = マウス
 *   3 = テンキー・記号
 */
bool combo_should_trigger(
    uint16_t combo_index,
    combo_t *combo,
    uint16_t keycode,
    keyrecord_t *record
) {
    uint8_t current_layer =
        get_highest_layer(layer_state | default_layer_state);

    switch (combo_index) {
        case BTN1_BTN2_TO_BTN3:
            return current_layer == 1;

        case NUM_46_EQUAL:
        case NUM_78_MULTIPLY:
        case NUM_89_DIVIDE:
        case NUM_45_PLUS:
        case NUM_56_MINUS:
            return current_layer == 3;

        default:
            return true;
    }
}

/*
 * 通常判定80ms
 * 押しにくいComboは140ms
 */
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case QL_ESC:
        case CV_CTRL_BSPC:
        case DM_CTRL_DEL:
        case MJ_DEL:
        case TS_ESC:
        case SH_ZKHK:
        case EI_F10:
        case YP_ESC:
        case JB_F4:
        case NUM_46_EQUAL:
        case NUM_78_MULTIPLY:
        case NUM_89_DIVIDE:
        case NUM_45_PLUS:
        case NUM_56_MINUS:
            return 140;

        default:
            return COMBO_TERM;
    }
}

#endif
