#ifndef LEDS_RUNNER
#define LEDS_RUNNER
// #include <FastLED.h>
// #include "I2SClocklessLedDriver.h"

#include "common.h"
#include "colors.h"
#include "modes.h"

#define NUM_PRESETS 40

int PRESETS[NUM_PRESETS][7] = {
    // { DARK,                           period,  0,          0,          0,            0,            0,            0,        0,          0 },                     // 
    // { TEST_CHASERS,                   period,  0,          0,          0,            0,            0,            0,        0,          0 },
    // { LIGHT_SIDES,                    period,  speed,                                                            palette,  palette_i,  palette_j },
    // { TWINKLE_STARS,                  period,  density,    speed_lo,   speed_hi,     0,            0,            palette,  palette_i,  palette_j },
    // { ROTATE_BANDS,                   period,  num_bands,  0,          0,            0,            0,            palette,  palette_i,  palette_j },
    // { RATS_IN_A_CAGE,                 period,  spacer,     jitter,     prob,         hop,          tail_length,  palette,  palette_i,  palette_j },
    // { SNOWFALL,                       period,  fall_speed, spacer,     jitter,       restart_prob, 0,            palette,  palette_i,  palette_j},
    // { FAKE_SPECTRA,                   period,  speed_lo,   speed_hi,   extent_lo,    extent_hi,    0,            palette,  palette_i,  palette_j },
    // { TRIGGER_LEVELS_DOWN,            period,  speed,      0,          0,            0,            0,            palette,  palette_i,  palette_j },
    // { TRIGGER_LEVELS_UP,              period,  speed,      0,          0,            0,            0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_SWEEP_DOWN_INIT,    period,  speed,      offset,     0,            0,            0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_SWEEP_UP_INIT,      period,  speed,      offset,     0,            0,            0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_FADE_DOWN_INIT,     period,  speed,      offset,     pulse_speed,  0,            0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_FADE_UP_INIT,       period,  speed,      offset,     pulse_speed,  0,            0,            palette,  palette_i,  palette_j },
    // { SPIRAL_LEVELS_SWEEP_DOWN_INIT,  period,  speed,      offset,     0,            0,            0,            palette,  palette_i,  palette_j },
    // { SPIRAL_LEVELS_SWEEP_UP_INIT,    period,  speed,      offset,     0,            0,            0,            palette,  palette_i,  palette_j }

    // Above is a guide
    // Defaults:
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    { DARK,                            0,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                    5,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                   20,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                    2,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                    5,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                    2,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                   20,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    { TEST_CHASERS,                   10,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    
    { LIGHT_SIDES,                  1000,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                  2000,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                   500,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                  1500,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                  1000,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                  2500,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                   500,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                  1000,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                   200,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    { LIGHT_SIDES,                  2000,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },

    
    { TWINKLE_STARS,                   5,     100,        3,        45,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   3,      50,        3,       120,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   5,     100,        3,        55,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                  10,     200,        3,       100,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   5,     100,        3,        65,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   3,     100,        3,        45,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   5,      50,        3,        15,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                  10,     100,       35,       145,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   5,     150,        3,        25,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    { TWINKLE_STARS,                   5,     100,       35,        95,           0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000

    
    
//    { ROTATE_BANDS,                    5,     11,          0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },  
//    
//    { RATS_IN_A_CAGE,                  5,     10,         3,        64,           3,          1 },    //,            palette,  palette_i,  palette_j },  // prob /= 256
//    
//    { SNOWFALL,                        5,     5,          12,       4,            0,          0 },    //,            palette,  palette_i,  palette_j },  // fall_speed /= 1000
//    
//    { FAKE_SPECTRA,                    2,     32,         156,      0,            256,        0 },    //,            palette,  palette_i,  palette_j },
//    
//    { TRIGGER_LEVELS_DOWN,            50,     1,          0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { TRIGGER_LEVELS_UP,              50,     1,          0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { LOOP_LEVELS_SWEEP_DOWN_INIT,     5,     1,          40,       0,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { LOOP_LEVELS_SWEEP_UP_INIT,       5,     1,          40,       0,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { LOOP_LEVELS_FADE_DOWN_INIT,      5,     1,          40,       1,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { LOOP_LEVELS_FADE_UP_INIT,        5,     1,          40,       1,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { SPIRAL_LEVELS_SWEEP_DOWN_INIT,   5,     1,          40,       0,            0,          0 },    //,            palette,  palette_i,  palette_j },
//    
//    { SPIRAL_LEVELS_SWEEP_UP_INIT,     5,     1,          40,       0,            0,          0 }    //,            palette,  palette_i,  palette_j }

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

  void init() {
    this->currentPreset_ = 20;
  };

  int get_weighted_random_preset() {
   this->currentPreset_ = random8(NUM_PRESETS);
   Serial.print("current preset: "); Serial.println(this->currentPreset_);

    return this->currentPreset_;
  };


  int get_next_preset() {
   this->currentPreset_ = (this->currentPreset_ + 1) % NUM_PRESETS;
   Serial.print("current preset: "); Serial.println(this->currentPreset_);

    return this->currentPreset_;
  };

};
#endif
