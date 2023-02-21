#ifndef LEDS_FAKE_SPECTRA
#define LEDS_FAKE_SPECTRA
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsFakeSpectra {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
  int speed_;
  uint8_t lvls_[NUM_LEVELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t speeds_[NUM_LEVELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t extents_[NUM_LEVELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int speed_lo_;
  int speed_hi_;
  int extent_lo_;
  int extent_hi_;
  
public:
  LEDsFakeSpectra(I2SClocklessLedDriver *driver) {
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    speed_ = 1;
    int speed_lo_ = 32;
    int speed_hi_ = 156;
    int extent_lo_ = 0;
    int extent_hi_ = 256;
    for (int ll=0; ll<NUM_LEVELS; ll++) {
      lvls_[ll] = 0;
      speeds_[ll] = 0;
      extents_[ll] = 0;
    }
  };

  void init() {

    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);

    this->speed_ = 1;
    for (int ll=0; ll<NUM_LEVELS; ll++) {
      this->lvls_[ll] = 0;
      this->speeds_[ll] = random8(this->speed_lo_,this->speed_hi_);
      this->extents_[ll] = random8(this->extent_lo_,this->extent_hi_);
    }
  }

  void setup() {
    this->init();
  };

  void update_model() {
    
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );

    for (int ll=0; ll<NUM_LEVELS; ll++) {
      this->lvls_[ll] = beatsin8(this->speeds_[ll], 0, this->extents_[ll]);
    }
    
  };

  void loop() {

    for (int sd=0; sd<2; sd++) {
      for (int ll=0; ll<NUM_LEVELS; ll++) {
        if (this->lvls_[ll] > 0) {
          
          int r = LEVELS[ll][sd][0];
          int o = LEVELS[ll][sd][1];
          int l = LEVELS[ll][sd][2];
          int d = LEVELS[ll][sd][3];
          int lt = LEVELS[ll][sd][4];
          int start = (r * NUM_LEDS_PER_STRIP) + o;
  
  
          if (d > 0) {
  
            int val = start + int((float(this->lvls_[ll]) / 256.0) * float(l));
            
            for (int j=start; j<val; j++) {
              this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
            }

            for (int j=val; j<(start+(d*l)); j++) {
              this->driver_->setPixel(j, 0, 0, 0);
            }
            
      
          } else {
  
            int val = start - 1 - int((float(this->lvls_[ll]) / 256.0) * float(l));
  
            for (int j=start-1; j>=val; j--) {
              this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
            }

            for (int j=val; j>=(start+(d*l)); j--) {
              this->driver_->setPixel(j, 0, 0, 0);
            }
  
          }
        }
      }
    }
    this->driver_->showPixels();
  };

};
#endif
