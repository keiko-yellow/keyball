/*
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
Licensed under the GNU General Public License v2
*/

#include "quantum.h"
#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif
#include "keyball.h"
#include "drivers/pmw3360/pmw3360.h"
#include <string.h>

const uint8_t CPI_DEFAULT    = KEYBALL_CPI_DEFAULT / 100;
const uint8_t CPI_MAX        = pmw3360_MAXCPI + 1;
const uint8_t SCROLL_DIV_MAX = 7;
const uint16_t AML_TIMEOUT_QU  = 50;
const uint16_t AML_TIMEOUT_MIN = 100;
const uint16_t AML_TIMEOUT_MAX = 1000;
const uint16_t AML_ACTIVATE_THRESHOLD = 50;

keyball_t keyball = {
    .this_have_ball = false,
    .that_enable    = false,
    .that_have_ball = false,
    .this_motion = {0}, .that_motion = {0},
    .cpi_value   = 0, .cpi_changed = false,
    .scroll_mode = false, .scroll_div  = 0,
    .last_layer_state = 0, .total_mouse_movement = 0,
    .pressing_keys = { '\xB0', '\xB0', '\xB0', '\xB0', '\xB0', '\xB0', 0 },
};

// --- Utilities ---
static int16_t add16(int16_t a, int16_t b) {
    int16_t r = a + b;
    if (a >= 0 && b >= 0 && r < 0) r = 32767;
    else if (a < 0 && b < 0 && r >= 0) r = -32768;
    return r;
}

static int16_t divmod16(int16_t *v, int16_t div) {
    int16_t r = *v / div;
    *v -= r * div;
    return r;
}

static inline int8_t clip2int8(int16_t v) {
    return (v) < -127 ? -127 : (v) > 127 ? 127 : (int8_t)v;
}

// --- Auto Mouse Logic (先頭に移動) ---
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
uint16_t keyball_get_auto_mouse_timeout(void) {
    return keyball.auto_mouse_layer_timeout;
}
void keyball_set_auto_mouse_timeout(uint16_t timeout) {
    keyball.auto_mouse_layer_timeout = timeout;
}
static uint16_t get_auto_mouse_keep_time(void) {
#ifdef AUTO_MOUSE_LAYER_KEEP_TIME
    return AUTO_MOUSE_LAYER_KEEP_TIME;
#else
    return keyball_get_auto_mouse_timeout();
#endif
}
bool auto_mouse_activation(report_mouse_t mouse_report) {
    keyball.total_mouse_movement += (abs(mouse_report.x) + abs(mouse_report.y));
    if (AML_ACTIVATE_THRESHOLD < keyball.total_mouse_movement) {
        keyball.total_mouse_movement = 0;
        if (keyball_get_auto_mouse_timeout() != get_auto_mouse_keep_time()) {
            set_auto_mouse_timeout(get_auto_mouse_keep_time());
        }
        return true;
    }
    return mouse_report.buttons;
}
void keyball_handle_auto_mouse_layer_change(layer_state_t state) {
    if (layer_state_cmp(keyball.last_layer_state, AUTO_MOUSE_DEFAULT_LAYER) && !layer_state_cmp(state, AUTO_MOUSE_DEFAULT_LAYER)) {
        set_auto_mouse_timeout(get_auto_mouse_keep_time());
    }
    keyball.last_layer_state = state;
}
#endif

// --- Driver & Motion ---
void pointing_device_driver_init(void) {
    keyball.this_have_ball = pmw3360_init();
    if (keyball.this_have_ball) pmw3360_cpi_set(CPI_DEFAULT - 1);
}
uint16_t pointing_device_driver_get_cpi(void) { return keyball_get_cpi(); }
void pointing_device_driver_set_cpi(uint16_t cpi) { keyball_set_cpi(cpi); }

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
#if (PRODUCT_ID & 0xff00) == 0x0000 // 46
    r->x = clip2int8(m->x); r->y = -clip2int8(m->y);
#else // 39, 44, 61
    r->x = clip2int8(m->y); r->y = clip2int8(m->x);
#endif
    if (is_left) { r->x = -r->x; r->y = -r->y; }
    m->x = 0; m->y = 0;
}

void keyball_on_apply_motion_to_mouse_scroll(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    int16_t div = 1 << (keyball_get_scroll_div() - 1);
    int16_t x = divmod16(&m->x, div); int16_t y = divmod16(&m->y, div);
#if (PRODUCT_ID & 0xff00) == 0x0000 // 46
    r->h = clip2int8(x); r->v = clip2int8(y);
#else
    r->h = clip2int8(y); r->v = -clip2int8(x);
#endif
    if (is_left) { r->h = -r->h; r->v = -r->v; }
#if KEYBALL_SCROLLSNAP_ENABLE == 2
    switch (keyball.scrollsnap_mode) {
        case KEYBALL_SCROLLSNAP_MODE_VERTICAL: r->h = 0; break;
        case KEYBALL_SCROLLSNAP_MODE_HORIZONTAL: r->v = 0; break;
        default: break;
    }
#endif
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t rep) {
    if (keyball.this_have_ball) {
        pmw3360_motion_t d = {0};
        if (pmw3360_motion_burst(&d)) {
            ATOMIC_BLOCK_FORCEON { keyball.this_motion.x = add16(keyball.this_motion.x, d.x); keyball.this_motion.y = add16(keyball.this_motion.y, d.y); }
        }
    }
    if (is_keyboard_master()) {
        if (keyball.scroll_mode) keyball_on_apply_motion_to_mouse_scroll(&keyball.this_motion, &rep, is_keyboard_left());
        else keyball_on_apply_motion_to_mouse_move(&keyball.this_motion, &rep, is_keyboard_left());
        keyball.last_mouse = rep;
    }
    return rep;
}

// --- APIs ---
bool keyball_get_scroll_mode(void) { return keyball.scroll_mode; }
void keyball_set_scroll_mode(bool mode) {
    if (mode != keyball.scroll_mode) keyball.scroll_mode_changed = timer_read32();
    keyball.scroll_mode = mode;
}
keyball_scrollsnap_mode_t keyball_get_scrollsnap_mode(void) { return keyball.scrollsnap_mode; }
void keyball_set_scrollsnap_mode(keyball_scrollsnap_mode_t mode) { keyball.scrollsnap_mode = mode; }
uint8_t keyball_get_scroll_div(void) { return keyball.scroll_div == 0 ? 4 : keyball.scroll_div; }
void keyball_set_scroll_div(uint8_t div) { keyball.scroll_div = div; }
uint8_t keyball_get_cpi(void) { return keyball.cpi_value == 0 ? CPI_DEFAULT : keyball.cpi_value; }
void keyball_set_cpi(uint8_t cpi) {
    keyball.cpi_value = (cpi > CPI_MAX) ? CPI_MAX : cpi;
    keyball.cpi_changed = true; if (keyball.this_have_ball) pmw3360_cpi_set(keyball.cpi_value == 0 ? CPI_DEFAULT - 1 : keyball.cpi_value - 1);
}

// --- OLED Rendering (未使用警告回避) ---
#ifdef OLED_ENABLE
static char to_1x(uint8_t x) { x &= 0x0f; return x < 10 ? x + '0' : x + 'a' - 10; }
static const char LFSTR_ON[] PROGMEM = "\xB2\xB3";
static const char LFSTR_OFF[] PROGMEM = "\xB4\xB5";

void keyball_oled_render_ballinfo(void) {
    oled_write_P(PSTR("Ball:"), false);
    oled_write(format_4d(keyball.last_mouse.x), false);
    oled_write(format_4d(keyball.last_mouse.y), false);
}
void keyball_oled_render_keyinfo(void) {
    oled_write_char(to_1x(keyball.last_pos.row), false);
    oled_write_char(to_1x(keyball.last_pos.col), false);
}
void keyball_oled_render_layerinfo(void) {
    oled_write_P(PSTR("L:"), false);
    for (uint8_t i = 1; i < 5; i++) oled_write_char(layer_state_is(i) ? (i + '0') : '.', false);
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    oled_write_P(get_auto_mouse_enable() ? LFSTR_ON : LFSTR_OFF, false);
    oled_write(format_4d(keyball_get_auto_mouse_timeout() / 10) + 1, false);
#    endif
}
#endif

// --- Record Process ---
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    keyball.last_kc = keycode; keyball.last_pos = record->event.key;
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    if ((IS_MOUSEKEY(keycode) || keycode == SCRL_MO) && record->event.pressed) {
        set_auto_mouse_timeout(keyball_get_auto_mouse_timeout()); keyball.total_mouse_movement = 0;
    }
#endif
    if (record->event.pressed) {
        switch (keycode) {
            case KBC_SAVE: {
                keyball_config_t c = { .cpi = keyball.cpi_value, 
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
                    .amlto = (keyball_get_auto_mouse_timeout() / AML_TIMEOUT_QU) - 1 
#endif
#if KEYBALL_SCROLLSNAP_ENABLE == 2
                    , .ssnap = keyball.scrollsnap_mode
#endif
                }; eeconfig_update_kb(c.raw);
            } break;
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
            case AML_I50: { uint16_t v = keyball_get_auto_mouse_timeout() + 50; keyball_set_auto_mouse_timeout(v > 1000 ? 1000 : v); } break;
            case AML_D50: { uint16_t v = keyball_get_auto_mouse_timeout() - 50; keyball_set_auto_mouse_timeout(v < 100 ? 100 : v); } break;
#endif
        }
    }
    return true;
}

void keyboard_post_init_kb(void) {
    if (eeconfig_is_enabled()) {
        keyball_config_t c = {.raw = eeconfig_read_kb()}; keyball_set_cpi(c.cpi);
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
        keyball_set_auto_mouse_timeout(c.amlto == 0 ? 500 : (c.amlto + 1) * AML_TIMEOUT_QU); set_auto_mouse_timeout(keyball_get_auto_mouse_timeout());
#endif
    }
}

__attribute__((weak)) void keyball_on_adjust_layout(keyball_adjust_t v) {}
