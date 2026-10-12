/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

roBa-style adaptation for Keyball61
REMAP-preserving combo fix
*/
#include QMK_KEYBOARD_H
#include "quantum.h"

/*
 * Match the REMAP Layer 0 shown by the user:
 * E = Ctrl-tap, I = Alt-tap,
 * S = Alt-tap, H = Ctrl-tap,
 * Z/B = Win/Cmd-tap.
 */
#define E_CTL LCTL_T(KC_E)
#define I_ALT LALT_T(KC_I)
#define S_ALT RALT_T(KC_S)
#define H_CTL RCTL_T(KC_H)
#define Z_GUI LGUI_T(KC_Z)
#define B_GUI RGUI_T(KC_B)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_universal(
        KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_DEL ,
        KC_Q   , KC_L   , KC_U   , KC_COMM, KC_DOT , KC_F   , KC_W   , KC_R   , KC_Y   , KC_P   , KC_INT3,
        E_CTL  , I_ALT  , KC_A   , KC_O   , KC_MINS, KC_PSCR, KC_F15 , KC_K   , KC_T   , KC_N   , S_ALT  , H_CTL,
        MO(1)  , Z_GUI  , KC_X   , KC_C   , KC_V   , KC_LBRC, KC_F2  , TG(1)  , KC_G   , KC_D   , KC_M   , KC_J   , B_GUI  , KC_RSFT,
        _______, KC_LCTL, KC_LALT, KC_LGUI, LT(1,KC_LNG2), LT(2,KC_SPC), LT(3,KC_LNG1), KC_BSPC, LT(2,KC_ENT), LT(1,KC_LNG2), KC_RGUI, _______, KC_RALT, KC_PSCR
    ),
    [1] = LAYOUT_universal(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
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

void keyboard_post_init_user(void) {
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    set_auto_mouse_layer(1);
    set_auto_mouse_enable(true);
#endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
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

    SH_ZKHK,
    EI_F10,
    ZX_F7,
    XV_HOME,
    DJ_END,
    IO_WIN_H,
    YP_ESC,
    TN_ACTION,
    WR_ACTION,
    RY_ACTION,

    TS_EQUAL,
    NS_MINUS,

    NUM_46_EQUAL,
    NUM_78_MULTIPLY,
    NUM_89_DIVIDE,
    NUM_45_PLUS,
    NUM_56_MINUS,
};

uint8_t combo_ref_from_layer(uint8_t layer) {
    return layer == 1 ? 0 : layer;
}

const uint16_t PROGMEM my_ql[] = {KC_Q, KC_L, COMBO_END};
const uint16_t PROGMEM my_lu[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM my_xc[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM my_ia[] = {I_ALT, KC_A, COMBO_END};
const uint16_t PROGMEM my_ao[] = {KC_A, KC_O, COMBO_END};
const uint16_t PROGMEM my_cv[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM my_dm[] = {KC_D, KC_M, COMBO_END};
const uint16_t PROGMEM my_mj[] = {KC_M, KC_J, COMBO_END};
const uint16_t PROGMEM my_sh[] = {S_ALT, H_CTL, COMBO_END};
const uint16_t PROGMEM my_ei[] = {E_CTL, I_ALT, COMBO_END};
const uint16_t PROGMEM my_zx[] = {Z_GUI, KC_X, COMBO_END};
const uint16_t PROGMEM my_xv[] = {KC_X, KC_V, COMBO_END};
const uint16_t PROGMEM my_dj[] = {KC_D, KC_J, COMBO_END};
const uint16_t PROGMEM my_io[] = {I_ALT, KC_O, COMBO_END};
const uint16_t PROGMEM my_yp[] = {KC_Y, KC_P, COMBO_END};

/* B is tap=B / hold=Right GUI. Keep the combo on the exact Mod-Tap keycode. */
const uint16_t PROGMEM pair_tn[] = {KC_T, KC_N, COMBO_END};
const uint16_t PROGMEM pair_wr[] = {KC_W, KC_R, COMBO_END};
const uint16_t PROGMEM pair_ry[] = {KC_R, KC_Y, COMBO_END};
const uint16_t PROGMEM pair_ts[] = {KC_T, S_ALT, COMBO_END};
const uint16_t PROGMEM pair_ns[] = {KC_N, S_ALT, COMBO_END};

const uint16_t PROGMEM num_46[] = {KC_P4, KC_P6, COMBO_END};
const uint16_t PROGMEM num_78[] = {KC_P7, KC_P8, COMBO_END};
const uint16_t PROGMEM num_89[] = {KC_P8, KC_P9, COMBO_END};
const uint16_t PROGMEM num_45[] = {KC_P4, KC_P5, COMBO_END};
const uint16_t PROGMEM num_56[] = {KC_P5, KC_P6, COMBO_END};

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
    [TN_ACTION] = COMBO_ACTION(pair_tn),
    [WR_ACTION] = COMBO_ACTION(pair_wr),
    [RY_ACTION] = COMBO_ACTION(pair_ry),
    [TS_EQUAL] = COMBO(pair_ts, S(KC_MINS)),
    [NS_MINUS] = COMBO(pair_ns, KC_MINS),

    [NUM_46_EQUAL] = COMBO(num_46, S(KC_MINS)),
    [NUM_78_MULTIPLY] = COMBO(num_78, S(KC_QUOT)),
    [NUM_89_DIVIDE] = COMBO(num_89, KC_PSLS),
    [NUM_45_PLUS] = COMBO(num_45, S(KC_SCLN)),
    [NUM_56_MINUS] = COMBO(num_56, KC_MINS),
};

static bool tn_mb3_active = false;

void process_combo_event(uint16_t combo_index, bool pressed) {
    uint8_t current_layer =
        get_highest_layer(layer_state | default_layer_state);

    /*
     * roBa-style pan:
     * On auto-mouse layer, hold T+N to hold MB3.
     * Release T or N to release MB3.
     * The flag guarantees MB3 is released even if layer 1 drops meanwhile.
     */
    if (combo_index == TN_ACTION) {
        if (pressed) {
            if (current_layer == 1) {
                register_code(KC_BTN3);
                tn_mb3_active = true;
            } else {
                tap_code(KC_PPLS);
            }
        } else if (tn_mb3_active) {
            unregister_code(KC_BTN3);
            tn_mb3_active = false;
        }
        return;
    }

    if (!pressed) {
        return;
    }

    switch (combo_index) {
        case WR_ACTION:
            if (current_layer == 1) {
                tap_code(KC_BTN4);
            } else {
                tap_code(KC_PAST);
            }
            break;

        case RY_ACTION:
            if (current_layer == 1) {
                tap_code(KC_BTN5);
            } else {
                tap_code(KC_PSLS);
            }
            break;
    }
}

bool combo_should_trigger(
    uint16_t combo_index,
    combo_t *combo,
    uint16_t keycode,
    keyrecord_t *record
) {
    uint8_t current_layer =
        get_highest_layer(layer_state | default_layer_state);

    switch (combo_index) {
        case NUM_46_EQUAL:
        case NUM_78_MULTIPLY:
        case NUM_89_DIVIDE:
        case NUM_45_PLUS:
        case NUM_56_MINUS:
            return current_layer == 3;

        default:
            return current_layer == 0 || current_layer == 1;
    }
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    if (index == TN_ACTION) {
        return 40;
    }

    if (index == SH_ZKHK || index == TS_EQUAL || index == NS_MINUS) {
        return 120;
    }

    return COMBO_TERM;
}
#endif
