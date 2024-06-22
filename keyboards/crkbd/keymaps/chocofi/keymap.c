/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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
//https://getreuer.info/posts/keyboards/layer-lock/index.html
#include "features/layer_lock.h"
//https://getreuer.info/posts/keyboards/achordion/index.html
#include "features/achordion.h"

enum custom_keycodes {
    LLOCK = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y,   KC_QUOT,  KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,  LGUI_T(KC_A), LALT_T(KC_R), LCTL_T(KC_S), LSFT_T(KC_T), KC_G,       KC_M,  RSFT_T(KC_N), RCTL_T(KC_E), LALT_T(KC_I), RGUI_T(KC_O), KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H,  KC_COMM,  KC_DOT,  KC_SLSH,  KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                        KC_ESC, LT(1,KC_SPC), KC_TAB, KC_ENT, LT(2, KC_BSPC), CW_TOGG
                                      //`--------------------------'  `--------------------------'

  ),

    //NAV
    [1] = LAYOUT_split_3x6_3(                                                             //maybe put some keys for using warpd here?
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_NO,  KC_NO, KC_KB_VOLUME_UP, KC_KB_MUTE, KC_KB_VOLUME_DOWN, KC_TAB,    KC_DEL, KC_PSCR,  KC_NO,   KC_NO,  KC_CAPS,  KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_NO,  KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_NO,                        KC_NO,   KC_LEFT,KC_DOWN, KC_UP,   KC_RIGHT, KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_SPC,  LLOCK,                       KC_INS,  KC_HOME,KC_PGDN, KC_PGUP, KC_END,   KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_NO,  _______,  KC_NO,     KC_ENT, KC_BSPC, KC_NO
                                      //`--------------------------'  `--------------------------'
  ),

    //NUMFUNC
    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, LGUI_T(KC_1), LALT_T(KC_2), LCTL_T(KC_3), LSFT_T(KC_4), KC_5,       KC_6, RSFT_T(KC_7), RCTL_T(KC_8), LALT_T(KC_9), RGUI_T(KC_0), KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,   KC_F11,  KC_F12,  KC_NO,   KC_NO,   KC_NO,                        LLOCK,   KC_BSPC, _______, _______, _______, KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                         KC_ESC,   KC_SPC,  KC_TAB,      KC_NO,  _______, KC_NO
                                      //`--------------------------'  `--------------------------'
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!process_achordion(keycode, record)) { return false; }
    if (!process_layer_lock(keycode, record, LLOCK)) { return false; }

    return true;
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
    switch (tap_hold_keycode) {
        case LT(2, KC_BSPC):
        case LT(1, KC_SPC):
            return true;
    }

    return achordion_opposite_hands(tap_hold_record, other_record);
}

void matrix_scan_user(void) {
    achordion_task();
}

#include "combos.h"

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    if (combo->keys[0] == KC_L && combo->keys[1] == KC_U) {
        return 20;
    }

    return COMBO_TERM;
}

//default except removed - from keys that get shifted and added to keys that don't end
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case KC_MINS:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}
