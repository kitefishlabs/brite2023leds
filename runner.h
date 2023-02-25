#ifndef LEDS_RUNNER
#define LEDS_RUNNER
// #include <FastLED.h>
// #include "I2SClocklessLedDriver.h"

#include "common.h"
#include "colors.h"
#include "modes.h"

#define NUM_PRESETS 180

int PRESETS[NUM_PRESETS][8] = {
    // { DARK,                           period,  0,          0,          0,            0,            0,            0,      },
    // { TEST_CHASERS,                   period,  0,          0,          0,            0,            0,            0,      },
    // { LIGHT_SIDES,                    period,  speed,                                                            palette },
    // { TWINKLE_STARS,                  period,  density,    speed_lo,   speed_hi,     0,            0,            palette },
    // { ROTATE_BANDS,                   period,  spped,      num_bands,  0,            0,            0,            palette },
    // { RATS_IN_A_CAGE,                 period,  spacer,     jitter,     prob,         hop,          tail_length,  palette },
    // { SNOWFALL,                       period,  fall_speed, spacer,     jitter,       restart_prob, 0,            palette },
    // { FAKE_SPECTRA,                   period,  speed_lo,   speed_hi,   extent_lo,    extent_hi,    0,            palette },
    // { TRIGGER_LEVELS_DOWN,            period,  speed,      0,          0,            0,            0,            palette },
    // { TRIGGER_LEVELS_UP,              period,  speed,      0,          0,            0,            0,            palette },
    // { LOOP_LEVELS_SWEEP_DOWN_INIT,    period,  speed,      offset,     0,            0,            0,            palette },
    // { LOOP_LEVELS_SWEEP_UP_INIT,      period,  speed,      offset,     0,            0,            0,            palette },
    // { LOOP_LEVELS_FADE_DOWN_INIT,     period,  speed,      offset,     pulse_speed,  0,            0,            palette },
    // { LOOP_LEVELS_FADE_UP_INIT,       period,  speed,      offset,     pulse_speed,  0,            0,            palette },
    // { SPIRAL_LEVELS_SWEEP_DOWN_INIT,  period,  speed,      offset,     0,            0,            0,            palette },
    // { SPIRAL_LEVELS_SWEEP_UP_INIT,    period,  speed,      offset,     0,            0,            0,            palette }

    // Above is a guide
    // Defaults:
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    { DARK,                            0,     0,          0,        0,            0,          0,      0 },
    
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                    5,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                   20,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                    2,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                    5,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                    2,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                   20,     0,          0,        0,            0,          0,      0 },
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0,      0 },
    
    { LIGHT_SIDES,                  4000,     2,          0,        0,            0,          0,      0 },
    { LIGHT_SIDES,                  3000,     2,          0,        0,            0,          0,      1 },
    { LIGHT_SIDES,                  3500,     3,          0,        0,            0,          0,      2 },
    { LIGHT_SIDES,                  1500,     2,          0,        0,            0,          0,      3 },
    { LIGHT_SIDES,                  6000,     2,          0,        0,            0,          0,      4 },
    { LIGHT_SIDES,                  2500,     3,          0,        0,            0,          0,      5 },
    { LIGHT_SIDES,                  2500,     3,          0,        0,            0,          0,      6 },
    { LIGHT_SIDES,                  3000,     0,          0,        0,            0,          0,      7 },
    { LIGHT_SIDES,                  2200,     2,          0,        0,            0,          0,      0 },
    { LIGHT_SIDES,                  3000,     1,          0,        0,            0,          0,      1 },

    
    { TWINKLE_STARS,                  10,     100,        3,        45,           0,          0,      9 },
    { TWINKLE_STARS,                  13,      70,        3,       120,           0,          0,     10 },
    { TWINKLE_STARS,                  15,     120,        3,        55,           0,          0,     11 },
    { TWINKLE_STARS,                  10,     200,        3,       100,           0,          0,     12 },
    { TWINKLE_STARS,                  12,      40,        3,        65,           0,          0,      8 },
    { TWINKLE_STARS,                  13,      30,        3,        45,           0,          0,      9 },
    { TWINKLE_STARS,                  15,      75,        3,        25,           0,          0,     10 },
    { TWINKLE_STARS,                  10,      15,       35,       145,           0,          0,     11 },
    { TWINKLE_STARS,                  12,      50,        3,        25,           0,          0,     12 },
    { TWINKLE_STARS,                  15,      20,       35,        95,           0,          0,      8 },
  
//
    { ROTATE_BANDS,                   12,      1,          4,        1,           0,          0,      0 },
    { ROTATE_BANDS,                   10,      1,          8,        1,           0,          0,      1 },
    { ROTATE_BANDS,                   15,      1,         12,        1,           0,          0,      2 },
    { ROTATE_BANDS,                   12,      3,          4,        1,           0,          0,      3 },
    { ROTATE_BANDS,                   25,      1,          2,        1,           0,          0,      4 },
    { ROTATE_BANDS,                    7,      1,          8,        1,           0,          0,      5 },
    { ROTATE_BANDS,                   25,      1,         12,        1,           0,          0,      6 },
    { ROTATE_BANDS,                   32,      2,          4,        1,           0,          0,      7 },
    { ROTATE_BANDS,                   27,      1,          8,        1,           0,          0,      8 },
    { ROTATE_BANDS,                   25,      1,         12,        1,           0,          0,      9 },

    { ROTATE_BANDS,                   15,      1,          4,       -1,           0,          0,      0 },
    { ROTATE_BANDS,                   12,      1,          8,       -1,           0,          0,      1 },
    { ROTATE_BANDS,                    7,      1,         12,       -1,           0,          0,      2 },
    { ROTATE_BANDS,                   10,      3,          4,       -1,           0,          0,      3 },
    { ROTATE_BANDS,                   25,      1,          2,       -1,           0,          0,      4 },
    { ROTATE_BANDS,                   12,      1,          8,       -1,           0,          0,      5 },
    { ROTATE_BANDS,                   15,      1,         12,       -1,           0,          0,      6 },
    { ROTATE_BANDS,                    9,      2,          4,       -1,           0,          0,      7 },
    { ROTATE_BANDS,                   12,      1,          8,       -1,           0,          0,      8 },
    { ROTATE_BANDS,                   15,      1,         12,       -1,           0,          0,      9 },
//    

    { RATS_IN_A_CAGE,                  5,     30,        10,         8,           2,          1,      0 },
    { RATS_IN_A_CAGE,                  5,     10,         3,        16,           3,          1,      1 },
    { RATS_IN_A_CAGE,                  2,     20,         3,        32,           5,          1,      2 },
    { RATS_IN_A_CAGE,                  5,     45,        15,        16,           3,          2,      3 },
    { RATS_IN_A_CAGE,                 10,     10,         3,        32,           5,          1,      4 },
    { RATS_IN_A_CAGE,                  5,     60,        10,         8,           7,          1,      5 },
    { RATS_IN_A_CAGE,                  5,     30,        10,         4,           2,          1,      6 },
    { RATS_IN_A_CAGE,                 10,     45,         5,        16,           1,          2,      7 },
    { RATS_IN_A_CAGE,                  3,     25,         3,         8,           3,          1,      8 },
    { RATS_IN_A_CAGE,                  5,     60,        20,         8,           1,          2,      9 },

// { SNOWFALL,                    period, fall_speed,spacer,    jitter,restart_prob,          0,palette },
   { SNOWFALL,                         4,      2,        12,         4,           0,          0,      0 },
   { SNOWFALL,                         2,      4,        12,         4,           0,          0,      1 },
   { SNOWFALL,                         5,      5,        12,         4,           0,          0,      2 },
   { SNOWFALL,                         3,      3,        12,         4,           0,          0,      3 },
   { SNOWFALL,                         5,      5,        12,         4,           0,          0,      4 },
   { SNOWFALL,                         5,      5,        12,         4,           0,          0,      5 },
   { SNOWFALL,                         2,      5,        12,         4,           0,          0,      6 },
   { SNOWFALL,                         5,      3,        12,         4,           0,          0,      7 },
   { SNOWFALL,                         3,      5,        12,         4,           0,          0,      8 },
   { SNOWFALL,                         5,      4,        12,         4,           0,          0,      9 },
//    
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      0 },
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      1 },
   { FAKE_SPECTRA,                     3,     24,       256,        32,         192,          0,      2 },
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      3 },
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      4 },
   { FAKE_SPECTRA,                     3,     24,       256,        32,         192,          0,      5 },
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      6 },
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      7 },
   { FAKE_SPECTRA,                     3,     24,       256,        32,         192,          0,      8 },
   { FAKE_SPECTRA,                     2,     24,       256,        32,         192,          0,      9 },

// 
   { TRIGGER_LEVELS_DOWN,             250,      1,        45,         0,           0,          0,      0 },
   { TRIGGER_LEVELS_DOWN,             450,      1,        20,         0,           0,          0,      2 },
   { TRIGGER_LEVELS_DOWN,             150,      1,        15,         0,           0,          0,      4 },
   { TRIGGER_LEVELS_DOWN,             50,      1,         8,         0,           0,          0,      6 },
   { TRIGGER_LEVELS_DOWN,             350,      1,         6,         0,           0,          0,      8 },
   
   { TRIGGER_LEVELS_UP,               450,      1,        45,         0,           0,          0,      1 },
   { TRIGGER_LEVELS_UP,               250,      1,        20,         0,           0,          0,      3 },
   { TRIGGER_LEVELS_UP,               150,      1,        15,         0,           0,          0,      5 },
   { TRIGGER_LEVELS_UP,               50,      1,         8,         0,           0,          0,      7 },
   { TRIGGER_LEVELS_UP,               350,      1,         6,         0,           0,          0,      9 },
   
   { LOOP_LEVELS_SWEEP_DOWN_INIT,      5,      1,        42,         0,           0,          0,      0 },
   { LOOP_LEVELS_SWEEP_DOWN_INIT,      5,      1,        20,         0,           0,          0,      2 },
   { LOOP_LEVELS_SWEEP_DOWN_INIT,      5,      1,        12,         0,           0,          0,      4 },
   { LOOP_LEVELS_SWEEP_DOWN_INIT,      5,      1,         8,         0,           0,          0,      6 },
   { LOOP_LEVELS_SWEEP_DOWN_INIT,      5,      1,         6,         0,           0,          0,      8 },
//    
   { LOOP_LEVELS_SWEEP_UP_INIT,        5,      1,        42,         0,           0,          0,      1 },
   { LOOP_LEVELS_SWEEP_UP_INIT,        5,      1,        20,         0,           0,          0,      3 },
   { LOOP_LEVELS_SWEEP_UP_INIT,        5,      1,        12,         0,           0,          0,      5 },
   { LOOP_LEVELS_SWEEP_UP_INIT,        5,      1,         8,         0,           0,          0,      7 },
   { LOOP_LEVELS_SWEEP_UP_INIT,        5,      1,         6,         0,           0,          0,      9 },
//    
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        35,         5,           0,          0,      0 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        35,         9,           0,          0,      1 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        10,         3,           0,          0,      2 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        10,         2,           0,          0,      3 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        25,         1,           0,          0,      4 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        25,         7,           0,          0,      5 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        20,         1,           0,          0,      6 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        20,         3,           0,          0,      7 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        15,         1,           0,          0,      8 },
   { LOOP_LEVELS_FADE_DOWN_INIT,       5,      1,        15,         4,           0,          0,      9 },
//    
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        35,         7,           0,          0,      0 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        35,         3,           0,          0,      1 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        10,         5,           0,          0,      2 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        10,         1,           0,          0,      3 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        25,         6,           0,          0,      4 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        25,         7,           0,          0,      5 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        20,         1,           0,          0,      6 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        20,         3,           0,          0,      7 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        15,         5,           0,          0,      8 },
   { LOOP_LEVELS_FADE_UP_INIT,         5,      1,        15,         9,           0,          0,      9 },

//    
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      0 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      1 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      2 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      3 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      4 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      5 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      6 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      7 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      8 },
   { SPIRAL_LEVELS_SWEEP_DOWN_INIT,    5,      1,        40,         0,           0,          0,      9 },
//    
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      0 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      1 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      2 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      3 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      4 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      5 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      6 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      7 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      8 },
   { SPIRAL_LEVELS_SWEEP_UP_INIT,      5,      1,        40,         0,           0,          0,      9 }

};

class LEDsPresetsRunner {

private:
  int currentPreset_;
  CRGBPalette16 currentPalette;
  TBlendType    currentBlending;

public:
  LEDsPresetsRunner() {      // int sides[2][NUMSTRIPS][5]
    currentPreset_ = 0;
  };

  void init(int presetid=0) {
    this->currentPreset_ = presetid;
  };

  int get_weighted_random_preset() {
   this->currentPreset_ = random8(NUM_PRESETS);
   Serial.print("current preset: "); Serial.println(this->currentPreset_);

    return this->currentPreset_;
  };


  int get_next_preset() {
   this->currentPreset_ = (this->currentPreset_ + 1) % NUM_PRESETS;
   Serial.print("current preset: "); Serial.println(this->currentPreset_);
   Serial.println("CHECK CHECK CHECK!");
    return this->currentPreset_;
  };

};
#endif
