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
#include "./layers.h"

layer_state_t layer_state_set_user(layer_state_t state) {
  rgb_matrix_sethsv_noeeprom(get_highest_layer(state), 0, 0);

  return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
  rgb_matrix_sethsv_noeeprom(get_highest_layer(state), 0, 0);

  return state;
}
