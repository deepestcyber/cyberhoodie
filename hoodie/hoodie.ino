#include "FastLED.h"

#define PIN_LAYER_0    5
#define PIN_LAYER_1    3

const unsigned int leds_per_module = 7;
const unsigned int num_layers = 2;
const unsigned int modules_per_layer[] = {3, 4};

CRGB *flat_leds[num_layers];

static CRGB* leds_by_module(int layer, int module) {
  return &flat_leds[layer][module * leds_per_module];
}

void rainbow(int layer, int module, int state, uint8_t chance=0) {
  CRGB* leds = leds_by_module(layer, module);
  
  for (int j = 0; j < leds_per_module; j++) {
    leds[j] = CHSV((7 + 256 / leds_per_module * (state + random8(chance) + j) - 1) % 256, 255, 255);
  }
}

void moving_dot(int layer, int module, int last_state, int state) {
  CRGB* leds = leds_by_module(layer, module);
  
  leds[last_state % leds_per_module] = CRGB::Black;
  leds[state % leds_per_module] = CRGB::Red;
}

void moving_ring(int layer, int module, int last_state, int state) {
  CRGB* leds = leds_by_module(layer, module);
  int i = (last_state % (leds_per_module - 1)) + 1;
  int j = (state % (leds_per_module - 1)) + 1;
  
  leds[i] = CRGB::Black;
  leds[j] = CRGB::Red;
}

void setup() {  
  flat_leds[0] = new CRGB[modules_per_layer[0] * leds_per_module];
  flat_leds[1] = new CRGB[modules_per_layer[1] * leds_per_module];

  FastLED.addLeds<NEOPIXEL, PIN_LAYER_0>(flat_leds[0], modules_per_layer[0] * leds_per_module);
  FastLED.addLeds<NEOPIXEL, PIN_LAYER_1>(flat_leds[1], modules_per_layer[1] * leds_per_module);

  for (int layer=0; layer < num_layers; layer++) {
    for (int module=0; module < modules_per_layer[layer]; module++) {
      CRGB* module_leds = leds_by_module(layer, module);

      for (int i=0; i < leds_per_module; i++) {
        module_leds[i] = CRGB::Green;
      }
    }
  }

  FastLED.show();
  FastLED.setBrightness(50);
  delay(100);
}

void fire_neuron(int layer, int module, int state) {
  CRGB* module_leds = leds_by_module(layer, module);
  int seq_state = state % 15;

  if (seq_state < 5) {
    module_leds[0] = CRGB::Blue;
  } else if (seq_state < 10) {
    module_leds[0] = CRGB::Black;
    for (int i=1; i < leds_per_module; i++) {
      module_leds[i] = CRGB::Red;
    }
  } else {
    for (int i=0; i < leds_per_module; i++) {
      module_leds[i] = CRGB::Black;
    }
  }
}


int state = 0;
int last_state = state;

void loop_spinning_wheels() {
  moving_ring(0, 0, last_state, state);
  moving_ring(0, 1, last_state+1, state+1);
  moving_ring(0, 2, last_state+2, state+2);
  moving_ring(1, 0, last_state+10, state+10);
  moving_ring(1, 1, last_state+11, state+11);
  moving_ring(1, 2, last_state+12, state+12);
  moving_ring(1, 3, last_state+13, state+13);

  last_state = state;
  state = (state + 1) % 256;
  
  FastLED.show();
  delay(100);
}

void loop_random_firing() {
  int layer = random() % 2;
  int module = random() % modules_per_layer[layer];

  fire_neuron(layer, module, state);

  last_state = state;
  state = (state + 1) % 256;
  
  FastLED.show();
  delay(100);
}

void loop() {
  //loop_spinning_wheels();
  loop_random_firing();
}



