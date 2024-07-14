/*
Copyright 2022 Cole Smith <cole@boadsource.xyz>

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

// #include "layout.h"
// #define TD_INDEX(keycode) ((keycode) & 0xFF




enum layers {
    _QWERTY,
    _RAISE,
    _LOWER,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_split_3x5_3(
                           KC_Q, KC_W, KC_E, KC_R, KC_T,                                    KC_Y, KC_U, KC_I, KC_O, KC_P,
                           LALT_T(KC_A), LCTL_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F), KC_G,    KC_H, LSFT_T(KC_J), LGUI_T(KC_K), LCTL_T(KC_L),LALT_T(KC_SCLN),
                           KC_Z, KC_X, KC_C, KC_V, KC_B,                                    KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
                             KC_ESC,LT(LOWER,KC_TAB),  KC_SPC,                              KC_ENT, LT(RAISE,KC_BSPC), KC_CAPS
                             ),
    [_LOWER] = LAYOUT_split_3x5_3(
                             KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,                         KC_5, KC_6, KC_7, KC_8, KC_9,
                             KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,                        KC_0, KC_1, KC_2, KC_3, KC_4,
                             LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_B),  KC_MINS, KC_EQL, KC_ASTR, KC_PLUS,KC_SLSH,
                             KC_NO, KC_NO, KC_TRNS,                                        KC_TRNS, KC_TRNS, KC_TRNS
                             ),
    [_RAISE] = LAYOUT_split_3x5_3(
                             LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_4),LSFT(KC_5),  KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                             LSFT(KC_6), LSFT(KC_7), LSFT(KC_8), LSFT(KC_9),LSFT(KC_0),  KC_DEL, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,
                             KC_GRV, KC_TILDE, KC_UNDS, KC_QUOT , KC_DQUO,          KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,
                             KC_NO, KC_NO, KC_TRNS,                             KC_TRNS, KC_NO, KC_NO
                             )
};

#include "timer.h"

// Tap-Hold 키 처리 함수
bool process_tap_hold_key(uint16_t keycode, keyrecord_t *record, uint16_t tap_key, uint16_t hold_key, uint16_t hold_delay) {
    static uint16_t tap_timer;

    if (record->event.pressed) {
        tap_timer = timer_read();
    } else {
        if (timer_elapsed(tap_timer) < TAPPING_TERM) {
            register_code16(tap_key);
            unregister_code16(tap_key);
        } else {
            register_code16(hold_key);
            if (hold_key == KC_CAPS) {
                wait_ms(hold_delay); // 맥에선 CAPS LOCK이 탭홀드로 동작해서, 누르고 있는 시간을 넣었다
            }
            unregister_code16(hold_key);
        }
    }
    return false;
}

// 사용자 정의 레코드 처리 함수
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // 콘솔이 활성화되어 있으면 매트릭스 위치와 키 상태를 출력합니다.
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    switch (keycode) {
        case KC_Q:    return process_tap_hold_key(keycode, record, KC_Q, KC_1, 0);
        case KC_W:    return process_tap_hold_key(keycode, record, KC_W, KC_2, 0);
        case KC_E:    return process_tap_hold_key(keycode, record, KC_E, KC_3, 0);
        case KC_R:    return process_tap_hold_key(keycode, record, KC_R, KC_4, 0);
        case KC_T:    return process_tap_hold_key(keycode, record, KC_T, KC_5, 0);
        case KC_Y:    return process_tap_hold_key(keycode, record, KC_Y, KC_6, 0);
        case KC_U:    return process_tap_hold_key(keycode, record, KC_U, KC_7, 0);
        case KC_I:    return process_tap_hold_key(keycode, record, KC_I, KC_8, 0);
        case KC_O:    return process_tap_hold_key(keycode, record, KC_O, KC_9, 0);
        case KC_P:    return process_tap_hold_key(keycode, record, KC_P, KC_0, 0);
        case KC_Z:    return process_tap_hold_key(keycode, record, KC_Z, KC_GRV, 0);
        case KC_X:    return process_tap_hold_key(keycode, record, KC_X, KC_TILDE, 0);
        case KC_C:    return process_tap_hold_key(keycode, record, KC_C, KC_UNDS, 0);
        case KC_V:    return process_tap_hold_key(keycode, record, KC_V, KC_QUOT, 0);
        case KC_B:    return process_tap_hold_key(keycode, record, KC_B, KC_DQUO, 0);
        case KC_N:    return process_tap_hold_key(keycode, record, KC_N, KC_MINS, 0);
        case KC_M:    return process_tap_hold_key(keycode, record, KC_M, KC_EQL, 0);
        case KC_COMM: return process_tap_hold_key(keycode, record, KC_COMM, KC_LBRC, 0);
        case KC_DOT:  return process_tap_hold_key(keycode, record, KC_DOT, KC_RBRC, 0);
        case KC_SLSH: return process_tap_hold_key(keycode, record, KC_SLSH, KC_BSLS, 0);
        case KC_G:    return process_tap_hold_key(keycode, record, KC_G, KC_ESC, 100);   // 지연시간 없으면  키보드 동작에 문제가 발생한다
        case KC_H:    return process_tap_hold_key(keycode, record, KC_H, KC_CAPS, 180);   // 맥에선 캡스락이 자체 탭홀드라 누르는 시간 증가
    }
    return true;
}

