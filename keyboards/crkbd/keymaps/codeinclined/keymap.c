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

enum {
  UK_PSHL = SAFE_RANGE
};

#include "./layers.h"

static uint8_t pushed_layer = 0;

layer_state_t layer_state_set_user(layer_state_t state) {
  rgb_matrix_sethsv_noeeprom(get_highest_layer(state), 0, 0);

  return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
  rgb_matrix_sethsv_noeeprom(get_highest_layer(state), 0, 0);

  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case UK_PSHL:
    if (record->event.pressed && pushed_layer == 0) {
      pushed_layer = get_highest_layer(default_layer_state);
      tap_code(KC_ENTER);
      set_single_default_layer(CI_LAY_BASE);
    }

    return false;

  case LCTL_T(KC_ENTER):
    if (pushed_layer == 0) {
      return true;
    }

    if (record->event.pressed && record->tap.count > 0) {
      tap_code(KC_ENTER);
      set_single_default_layer(pushed_layer);
      pushed_layer = 0;

      return false;
    }

    return true;

  // Prevent any weirdness with switching default layers using DF(n)
  // while a layer is pushed by clearing the pushed_layer first.
  // Make sure to add any new default layers here
  case DF(CI_LAY_BASE):
  case DF(CI_LAY_WASD):
  case DF(CI_LAY_LOL):
    pushed_layer = 0;
    return true;

  default:
    return true;
  }
}
