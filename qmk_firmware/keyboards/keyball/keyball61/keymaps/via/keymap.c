/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

roBa-style adaptation for Keyball61
- Keep Keyball61 top number row / outer extra keys / Keyball-specific layers.
- Make the main 3 typing rows and mouse-button feel match roBa.
- Keep VIA support.
*/
#include QMK_KEYBOARD_H
#include "quantum.h"

/* roBa-style Mod-Tap keys */
#define E_CTL LCTL_T(KC_E)
#define I_ALT LALT_T(KC_I)
#define Z_GUI LGUI_T(KC_Z)
#define H_CTL RCTL_T(KC_H)
#define B_GUI LGUI_T(KC_B)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*
     * Layer 0: BASE
     *
     * Top number row and Keyball61-only outer keys are retained.
     *
     * Main typing area follows current roBa:
     *   Q L U , .        F W R Y P
     *   E I A O -   PSCR F15 K   T N S H
     *   Z X C V [    F2  TG(mouse) G D M J B
     */
    [0] = LAYOUT_universal(
        KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_DEL ,
        KC_Q   , KC_L   , KC_U   , KC_COMM, KC_DOT , KC_F   , KC_W   , KC_R   , KC_Y   , KC_P   , KC_INT3,
        E_CTL  , I_ALT  , KC_A   , KC_O   , KC_MINS, KC_PSCR, KC_F15 , KC_K   , KC_T   , KC_N   , KC_S   , H_CTL,
        MO(1)  , Z_GUI  , KC_X   , KC_C   , KC_V   , KC_LBRC, KC_F2  , TG(1)  , KC_G   , KC_D   , KC_M   , KC_J   , B_GUI  , KC_RSFT,
        _______, KC_LCTL, KC_LALT, KC_LGUI, LT(1,KC_LNG2), LT(2,KC_SPC), LT(3,KC_LNG1), KC_BSPC, LT(2,KC_ENT), LT(1,KC_LNG2), KC_RGUI, _______, KC_RALT, KC_PSCR
    ),

    /*
     * Layer 1: AUTO_MOUSE / manual mouse layer
     * roBa style:
     *   T = MB1
     *   N = MB2
     *   T+N = MB3 (combo)
     *   W+R = MB4 (combo)
     *   R+Y = MB5 (combo)
     *
     * Everything else is transparent so normal letters remain available.
     */
    [1] = LAYOUT_universal(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    /*
     * Layer 2: Keep the existing Keyball61 mouse/number/F-key layer.
     * This preserves familiar Keyball61-specific functions.
     */
    [2] = LAYOUT_universal(
        SSNP_FRE, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, SSNP_VRT,
        _______, KC_7, KC_8, KC_9, _______, _______, KC_LEFT, KC_UP, KC_RGHT, _______, KC_F12,
        SSNP_HOR, _______, KC_4, KC_5, KC_6, S(KC_SCLN), KC_PGUP, KC_BTN1, KC_DOWN, KC_BTN2, KC_BTN3, _______,
        _______, _______, KC_1, KC_2, KC_3, S(KC_MINS), S(KC_8), S(KC_9), KC_PGDN, _______, _______, _______, _______, _______,
        _______, KC_0, KC_DOT, _______, _______, _______, KC_DEL, _______, _______, _______, _______, _______, _______, _______
    ),

    /*
     * Layer 3: Keep existing Keyball61 configuration / scroll layer.
     */
    [3] = LAYOUT_universal(
        RGB_TOG, AML_TO, AML_I50, AML_D50, _______, _______, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_MOD,
        RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, RGB_M_X, RGB_M_G, RGB_M_T, RGB_M_TW, _______, _______, RGB_RMOD,
        RGB_HUD, RGB_SAD, RGB_VAD, _______, _______, CPI_D1K, CPI_D100, CPI_I100, CPI_I1K, KBC_SAVE, KBC_RST, _______,
        _______, _______, SCRL_DVD, SCRL_DVI, SCRL_MO, SCRL_TO, EE_CLR, EE_CLR, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______, _______, QK_BOOT,
        _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______, _______, KC_BSPC, _______, _______, _______, _______, _______, QK_BOOT
    ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    /* Preserve existing Keyball61 scroll layer behavior on layer 3. */
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

#define CURRENT_COMBO_COUNT 29
#if COMBO_COUNT != CURRENT_COMBO_COUNT
#    error "Set COMBO_COUNT to 29 in config.h"
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

    SH_ZKHK,
    EI_F10,
    ZX_F7,
    XV_HOME,
    DJ_END,
    IO_WIN_H,
    YP_ESC,
    JB_F4,

    BTN1_BTN2_TO_BTN3,
    WR_BTN4,
    RY_BTN5,

    BASE_TS_EQUAL,
    BASE_WR_MULTIPLY,
    BASE_RY_DIVIDE,
    BASE_TN_PLUS,
    BASE_NS_MINUS,

    NUM_46_EQUAL,
    NUM_78_MULTIPLY,
    NUM_89_DIVIDE,
    NUM_45_PLUS,
    NUM_56_MINUS,
};

/* roBa general combos */
const uint16_t PROGMEM my_ql[] = {KC_Q, KC_L, COMBO_END};
const uint16_t PROGMEM my_lu[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM my_xc[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM my_ia[] = {I_ALT, KC_A, COMBO_END};
const uint16_t PROGMEM my_ao[] = {KC_A, KC_O, COMBO_END};

const uint16_t PROGMEM my_cv[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM my_dm[] = {KC_D, KC_M, COMBO_END};
const uint16_t PROGMEM my_mj[] = {KC_M, KC_J, COMBO_END};

const uint16_t PROGMEM my_sh[] = {KC_S, H_CTL, COMBO_END};
const uint16_t PROGMEM my_ei[] = {E_CTL, I_ALT, COMBO_END};
const uint16_t PROGMEM my_zx[] = {Z_GUI, KC_X, COMBO_END};
const uint16_t PROGMEM my_xv[] = {KC_X, KC_V, COMBO_END};
const uint16_t PROGMEM my_dj[] = {KC_D, KC_J, COMBO_END};
const uint16_t PROGMEM my_io[] = {I_ALT, KC_O, COMBO_END};
const uint16_t PROGMEM my_yp[] = {KC_Y, KC_P, COMBO_END};
const uint16_t PROGMEM my_jb[] = {KC_J, B_GUI, COMBO_END};

/* Mouse-layer combos */
const uint16_t PROGMEM btn3_combo[] = {KC_BTN1, KC_BTN2, COMBO_END};
const uint16_t PROGMEM mouse_wr[] = {KC_W, KC_R, COMBO_END};
const uint16_t PROGMEM mouse_ry[] = {KC_R, KC_Y, COMBO_END};

/* BASE operator combos matching roBa */
const uint16_t PROGMEM base_ts[] = {KC_T, KC_S, COMBO_END};
const uint16_t PROGMEM base_wr[] = {KC_W, KC_R, COMBO_END};
const uint16_t PROGMEM base_ry[] = {KC_R, KC_Y, COMBO_END};
const uint16_t PROGMEM base_tn[] = {KC_T, KC_N, COMBO_END};
const uint16_t PROGMEM base_ns[] = {KC_N, KC_S, COMBO_END};

/*
 * Existing Keyball61 number layer uses normal number keycodes.
 * Keep its 4/6, 7/8, 8/9, 4/5, 5/6 operator combos.
 */
const uint16_t PROGMEM my_num_46[] = {KC_4, KC_6, COMBO_END};
const uint16_t PROGMEM my_num_78[] = {KC_7, KC_8, COMBO_END};
const uint16_t PROGMEM my_num_89[] = {KC_8, KC_9, COMBO_END};
const uint16_t PROGMEM my_num_45[] = {KC_4, KC_5, COMBO_END};
const uint16_t PROGMEM my_num_56[] = {KC_5, KC_6, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [QL_ESC] = COMBO(my_ql, KC_ESC),
    [LU_UP] = COMBO(my_lu, KC_UP),
    [XC_DOWN] = COMBO(my_xc, KC_DOWN),
    [IA_LEFT] = COMBO(my_ia, KC_LEFT),
    [AO_RIGHT] = COMBO(my_ao, KC_RIGHT),

    [CV_CTRL_BSPC] = COMBO(my_cv, LCTL(KC_BSPC)),
    [DM_CTRL_DEL] = COMBO(my_dm, LCTL(KC_DEL)),
    [MJ_DEL] = COMBO(my_mj, KC_DEL),

    [SH_ZKHK] = COMBO(my_sh, KC_GRV),
    [EI_F10] = COMBO(my_ei, KC_F10),
    [ZX_F7] = COMBO(my_zx, KC_F7),
    [XV_HOME] = COMBO(my_xv, KC_HOME),
    [DJ_END] = COMBO(my_dj, KC_END),
    [IO_WIN_H] = COMBO(my_io, LGUI(KC_H)),
    [YP_ESC] = COMBO(my_yp, KC_ESC),
    [JB_F4] = COMBO(my_jb, KC_F4),

    [BTN1_BTN2_TO_BTN3] = COMBO(btn3_combo, KC_BTN3),
    [WR_BTN4] = COMBO(mouse_wr, KC_BTN4),
    [RY_BTN5] = COMBO(mouse_ry, KC_BTN5),

    /* Windows Japanese-layout compatible outputs, same approach as existing keymap. */
    [BASE_TS_EQUAL] = COMBO(base_ts, S(KC_MINS)),
    [BASE_WR_MULTIPLY] = COMBO(base_wr, S(KC_QUOT)),
    [BASE_RY_DIVIDE] = COMBO(base_ry, KC_PSLS),
    [BASE_TN_PLUS] = COMBO(base_tn, S(KC_SCLN)),
    [BASE_NS_MINUS] = COMBO(base_ns, KC_MINS),

    [NUM_46_EQUAL] = COMBO(my_num_46, S(KC_MINS)),
    [NUM_78_MULTIPLY] = COMBO(my_num_78, S(KC_QUOT)),
    [NUM_89_DIVIDE] = COMBO(my_num_89, KC_PSLS),
    [NUM_45_PLUS] = COMBO(my_num_45, S(KC_SCLN)),
    [NUM_56_MINUS] = COMBO(my_num_56, KC_MINS),
};

bool combo_should_trigger(
    uint16_t combo_index,
    combo_t *combo,
    uint16_t keycode,
    keyrecord_t *record
) {
    uint8_t current_layer =
        get_highest_layer(layer_state | default_layer_state);

    switch (combo_index) {
        /* Mouse-only */
        case BTN1_BTN2_TO_BTN3:
        case WR_BTN4:
        case RY_BTN5:
            return current_layer == 1;

        /* BASE-only operator combos */
        case BASE_TS_EQUAL:
        case BASE_WR_MULTIPLY:
        case BASE_RY_DIVIDE:
        case BASE_TN_PLUS:
        case BASE_NS_MINUS:
            return current_layer == 0;

        /* Existing number layer operator combos */
        case NUM_46_EQUAL:
        case NUM_78_MULTIPLY:
        case NUM_89_DIVIDE:
        case NUM_45_PLUS:
        case NUM_56_MINUS:
            return current_layer == 2;

        default:
            return true;
    }
}

/* roBa: MB3 is fast (40ms); all other combos use the global 80ms. */
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    if (index == BTN1_BTN2_TO_BTN3) {
        return 40;
    }
    return COMBO_TERM;
}
#endif
