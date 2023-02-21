#ifndef LEDS_RUNNER
#define LEDS_RUNNER
// #include <FastLED.h>
// #include "I2SClocklessLedDriver.h"

#include "common.h"
#include "colors.h"
#include "modes.h"

#define NUM_PRESETS 16

int PRESETS[NUM_PRESETS][7] = {
    // { DARK,                           period,  0,          0,        0,            0,          0,            0,        0,          0 },                     // 
    // { TEST_CHASERS,                   period,  0,          0,        0,            0,          0,            0,        0,          0 },
    // { LIGHT_SIDES,                    period,  speed,                                                        palette,  palette_i,  palette_j },
    // { TWINKLE_STARS,                  period,  density,    0,        0,            0,          0,            palette,  palette_i,  palette_j },
    // { ROTATE_BANDS,                   period,  period,  num_bands,  0,        0,            0,          0,            palette,  palette_i,  palette_j },
    // { RATS_IN_A_CAGE,                 period,  spacer,     jitter,   prob,         hop,        tail_length,  palette,  palette_i,  palette_j },
    // { SNOWFALL,                       period,  fall_speed, spacer,   jitter,       0,          0,            palette,  palette_i,  palette_j},
    // { FAKE_SPECTRA,                   period,  speed_lo,   speed_hi, extent_lo,    extent_hi,  0,            palette,  palette_i,  palette_j },
    // { TRIGGER_LEVELS_DOWN,            period,  speed,      0,        0,            0,          0,            palette,  palette_i,  palette_j },
    // { TRIGGER_LEVELS_UP,              period,  speed,      0,        0,            0,          0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_SWEEP_DOWN_INIT,    period,  speed,      offset,   0,            0,          0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_SWEEP_UP_INIT,      period,  speed,      offset,   0,            0,          0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_FADE_DOWN_INIT,     period,  speed,      offset,   pulse_speed,  0,          0,            palette,  palette_i,  palette_j },
    // { LOOP_LEVELS_FADE_UP_INIT,       period,  speed,      offset,   pulse_speed,  0,          0,            palette,  palette_i,  palette_j },
    // { SPIRAL_LEVELS_SWEEP_DOWN_INIT,  period,  speed,      offset,   0,            0,          0,            palette,  palette_i,  palette_j },
    // { SPIRAL_LEVELS_SWEEP_UP_INIT,    period,  speed,      offset,   0,            0,          0,            palette,  palette_i,  palette_j }

    // Above is a guide
    // Defaults:
    { DARK,                           50,     0,          0,        0,            0,          0 },    //             0,        0,          0 },          // 
    
    { TEST_CHASERS,                   50,     0,          0,        0,            0,          0 },    //             0,        0,          0 },
    
    { LIGHT_SIDES,                    50,     1,          0,        0,            0,          0 },    //             palette,  palette_i,  palette_j },
    
    { TWINKLE_STARS,                  50,     100,        0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },  // density /= 1000
    
    { ROTATE_BANDS,                   50,     4,          0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },  
    
    { RATS_IN_A_CAGE,                 50,     10,         3,        64,           3,          1 },    //,            palette,  palette_i,  palette_j },  // prob /= 256
    
    { SNOWFALL,                       50,     5,          12,       4,            0,          0 },    //,            palette,  palette_i,  palette_j },  // fall_speed /= 1000
    
    { FAKE_SPECTRA,                   50,     32,         156,      0,            256,        0 },    //,            palette,  palette_i,  palette_j },
    
    { TRIGGER_LEVELS_DOWN,            50,     1,          0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { TRIGGER_LEVELS_UP,              50,     1,          0,        0,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { LOOP_LEVELS_SWEEP_DOWN_INIT,    50,     1,          40,       0,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { LOOP_LEVELS_SWEEP_UP_INIT,      50,     1,          40,       0,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { LOOP_LEVELS_FADE_DOWN_INIT,     50,     1,          40,       1,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { LOOP_LEVELS_FADE_UP_INIT,       50,     1,          40,       1,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { SPIRAL_LEVELS_SWEEP_DOWN_INIT,  50,     1,          40,       0,            0,          0 },    //,            palette,  palette_i,  palette_j },
    
    { SPIRAL_LEVELS_SWEEP_UP_INIT,    50,     1,          40,       0,            0,          0 }    //,            palette,  palette_i,  palette_j }

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
    this->currentPreset_ = 0;
  };

  int get_weighted_random_preset() {
//    this->currentPreset_ = random8(NUM_PRESETS);
//    Serial.println(this->currentPreset_);

//    if(this->currentPreset_ == 0) {
//      this->currentPreset_ = 6;
//    } else {
//      this->currentPreset_ = 0;
//    }

    this->currentPreset_ = 15;

    return this->currentPreset_;
  };

};
#endif
