/*
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
Licensed under the GNU General Public License v2
*/

#pragma once

#include "quantum.h"

#ifndef KEYBALL_CPI_DEFAULT
#    define KEYBALL_CPI_DEFAULT 500
#endif
#ifndef KEYBALL_SCROLL_DIV_DEFAULT
#    define KEYBALL_SCROLL_DIV_DEFAULT 4
#endif
#ifndef KEYBALL_REPORTMOUSE_INTERVAL
#    define KEYBALL_REPORTMOUSE_INTERVAL 8
#endif
#ifndef KEYBALL_SCROLLBALL_INHIVITOR
#    define KEYBALL_SCROLLBALL_INHIVITOR 50
#endif
#ifndef KEYBALL_SCROLLSNAP_ENABLE
#    define KEYBALL_SCROLLSNAP_ENABLE 2
#endif

#define KEYBALL_TX_GETINFO_INTERVAL 500
#define KEYBALL_TX_GETINFO_MAXTRY 10
#define KEYBALL_TX_GETMOTION_INTERVAL 4
#define KEYBALL_OLED_MAX_PRESSING_KEYCODES 6

enum keyball_keycodes {
    KBC_RST  = QK_KB_0, 
    KBC_SAVE = QK_KB_1, 
    CPI_I100 = QK_KB_2, 
    CPI_D100 = QK_KB_3, 
    CPI_I1K  = QK_KB_4, 
    CPI_D1K  = QK_KB_5, 
    SCRL_TO  = QK_KB_6, 
    SCRL_MO  = QK_KB_7, 
    SCRL_DVI = QK_KB_8, 
    SCRL_DVD = QK_KB_9, 
    SSNP_VRT = QK_KB_13,
    SSNP_HOR = QK_KB_14,
    SSNP_FRE = QK_KB_15,
    AML_TO   = QK_KB_10,
    AML_I50  = QK_KB_11,
    AML_D50  = QK_KB_12,
    KEYBALL_SAFE_RANGE = QK_USER_0,
};

typedef union {
    uint32_t raw;
    struct {
        uint8_t cpi : 7;
        uint8_t sdiv : 3;
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
        uint8_t amle : 1;
        uint16_t amlto : 5;
#endif
#if KEYBALL_SCROLLSNAP_ENABLE == 2
        uint8_t ssnap : 2;
#endif
    };
} keyball_config_t;

typedef struct { uint8_t ballcnt; } keyball_info_t;
typedef struct { int16_t x; int16_t y; } keyball_motion_t;
typedef uint8_t keyball_cpi_t;

typedef enum {
    KEYBALL_SCROLLSNAP_MODE_VERTICAL   = 0,
    KEYBALL_SCROLLSNAP_MODE_HORIZONTAL = 1,
    KEYBALL_SCROLLSNAP_MODE_FREE       = 2,
} keyball_scrollsnap_mode_t;

typedef enum {
    KEYBALL_ADJUST_PENDING   = 0,
    KEYBALL_ADJUST_PRIMARY   = 1,
    KEYBALL_ADJUST_SECONDARY = 2,
} keyball_adjust_t;

typedef struct {
    bool this_have_ball;
    bool that_enable;
    bool that_have_ball;
    keyball_motion_t this_motion;
    keyball_motion_t that_motion;
    uint8_t cpi_value;
    bool    cpi_changed;
    bool     scroll_mode;
    uint32_t scroll_mode_changed;
    uint8_t  scroll_div;
#if KEYBALL_SCROLLSNAP_ENABLE == 2
    keyball_scrollsnap_mode_t scrollsnap_mode;
#endif
    layer_state_t last_layer_state;
    uint16_t      total_mouse_movement;
    uint16_t      auto_mouse_layer_timeout;
    uint16_t       last_kc;
    keypos_t       last_pos;
    report_mouse_t last_mouse;
    char pressing_keys[KEYBALL_OLED_MAX_PRESSING_KEYCODES + 1];
} keyball_t;

extern keyball_t keyball;

void keyball_oled_render_ballinfo(void);
void keyball_oled_render_keyinfo(void);
void keyball_oled_render_layerinfo(void);
bool keyball_get_scroll_mode(void);
void keyball_set_scroll_mode(bool mode);
keyball_scrollsnap_mode_t keyball_get_scrollsnap_mode(void);
void keyball_set_scrollsnap_mode(keyball_scrollsnap_mode_t mode);
uint8_t keyball_get_scroll_div(void);
void keyball_set_scroll_div(uint8_t div);
uint8_t keyball_get_cpi(void);
void keyball_set_cpi(uint8_t cpi);

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
void keyball_handle_auto_mouse_layer_change(layer_state_t state);
uint16_t keyball_get_auto_mouse_timeout(void);
void keyball_set_auto_mouse_timeout(uint16_t timeout);
#endif

void keyball_on_adjust_layout(keyball_adjust_t v);
