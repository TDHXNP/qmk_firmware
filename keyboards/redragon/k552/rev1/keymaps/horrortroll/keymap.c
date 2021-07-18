/*
Copyright 2019 Stephen Peery

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

#include "rev1.h"

// For CUSTOM_GRADIENT
HSV gradient_0 = {205, 250, 255};
HSV gradient_100 = {140, 215, 125};
bool reflected_gradient = false;
uint8_t gp_i = 0;

typedef struct {
    HSV gradient_0;
    HSV gradient_1;
    bool reflected;
} CUSTOM_PRESETS;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    _BASE = 0,
    _CSGO = 1,
    _FL = 2,
};

enum layer_keycodes {
  G1_HUI = SAFE_RANGE,    //Custom gradient color 1 hue increase
  G1_HUD,                 //Custom gradient color 1 hue decrease
  G1_SAI,                 //Custom gradient color 1 saturation increase
  G1_SAD,                 //Custom gradient color 1 saturation decrease
  G1_VAI,                 //Custom gradient color 1 value increase
  G1_VAD,                 //Custom gradient color 1 value decrease
  G2_HUI,                 //Custom gradient color 2 hue increase
  G2_HUD,                 //Custom gradient color 2 hue decrease
  G2_SAI,                 //Custom gradient color 2 saturation increase
  G2_SAD,                 //Custom gradient color 2 saturation decrease
  G2_VAI,                 //Custom gradient color 2 value increase
  G2_VAD,                 //Custom gradient color 2 value decrease
  G_PRE,                  //Gradient presets
  REF_G,                  //Toggle between linear and reflected gradient
  G_FLIP,                 //Flip the gradient colors
  
  //Custom user effect keycode
  RGB_C_G,                //Change effect to custom gradient
  RGB_D,                  //Change effect to diagonal
  RGB_R_R,                //Change effect to rainbow reactive simple
  RGB_K,                  //Change effect to knight rider
  RGB_R_B                 //Change effect to random breath rainbow

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
       ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
       │Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Scr│Pse│
       └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐
       │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │Ins│Hom│PgU│
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
       │ Tab │ q │ w │ e │ r │ t │ y │ u │ i │ o │ p │ [ │ ] │  \  │ │Del│End│PgD│
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘
       │ Caps │ a │ s │ d │ f │ g │ h │ j │ k │ l │ ; │ ' │  Enter │
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐
       │ LShift │ z │ x │ c │ v │ b │ n │ m │ , │ . │ / │   RShift │     │ ↑ │
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐
       │LCrl│GUI │LAlt│         Space          │RAlt│ Fn │ App│RCrl│ │ ← │ ↓ │ → │
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘

       ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
       │   │   │   │   │   │   │ │   │   │   │   │ │   │   │   │   │ │   │   │   │
       └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐
       │ ~ │ ! │ @ │ # │ $ │ % │ ^ │ & │ * │ ( │ ) │ _ │ + │       │ │   │   │   │
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
       │     │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ { │ } │  |  │ │   │   │   │
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘
       │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ : │ " │        │
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐
       │ LShift │ Z │ X │ C │ V │ B │ N │ M │ < │ > │ ? │   RShift │     │   │
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐
       │    │    │    │                        │    │    │    │    │ │   │   │   │
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4        5        6        7        8        9        10       11       12       13       14       15         16       */
    [_BASE] = LAYOUT_ansi(
                KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK,   KC_PAUSE,
                KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_INS,  KC_HOME,   KC_PGUP,
                KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,    KC_PGDN,
                KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
                KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,          KC_UP,
                KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(_FL), KC_APP,  KC_RCTL, KC_LEFT, KC_DOWN,   KC_RIGHT
            ),

    /*  Row:    0        1        2        3        4        5        6        7        8        9        10       11       12       13       14       15         16       */
    [_CSGO] = LAYOUT_ansi(
                KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK,   KC_PAUSE,
                KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_INS,  KC_HOME,   KC_PGUP,
                KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,    KC_PGDN,
                KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
                KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,          KC_UP,
                KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(_FL), KC_APP,  KC_RCTL, KC_LEFT, KC_DOWN,   KC_RIGHT
            ),

/*
       ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
       │Rst│   │MeP│VoD│VoU│Mut│ │Stp│Prv│Ply│Nxt│ │Mai│Hom│Cal│Sch│ │   │   │   │
       └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐
       │   │   │   │   │   │   │   │   │   │   │   │Spd│Spi│       │ │Mod│Rod│Tog│
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
       │     │   │   │   │   │   │   │   │   │   │   │   │   │     │ │   │   │   │
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘
       │      │   │   │   │   │   │   │   │   │   │   │   │        │
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐
       │        │   │   │   │   │   │   │   │   │   │   │          │     │Vai│
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐
       │    │TGUI│    │                        │    │ Fn │    │    │ │Hud│Vad│Hui│
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4        5          6        7        8        9        10       11       12       13       14       15         16       */
    [_FL]   = LAYOUT_ansi(
                RESET,            KC_MSEL, KC_VOLD, KC_VOLU, KC_MUTE,   KC_MSTP, KC_MPRV, KC_MPLY, KC_MNXT, KC_MAIL, KC_WHOM, KC_CALC, KC_WSCH, RGB_MOD, RGB_RMOD,  RGB_TOG,
                NK_TOGG, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, RGB_SPD, RGB_SPI, _______, RGB_C_G, RGB_D,     RGB_R_R,
                _______, G1_HUD,  G1_HUI,  G1_SAD,  G1_SAI,  G1_VAD,    G1_VAI,  _______, _______, _______, _______, _______, _______, _______, RGB_K,   RGB_R_B,   TG(_CSGO),
                _______, G2_HUD,  G2_HUI,  G2_SAD,  G2_SAI,  G2_VAD,    G2_VAI,  _______, _______, _______, _______, _______,          _______,
                _______,          G_PRE,   REF_G,   G_FLIP,  _______, _______, _______, _______, _______, _______, _______,          _______,          RGB_VAI,
                _______, GUI_TOG, _______,                              _______,                            _______, _______, _______, _______, RGB_HUD, RGB_VAD,   RGB_HUI
            ),
};

//colours      hsv
#define Red    {0,255,255}
#define Orange {28,255,255}
#define Yellow {43,255,255}
#define Green  {85,255,255}
#define Blue   {170,255,255}
#define Violet {193,255,255}
#define Sakura {242,171,255}
#define White  {0,0,255}
#define ______ {0,0,0}       //no colour

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    /*  Row:    0        1        2        3        4        5        6        7        8        9        10       11       12       13       14       15        16     */
    [_CSGO] = { Red,              ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  Red,     ______,  ______,  ______,  ______,   ______, \
                ______,  Yellow,  Yellow,  Yellow,  Yellow,  Yellow,  ______,  ______,  ______,  ______,  ______,  Sakura,  Sakura,  ______,  ______,  ______,   ______, \
                Red,     Violet,  Green,   Violet,  Violet,  ______,  ______,  ______,  ______,  ______,  Sakura,  Sakura,  Sakura,  ______,  ______,  ______,   ______, \
                ______,  Green,   Green,   Green,   Violet,  Violet,  ______,  ______,  Violet,  ______,  ______,  ______,           ______,                             \
                Orange,           Blue,    Blue,    Blue,    Blue,    Violet,  ______,  ______,  ______,  ______,  ______,           ______,           ______,           \
                Orange,  ______,  ______,                             ______,                             ______,  White,   ______,  ______,  ______,  ______,   ______ },

    [_FL]   = { White,            Green,   Green,   Green,   Green,   Green,   Green,   Green,   Green,   Green,   Green,   Green,   Green,   Orange,  Orange,   Orange, \
                White,   ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  Orange,  Orange,  ______,  Sakura,  Sakura,   Sakura, \
                ______,  Yellow,  Yellow,  Yellow,  Yellow,  Yellow,  Yellow,  ______,  ______,  ______,  ______,  ______,  ______,  ______,  Sakura,  Sakura,   Sakura, \
                ______,  Yellow,  Yellow,  Yellow,  Yellow,  Yellow,  Yellow,  ______,  ______,  ______,  ______,  ______,           ______,                             \
                ______,           Yellow,  Yellow,  Yellow,  ______,  ______,  ______,  ______,  ______,  ______,  ______,           ______,           Orange,           \
                ______,  White,   ______,                             ______,                             ______,  White,   ______,  ______,  Orange,  Orange,   Orange },
};

void set_layer_color(int layer) {

  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uint8_t color_adj_step = 5;
  CUSTOM_PRESETS gradient_presets[] = {
    {{205, 250, 255}, {140, 215, 125}, false },
    {{41, 255, 255}, {233, 245, 255}, false },
    {{45, 245, 155}, {160, 255, 80}, false },
    {{173, 245, 40}, {41, 255, 205}, true },
    {{32, 255, 165}, {217, 185, 70}, false },
    {{240, 255, 145}, {115, 255, 245}, true },
    {{118, 255, 255}, {242, 255, 255}, false },
    {{118, 255, 255}, {242, 255, 255}, false },
    {{212, 0, 0}, {223, 235, 165}, true },
  };
  uint8_t gp_length = sizeof(gradient_presets)/sizeof(gradient_presets[0]);

  switch (keycode) {
	case G1_HUI:
      if (record->event.pressed) {
        gradient_0.h += color_adj_step;
        dprintf("Gradient 0 HSV: %d, %d, %d\n", gradient_0.h, gradient_0.s, gradient_0.v);
      }
      return false;
    case G1_HUD:
      if (record->event.pressed) {
        gradient_0.h -= color_adj_step;
        dprintf("Gradient 0 HSV: %d, %d, %d\n", gradient_0.h, gradient_0.s, gradient_0.v);
      }
      return false;
    case G1_SAI:
      if (record->event.pressed) {
        gradient_0.s = (gradient_0.s + color_adj_step * 2 <= 255) ? gradient_0.s + color_adj_step * 2 : 255;
        dprintf("Gradient 0 HSV: %d, %d, %d\n", gradient_0.h, gradient_0.s, gradient_0.v);
      }
      return false;
    case G1_SAD:
      if (record->event.pressed) {
        gradient_0.s = (gradient_0.s - color_adj_step * 2 >= 0) ? gradient_0.s - color_adj_step * 2 : 0;
        dprintf("Gradient 0 HSV: %d, %d, %d\n", gradient_0.h, gradient_0.s, gradient_0.v);
      }
      return false;
    case G1_VAI:
      if (record->event.pressed) {
        gradient_0.v = (gradient_0.v + color_adj_step * 2 <= 255) ? gradient_0.v + color_adj_step * 2 : 255;
        dprintf("Gradient 0 HSV: %d, %d, %d\n", gradient_0.h, gradient_0.s, gradient_0.v);
      }
      return false;
    case G1_VAD:
      if (record->event.pressed) {
        gradient_0.v = (gradient_0.v - color_adj_step * 2 >= 0) ? gradient_0.v - color_adj_step * 2 : 0;
        dprintf("Gradient 0 HSV: %d, %d, %d\n", gradient_0.h, gradient_0.s, gradient_0.v);
      }
      return false;
    case G2_HUI:
      if (record->event.pressed) {
        gradient_100.h += color_adj_step;
        dprintf("Gradient 100 HSV: %d, %d, %d\n", gradient_100.h, gradient_100.s, gradient_100.v);
      }
      return false;
    case G2_HUD:
      if (record->event.pressed) {
        gradient_100.h -= color_adj_step;
        dprintf("Gradient 100 HSV: %d, %d, %d\n", gradient_100.h, gradient_100.s, gradient_100.v);
      }
      return false;
    case G2_SAI:
      if (record->event.pressed) {
        gradient_100.s = (gradient_100.s + color_adj_step * 2 <= 255) ? gradient_100.s + color_adj_step * 2 : 255;
        dprintf("Gradient 100 HSV: %d, %d, %d\n", gradient_100.h, gradient_100.s, gradient_100.v);
      }
      return false;
    case G2_SAD:
      if (record->event.pressed) {
        gradient_100.s = (gradient_100.s - color_adj_step * 2 >= 0) ? gradient_100.s - color_adj_step * 2 : 0;
        dprintf("Gradient 100 HSV: %d, %d, %d\n", gradient_100.h, gradient_100.s, gradient_100.v);
      }
      return false;
    case G2_VAI:
      if (record->event.pressed) {
        gradient_100.v = (gradient_100.v + color_adj_step * 2 <= 255) ? gradient_100.v + color_adj_step * 2 : 255;
        dprintf("Gradient 100 HSV: %d, %d, %d\n", gradient_100.h, gradient_100.s, gradient_100.v);
      }
      return false;
    case G2_VAD:
      if (record->event.pressed) {
        gradient_100.v = (gradient_100.v - color_adj_step * 2 >= 0) ? gradient_100.v - color_adj_step * 2 : 0;
        dprintf("Gradient 100 HSV: %d, %d, %d\n", gradient_100.h, gradient_100.s, gradient_100.v);
      }
      return false;
    case G_PRE:
      if (record->event.pressed) {
        gp_i = (gp_i + gp_length ) % gp_length;

        gradient_0 = gradient_presets[gp_i].gradient_0;
        gradient_100 = gradient_presets[gp_i].gradient_1;
        reflected_gradient = gradient_presets[gp_i].reflected;

        gp_i += 1;
      }
      return false;
    case REF_G:
      if (record->event.pressed) {
        reflected_gradient = !reflected_gradient;
      }
      return false;
    case G_FLIP:
      if (record->event.pressed) {
        HSV temp_color = gradient_0;
        gradient_0 = gradient_100;
        gradient_100 = temp_color;
      }
      return false;
	case RGB_C_G:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_CUSTOM_GRADIENT);
      return false;
      break;
	case RGB_D:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_DIAGONAL);
      return false;
      break;
	case RGB_R_R:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_RAINBOW_REACTIVE_SIMPLE);
      return false;
      break;
	case RGB_K:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_KITT);
      return false;
      break;
	case RGB_R_B:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_RANDOM_BREATH_RAINBOW);
      return false;
      break;
  }
  return true;
}

void rgb_matrix_indicators_user(void) {
  switch (biton32(layer_state)) {
    case _FL:
      set_layer_color(_FL);
      break;
    case _CSGO:
      set_layer_color(_CSGO);
      break;
  }
}