/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

/*
 * レイヤー0の長押しキー定義
 *
 * E : 単打 E / 長押し 左Ctrl
 * I : 単打 I / 長押し 左Alt
 * Z : 単打 Z / 長押し 左Win-Cmd
 *
 * S : 単打 S / 長押し 右Alt
 * H : 単打 H / 長押し 右Ctrl
 * B : 単打 B / 長押し 左Win-Cmd
 */
#define E_CTL LCTL_T(KC_E)
#define I_ALT LALT_T(KC_I)
#define Z_GUI LGUI_T(KC_Z)

#define S_ALT RALT_T(KC_S)
#define H_CTL RCTL_T(KC_H)
#define B_GUI LGUI_T(KC_B)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , E_CTL    , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , I_ALT    , KC_O     , KC_P     , KC_DEL   ,
    KC_TAB   , KC_A     , S_ALT    , KC_D     , KC_F     , KC_G     ,                                        H_CTL    , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    KC_LSFT  , Z_GUI    , KC_X     , KC_C     , KC_V     , B_GUI    ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    SSNP_FRE ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   KC_DEL   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  , _______  ,                                        RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , SCRL_DVI ,                                        RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , SCRL_DVD ,                                        CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , _______  , KBC_SAVE ,
                  QK_BOOT  , KBC_RST  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , KBC_RST  , QK_BOOT
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
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

#define CURRENT_COMBO_COUNT 20

#if COMBO_COUNT != CURRENT_COMBO_COUNT
#    error "Set COMBO_COUNT to 20 in config.h"
#endif

enum combos {

QL_ESC,

LU_UP,
XC_DOWN,
IA_LEFT,
AO_RIGHT,

TN_CTRL_BSPC,

CV_CTRL_DEL,
DM_CTRL_BSPC,
MJ_DEL,

WR_BTN4,
RY_BTN5,
TS_ESC,

SH_CTRL_BSPC,
EI_F10,
ZX_F7,
XV_HOME,
DJ_END,
JB_ZKHK,
IO_WIN_H,

BTN1_BTN2_TO_BTN3,

};

const uint16_t PROGMEM my_ql[] = {KC_Q, KC_L, COMBO_END};

const uint16_t PROGMEM my_lu[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM my_xc[] = {KC_X, KC_C, COMBO_END};

const uint16_t PROGMEM my_ia[] = {I_ALT, KC_A, COMBO_END};
const uint16_t PROGMEM my_ao[] = {KC_A, KC_O, COMBO_END};

const uint16_t PROGMEM my_tn[] = {KC_T, KC_N, COMBO_END};

/*
 * C + V → 右単語削除 Ctrl + Delete
 * D + M → 左単語削除 Ctrl + Backspace
 * M + J → Delete
 */
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

const uint16_t PROGMEM my_jb[] = {KC_J, B_GUI, COMBO_END};
const uint16_t PROGMEM my_io[] = {I_ALT, KC_O, COMBO_END};

const uint16_t PROGMEM btn3_combo[] = {KC_BTN1, KC_BTN2, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {

[QL_ESC] = COMBO(my_ql, KC_ESC),

[LU_UP] = COMBO(my_lu, KC_UP),
[XC_DOWN] = COMBO(my_xc, KC_DOWN),

[IA_LEFT] = COMBO(my_ia, KC_LEFT),
[AO_RIGHT] = COMBO(my_ao, KC_RIGHT),

[TN_CTRL_BSPC] = COMBO(my_tn, LCTL(KC_BSPC)),

[CV_CTRL_DEL] = COMBO(my_cv, LCTL(KC_DEL)),
[DM_CTRL_BSPC] = COMBO(my_dm, LCTL(KC_BSPC)),
[MJ_DEL] = COMBO(my_mj, KC_DEL),

[WR_BTN4] = COMBO(my_wr, KC_BTN4),
[RY_BTN5] = COMBO(my_ry, KC_BTN5),

[TS_ESC] = COMBO(my_ts, KC_ESC),

[SH_CTRL_BSPC] = COMBO(my_sh, LCTL(KC_BSPC)),

[EI_F10] = COMBO(my_ei, KC_F10),
[ZX_F7] = COMBO(my_zx, KC_F7),

[XV_HOME] = COMBO(my_xv, KC_HOME),
[DJ_END] = COMBO(my_dj, KC_END),

[JB_ZKHK] = COMBO(my_jb, KC_GRV),
[IO_WIN_H] = COMBO(my_io, LGUI(KC_H)),

[BTN1_BTN2_TO_BTN3] = COMBO(btn3_combo, KC_BTN3),

};

/*
 * 効いていなかったComboは判定時間を長めにする
 *
 * 通常 : 80ms
 * 下記 : 140ms
 */
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case QL_ESC:
        case TN_CTRL_BSPC:
        case CV_CTRL_DEL:
        case DM_CTRL_BSPC:
        case MJ_DEL:
        case TS_ESC:
        case SH_CTRL_BSPC:
        case EI_F10:
            return 140;

        default:
            return COMBO_TERM;
    }
}

#endif
