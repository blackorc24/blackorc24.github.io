

#include QMK_KEYBOARD_H

// #include "layout.h"

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;


// Tap dance enums
enum {
    Q_CTL = 0,
    W_CTL,
    E_CTL,
    R_CTL,
    T_CTL,
    Y_CTL,
    U_CTL,
    I_CTL,
    O_CTL,
    P_CTL,
    Z_CTL,
    X_CTL,
    C_CTL,
    V_CTL,
    B_CTL,
    N_CTL,
    M_CTL,
    COMM_CTL,
    DOT_CTL,
    BSLS_CTL,
    G_CTL,
    H_CTL,
    SCLN_CTL
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x5_3(
                            Q_CTL, W_CTL,E_CTL, R_CTL, T_CTL,       Y_CTL, U_CTL, I_CTL, O_CTL, P_CTL,
                             LOPT_T(KC_A), LCTL_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F),KC_G, KC_H,  LSFT_T(KC_J), LGUI_T(KC_K),LCTL_T(KC_L),LOPT_T(KC_SCLN),
                            Z_CTL, X_CTL, C_CTL, V_CTL, B_CTL,    N_CTL, M_CTL, COMM_CTL, DOT_CTL, BSLS_CTL,
                             KC_HYPR,LT(1,KC_TAB),  KC_SPC,                    KC_ENT, LT(2,KC_BSPC), KC_LNG1
                             ),
    [1] = LAYOUT_split_3x5_3(
                             KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,                         KC_5, KC_6, KC_7, KC_8, KC_9,
                             KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,                        KC_0, KC_1, KC_2, KC_3, KC_4,
                             LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_B),  KC_MINS, KC_EQL, KC_ASTR, KC_PLUS,KC_SLSH,
                             KC_NO, KC_NO, KC_SPC,                                        KC_ENT, KC_TRNS, KC_TRNS
                             ),
    [2] = LAYOUT_split_3x5_3(
                             KC_NO, KC_WH_D, KC_MS_U, KC_WH_U, KC_ACL0,          KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
                             KC_NO, KC_MS_L, KC_MS_D, KC_MS_R, KC_ACL1,          KC_DEL, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,
                             KC_MUTE, KC_VOLD, KC_VOLU, KC_NO , KC_ACL2,          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                             KC_BTN3, KC_BTN2, KC_BTN1,                             KC_NO, KC_NO, KC_NO
                             )
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;
    // static uint16_t my_hash_timer;

    #ifdef CONSOLE_ENABLE
        uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    #endif


    switch (keycode) {
        case Q_CTL:            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case W_CTL:            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(E_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(R_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(T_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(Y_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(U_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(I_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(O_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(P_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(Z_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(X_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(C_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(V_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(B_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(N_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(M_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(COMM_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(DOT_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(BSLS_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(G_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        //  case H_CTL:
        //     if (record->tap.count && record->event.pressed ) {
        //         tap_code16(KC_H); // Intercept tap function to send Ctrl-C
        //     } else if (record->event.pressed) {
        //         caps_word_toggle();
        //     }
        //     return false;
        // --
        // case H_CTL:
        //     if (record->event.pressed ) {
        //          my_hash_timer = timer_read();
        //     } else {
        //         if(timer_elapsed(my_hash_timer) < TAPPING_TERM) {
        //             register_code16(KC_H);
        //             unregister_code16(KC_H);
        //         }  else {
        //             caps_word_on();
        //         }
        //     }
        //     return false;
        //     break;
        // ----
        case TD(H_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
        case TD(SCLN_CTL):            action = &tap_dance_actions[TD_INDEX(keycode)];            if (!record->event.pressed && action->state.count && !action->state.finished) {                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;                tap_code16(tap_hold->tap);            };
            break;
    }
    return true;
}

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
            ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
{ .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }


tap_dance_action_t tap_dance_actions[] = {
    [Q_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_Q, KC_1),
    [W_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_W, KC_2),
    [E_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_E, KC_3),
    [R_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_4),
    [T_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_T, KC_5),
    [Y_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_Y, KC_6),
    [U_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_U, KC_7),
    [I_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_I, KC_8),
    [O_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_O, KC_9),
    [P_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_P, KC_0),
    [Z_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_Z, KC_GRV),
    [X_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_X, KC_TILDE),
    [C_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_C, KC_UNDS),
    [V_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_V, KC_QUOT),
    [B_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_B, KC_DQUO),
    [N_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_N, KC_MINS),
    [M_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_M, KC_EQL),
    [COMM_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_COMM, KC_LBRC),
    [DOT_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_DOT, KC_RBRC),
    [BSLS_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_SLSH, KC_BSLS),
    [G_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_G, KC_ESC),
    [H_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_H, KC_CAPS),
    [SCLN_CTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_SCLN, KC_QUOT)
};


