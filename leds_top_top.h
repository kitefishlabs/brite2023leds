#ifndef LEDS_TOP_TOP
#define LEDS_TOP_TOP
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"
#include "colors.h"
#include "modes.h"


class LEDsTopTop {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_;                 // standard temp color var conversion destination

public:
  int speed_;                       // speed of evolution of HSV color

  LEDsTopTop(I2SClocklessLedDriver *driver) {      // int sides[2][NUMSTRIPS][5]
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    speed_ = 1;
  };

  void init() {
//    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
//    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);

//    this->currentRGB_ = ColorFromPalette( paletteCtl.currentPalette_, 1, 128, paletteCtl.currentBlending_);

  };

  void update_model(LEDsPaletteController paletteCtl, int index) {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes

//    Serial.print("index: "); Serial.print(index); Serial.print(", R: "); Serial.print(this->currentRGB_.r); Serial.print(", G: "); Serial.print(this->currentRGB_.g); Serial.print(", B: "); Serial.println(this->currentRGB_.b);
    this->currentHSV_ = ColorFromPalette( paletteCtl.currentPalette_, index, 128, paletteCtl.currentBlending_);
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );

//     Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//     Serial.println(this->currentSide_);
//     Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  void loop() {
    
    // light-sides is a one-shot, so this loop is executed once when the whole side updates.
    // one-shot effects allow us to use conventional for-loops in the effect's loop() function

        for (int j=0; j<PW[12]; j++) {
//          hard-wiring this one
          this->driver_->setPixel((7 * NUM_LEDS_PER_STRIP) + j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);

        }
    this->driver_->showPixels(); 
  };
};
#endif
