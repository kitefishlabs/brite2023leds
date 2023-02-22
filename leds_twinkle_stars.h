#ifndef LEDS_TWINKLE_STARS
#define LEDS_TWINKLE_STARS
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsTwinkleStars {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_;                 // standard temp color var conversion destination
  int speed_;                       // speed of evolution of HSV color
//  int currentSide_;                 // track which side we are on
  uint8_t *leds_state_;
  uint8_t *leds_speed_state_;

public:
  int density_;
  int speed_lo_;
  int speed_hi_;
  
  LEDsTwinkleStars(I2SClocklessLedDriver *driver, uint8_t *states, uint8_t *speeds ) {
    driver_ = driver;
    currentHSV_ = CHSV(BLUE,200,240);
    currentRGB_ = CRGB(0,0,0);
    // index = 0;
    speed_ = 1;    
    leds_state_ = states;
    leds_speed_state_ = speeds;
    density_ = 10;               // (density_ / 1000)
    speed_lo_ = 3;
    speed_hi_ = 23;
  };

  void init() {
//    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    
    for (int i=0; i< TOTAL_LEDS; i++) {
      if (random(1000) < this->density_) {
        this->leds_state_[i] = 1;
        this->leds_speed_state_[i] = random8(this->speed_lo_, this->speed_hi_);
        Serial.print(i); Serial.print(" "); Serial.print(leds_state_[i]); Serial.print(" "); Serial.println(leds_speed_state_[i]);
      }
    }
  };

  void setup() {
    this->init();
  };

  void update_model() {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes
//
//    for (int i=0; i<TOTAL_LEDS; i++) {
//      if (this->leds_state_[i] > 0) {
//        this->leds_state_[i] = beatsin8(this->leds_speed_state_[i], 10, 240);
////        if (i<30) {
////          Serial.print(i); Serial.print(" "); Serial.print(leds_state_[i]); Serial.print(" "); Serial.println(leds_speed_state_[i]);
////        }
//      }
//    }

//    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
//    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
//     Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//     Serial.println(this->currentSide_);
//     Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  void loop() {
    
    // light-sides is a one-shot, so this loop is executed once when the whole side updates.
    // one-shot effects allow us to use conventional for-loops in the effect's loop() function

    for (int i=0; i<TOTAL_LEDS; i++) {
      if (this->leds_state_[i] > 0) {
        this->leds_state_[i] = beatsin8(this->leds_speed_state_[i], 10, 240);
//        if (i<30) {
//          Serial.print(i); Serial.print(" "); Serial.print(leds_state_[i]); Serial.print(" "); Serial.println(leds_speed_state_[i]);
//        }
        this->currentHSV_ = CHSV(this->currentHSV_.h , this->currentHSV_.s , this->leds_state_[i]);
        hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
        this->driver_->setPixel(i, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
      }
    }
//
//    for (int i=0; i<NUM_LEVELS; i++) {
//      for (int s=0; s<2; s++) {
//        
//        int r = SIDES[s][i][0];
//        int o = SIDES[s][i][1];
//        int l = SIDES[s][i][2];
//        int d = SIDES[s][i][3];
//        int lt = SIDES[s][i][4];
//        
//        int start = (r * NUM_LEDS_PER_STRIP) + o;
//        
//        if (d > 0) {
//  
//          for (int j=start; j<(start+(d*l)); j++) {
//  
//            this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
//  
//          }
//        
//        } else {
//  
//          for (int j=start-1; j>=(start+(d*l)); j--) {
//  
//            this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
//  
//          }
//        }
//      }
//    }
    this->driver_->showPixels(); 
  };
};
#endif
