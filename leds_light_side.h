#ifndef LEDS_LIGHT_SIDE
#define LEDS_LIGHT_SIDE
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsLightSide {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_;                 // standard temp color var conversion destination
  int currentSide_;                 // track which side we are on


public:
  int speed_;                       // speed of evolution of HSV color
  uint8_t mirror_;

  LEDsLightSide(I2SClocklessLedDriver *driver) {      // int sides[2][NUMSTRIPS][5]
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    speed_ = 1;
    currentSide_ = 0;
    mirror_ = 3;
  };

  void init() {
//    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
//    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);

//    this->currentRGB_ = ColorFromPalette( paletteCtl.currentPalette_, 1, 128, paletteCtl.currentBlending_);

    this->currentSide_ = 0;
  };

  void update_model(LEDsPaletteController paletteCtl, int index) {

    if (index < 14) {
      int hueIndexer = beatsin8(min(max(speed_, 0), 15), 0, 255);
//      Serial.println(hueIndexer);
      this->currentHSV_ = ColorFromPalette( paletteCtl.currentPalette_, hueIndexer, 128, paletteCtl.currentBlending_);
    } else {
      this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    }
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );

//     Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//     Serial.println(this->currentSide_);
//     Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };


  void light_side() {

    int r = 0;
    int o = 0;
    int l = 0;
    int d = 0;
    int lt = 0;
    int start = 0;

    for (int i=0; i<NUM_LEVELS; i++) {
      
      r = SIDES[this->currentSide_][i][0];
      o = SIDES[this->currentSide_][i][1];
      l = SIDES[this->currentSide_][i][2];
      d = SIDES[this->currentSide_][i][3];
      lt = SIDES[this->currentSide_][i][4];
      start = (r * NUM_LEDS_PER_STRIP) + o;
      
      if (d > 0) {

        for (int j=start; j<(start+(d*l)); j++) {

          this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);

        }
      
      } else {

        for (int j=start-1; j>=(start+(d*l)); j--) {

          this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);

        }
      }
    }
    this->driver_->showPixels();
  };

  void loop() {
    
    // light-sides is a one-shot, so this loop is executed once when the whole side updates.
    // one-shot effects allow us to use conventional for-loops in the effect's loop() function
    
    if (this->mirror_ == 0) {
    
      this->currentSide_ = 0;
      this->light_side();
    
    } else if (this->mirror_ == 1) {

      this->currentSide_ = 1;
      this->light_side();
    
    } else if (this->mirror_ == 2) {
      
      for (int i=0; i<2; i++) {
        this->currentSide_ = i;
        this->light_side();
      }

    } else if (this->mirror_ == 3) {
      this->currentSide_ = 1 - this->currentSide_;
      this->light_side();
    }
    
    this->driver_->showPixels();
  };

};
#endif
