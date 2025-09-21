// vim: set ft=c:
RGB_MATRIX_EFFECT(ci_layers_rgb)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "./cp_colors.h"
#include "./layers_colors.h"

static bool ci_layers_rgb(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  uint8_t layer = rgb_matrix_get_hue();

  for (int i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i,
      ci_layers_colors[layer][i][0],
      ci_layers_colors[layer][i][1],
      ci_layers_colors[layer][i][2]
    );
  }

  return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
