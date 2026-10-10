
#include QMK_KEYBOARD_H
#include "quantum.h"

#define E_CTL LCTL_T(KC_E)
#define I_ALT LALT_T(KC_I)
#define Z_GUI LGUI_T(KC_Z)
#define H_CTL RCTL_T(KC_H)
#define B_GUI LGUI_T(KC_B)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_universal(
KC_ESC,KC_1,KC_2,KC_3,KC_4,KC_5,                                  KC_6,KC_7,KC_8,KC_9,KC_0,KC_MINS,
KC_DEL,KC_Q,KC_L,KC_U,KC_COMM,KC_DOT,                              KC_F,KC_W,KC_R,KC_Y,KC_P,KC_INT3,
KC_TAB,E_CTL,I_ALT,KC_A,KC_O,KC_MINS,                              KC_F15,KC_K,KC_T,KC_N,KC_S,H_CTL,
MO(1),Z_GUI,KC_X,KC_C,KC_V,KC_LBRC,KC_F2,              TG(1),KC_G,KC_D,KC_M,KC_J,B_GUI,KC_RSFT,
_______,KC_LCTL,KC_LALT,KC_LGUI,LT(1,KC_LNG2),LT(2,KC_SPC),LT(3,KC_LNG1), KC_BSPC,LT(2,KC_ENT),LT(1,KC_LNG2),KC_RGUI,_______,KC_RALT,KC_PSCR
),
[1] = LAYOUT_universal(
_______,_______,_______,_______,_______,_______,                    _______,_______,_______,_______,_______,_______,
_______,_______,_______,_______,_______,_______,                    _______,_______,_______,_______,_______,_______,
_______,_______,_______,_______,_______,_______,                    _______,_______,KC_BTN1,KC_BTN2,_______,_______,
_______,_______,_______,_______,_______,_______,_______,            _______,_______,_______,_______,_______,_______,_______,
_______,_______,_______,_______,_______,_______,_______,            _______,_______,_______,_______,_______,_______,_______
),
[2] = LAYOUT_universal(
SSNP_FRE,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,                           KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,
SSNP_VRT,_______,KC_7,KC_8,KC_9,_______,                           _______,KC_LEFT,KC_UP,KC_RGHT,_______,KC_F12,
SSNP_HOR,_______,KC_4,KC_5,KC_6,S(KC_SCLN),                       KC_PGUP,KC_BTN1,KC_DOWN,KC_BTN2,KC_BTN3,_______,
_______,_______,KC_1,KC_2,KC_3,S(KC_MINS),S(KC_8),                S(KC_9),KC_PGDN,_______,_______,_______,_______,_______,
_______,_______,KC_0,KC_DOT,_______,_______,_______,               KC_DEL,_______,_______,_______,_______,_______,_______
),
[3] = LAYOUT_universal(
RGB_TOG,AML_TO,AML_I50,AML_D50,_______,_______,                    RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,
RGB_MOD,RGB_HUI,RGB_SAI,RGB_VAI,_______,_______,                    RGB_M_X,RGB_M_G,RGB_M_T,RGB_M_TW,_______,_______,
RGB_RMOD,RGB_HUD,RGB_SAD,RGB_VAD,_______,_______,                   CPI_D1K,CPI_D100,CPI_I100,CPI_I1K,KBC_SAVE,KBC_RST,
_______,_______,SCRL_DVD,SCRL_DVI,SCRL_MO,SCRL_TO,EE_CLR,          EE_CLR,KC_HOME,KC_PGDN,KC_PGUP,KC_END,_______,_______,
QK_BOOT,_______,KC_LEFT,KC_DOWN,KC_UP,KC_RGHT,_______,             _______,KC_BSPC,_______,_______,_______,_______,QK_BOOT
)
};
// clang-format on

void keyboard_post_init_user(void) {
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    set_auto_mouse_layer(1);
    set_auto_mouse_enable(true);
    set_auto_mouse_timeout(30000);
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
# include "lib/oledkit/oledkit.h"
void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef COMBO_ENABLE
enum combo_events {
C_QL_ESC,C_LU_UP,C_XC_DOWN,C_IA_LEFT,C_AO_RIGHT,
C_CV_CTRL_BSPC,C_DM_CTRL_DEL,C_MJ_DEL,C_SH_HANKAKU,
C_EI_F10,C_ZX_F7,C_XV_HOME,C_DJ_END,C_IO_WIN_H,C_YP_ESC,C_JB_F4,
C_MOUSE_TN_MB3,C_MOUSE_WR_MB4,C_MOUSE_RY_MB5,
C_TS_EQUAL,C_WR_MULTIPLY,C_RY_DIVIDE,C_TN_PLUS,C_NS_MINUS
};

uint8_t combo_ref_from_layer(uint8_t layer) { return layer == 1 ? 0 : layer; }

const uint16_t PROGMEM ql[]={KC_Q,KC_L,COMBO_END};
const uint16_t PROGMEM lu[]={KC_L,KC_U,COMBO_END};
const uint16_t PROGMEM xc[]={KC_X,KC_C,COMBO_END};
const uint16_t PROGMEM ia[]={I_ALT,KC_A,COMBO_END};
const uint16_t PROGMEM ao[]={KC_A,KC_O,COMBO_END};
const uint16_t PROGMEM cv[]={KC_C,KC_V,COMBO_END};
const uint16_t PROGMEM dm[]={KC_D,KC_M,COMBO_END};
const uint16_t PROGMEM mj[]={KC_M,KC_J,COMBO_END};
const uint16_t PROGMEM sh[]={KC_S,H_CTL,COMBO_END};
const uint16_t PROGMEM ei[]={E_CTL,I_ALT,COMBO_END};
const uint16_t PROGMEM zx[]={Z_GUI,KC_X,COMBO_END};
const uint16_t PROGMEM xv[]={KC_X,KC_V,COMBO_END};
const uint16_t PROGMEM dj[]={KC_D,KC_J,COMBO_END};
const uint16_t PROGMEM io[]={I_ALT,KC_O,COMBO_END};
const uint16_t PROGMEM yp[]={KC_Y,KC_P,COMBO_END};
const uint16_t PROGMEM jb[]={KC_J,B_GUI,COMBO_END};
const uint16_t PROGMEM tn[]={KC_T,KC_N,COMBO_END};
const uint16_t PROGMEM wr[]={KC_W,KC_R,COMBO_END};
const uint16_t PROGMEM ry[]={KC_R,KC_Y,COMBO_END};
const uint16_t PROGMEM ts[]={KC_T,KC_S,COMBO_END};
const uint16_t PROGMEM ns[]={KC_N,KC_S,COMBO_END};

combo_t key_combos[] = {
[C_QL_ESC]=COMBO(ql,KC_ESC),
[C_LU_UP]=COMBO(lu,KC_UP),
[C_XC_DOWN]=COMBO(xc,KC_DOWN),
[C_IA_LEFT]=COMBO(ia,KC_LEFT),
[C_AO_RIGHT]=COMBO(ao,KC_RIGHT),
[C_CV_CTRL_BSPC]=COMBO(cv,LCTL(KC_BSPC)),
[C_DM_CTRL_DEL]=COMBO(dm,LCTL(KC_DEL)),
[C_MJ_DEL]=COMBO(mj,KC_DEL),
[C_SH_HANKAKU]=COMBO(sh,KC_GRV),
[C_EI_F10]=COMBO(ei,KC_F10),
[C_ZX_F7]=COMBO(zx,KC_F7),
[C_XV_HOME]=COMBO(xv,KC_HOME),
[C_DJ_END]=COMBO(dj,KC_END),
[C_IO_WIN_H]=COMBO(io,LGUI(KC_H)),
[C_YP_ESC]=COMBO(yp,KC_ESC),
[C_JB_F4]=COMBO(jb,KC_F4),
[C_MOUSE_TN_MB3]=COMBO(tn,KC_BTN3),
[C_MOUSE_WR_MB4]=COMBO(wr,KC_BTN4),
[C_MOUSE_RY_MB5]=COMBO(ry,KC_BTN5),
[C_TS_EQUAL]=COMBO(ts,S(KC_MINS)),
[C_WR_MULTIPLY]=COMBO(wr,KC_PAST),
[C_RY_DIVIDE]=COMBO(ry,KC_PSLS),
[C_TN_PLUS]=COMBO(tn,KC_PPLS),
[C_NS_MINUS]=COMBO(ns,KC_PMNS)
};

bool combo_should_trigger(uint16_t i, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    switch (i) {
        case C_MOUSE_TN_MB3:
        case C_MOUSE_WR_MB4:
        case C_MOUSE_RY_MB5:
            return layer == 1;
        case C_TS_EQUAL:
        case C_NS_MINUS:
            return layer == 0 || layer == 1;
        case C_WR_MULTIPLY:
        case C_RY_DIVIDE:
        case C_TN_PLUS:
            return layer == 0;
        default:
            return layer == 0 || layer == 1;
    }
}

uint16_t get_combo_term(uint16_t i, combo_t *combo) {
    if (i == C_MOUSE_TN_MB3) return 40;
    if (i == C_SH_HANKAKU) return 120;
    return 80;
}
#endif
