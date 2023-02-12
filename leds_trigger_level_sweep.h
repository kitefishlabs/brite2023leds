#ifndef LEDS_TRIGGER_LEVEL_SWEEP
#define LEDS_TRIGGER_LEVEL_SWEEP
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"
#include "runnable.h"
#include "colors.h"
#include "modes.h"


class LEDsTriggerLevelSweep {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
  uint8_t currentLevel_;
  uint8_t currentSide_;
  int speed_;
  int counter_;
  uint8_t subMode_;
  int offset_;
  int lvls_[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  
public:
  LEDsTriggerLevelSweep(I2SClocklessLedDriver *driver) {
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    currentLevel_ = 0;
    currentSide_ = 0;
    speed_ = 1;
    counter_ = 0;
    subMode_ = 2;
    offset_ = 40;
    for (int i=0; i<8; i++) {
      lvls_[i] = 0;
    }
  };

  void init(int dir) {
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    
    if (dir > 0) {
      this->currentLevel_ = 0;  
    } else {
      this->currentLevel_ = 0;  
    }
    
    this->currentSide_ = 0;
    this->speed_ = 1;
    this->counter_ = 0;
    this->subMode_ = 2;
    this->offset_ = 40;
    for (int i=0; i<8; i++) {
      this->lvls_[i] = 0;
    }
    Serial.println("INIT");
    for (int i=0; i<8; i++) {
      Serial.println(this->lvls_[i]);
    }
  }

  void setup() {
    this->init(1);
  };


  void update_model(int dir) {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes
    
    
    Serial.print("counter: "); Serial.println(this->counter_);
    Serial.print("cur lvl: "); Serial.println(this->currentLevel_);
    Serial.print("lvl val: "); Serial.println(this->lvls_[((this->currentLevel_+dir)%8)]);
    for (int i=0; i<8; i++) {
      Serial.println(this->lvls_[i]);
    }

    if (((this->counter_ % this->offset_) == 0) && (this->lvls_[this->currentLevel_] == 0)) {
      
      Serial.print("LVL: ");  Serial.println(this->currentLevel_);
      if (this->currentLevel_ == 0) {
        this->currentSide_ = 1 - this->currentSide_;
        Serial.print("lvl == 0 detected sd="); Serial.println(this->currentSide_);
      }
      this->lvls_[this->currentLevel_] = 1;
      if ((this->currentLevel_ + dir) < 0) {
        this->currentLevel_ = 7;
      } else {
        this->currentLevel_ = (this->currentLevel_ + dir) % 8;
      }
    }
    
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
    Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
    Serial.println(this->currentSide_);
    Serial.println(this->currentLevel_);
    Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  void loop(int dir) {

    Serial.print("lvl: "); Serial.println(this->currentLevel_);
    for (int ll = 0; ll < 8; ll++) {
      Serial.print("LL: "); Serial.println(ll);  
      if (this->lvls_[ll] > 0) {
        
        int r = LEVELS[ll][this->currentSide_][0];
        int o = LEVELS[ll][this->currentSide_][1];
        int l = LEVELS[ll][this->currentSide_][2];
        int d = LEVELS[ll][this->currentSide_][3];
        int lt = LEVELS[ll][this->currentSide_][4];
        int start = (r * 200) + o;
        
        if (d>0) {
        
          int j = start + this->lvls_[ll];
          this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
          if (j >= start+(d*l)) {
            this->lvls_[ll] = 0;
            Serial.print("LL----------------------------->0 (++) "); Serial.println(ll); 
          } else {
            this->lvls_[ll] = (this->lvls_[ll] + 1);
          }
        
        } else {
        
          int j = start - 1 - this->lvls_[ll];
          this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
          if (j <= (start+(d*l))) {
            this->lvls_[ll] = 0;
            Serial.print("LL-------------------------->0 (--) "); Serial.println(ll); 
          } else {
            this->lvls_[ll] = (this->lvls_[ll] + 1);
          }
        }
      }
    }
    this->counter_ = (this->counter_ + 1) % 10000;
    this->driver_->showPixels();
  };
};

#endif

// //void trigger_level_sweep(int dir) {
// //
// //  CRGB temp = CRGB(0, 0, 0);
// //  example_blue = CHSV(beatsin8(3*speed,0,255), beatsin8(5*speed,120,240), beatsin8(7*speed,48,200));
// //  hsv2rgb_rainbow( example_blue, temp);
// //
// ////  Serial.print(example_blue.hue); Serial.print(" "); Serial.print(example_blue.saturation); Serial.print(" "); Serial.println(example_blue.value); Serial.print(" "); Serial.println(sd);
// //
// ////  every tenth tick, look ahead to see if next level is zeroed out (if not zeroed out, we wait)
// //  Serial.print("counter = "); Serial.print(counter); Serial.print(" "); Serial.println(((lvl+1)%8));
// //  if (((counter % 10) == 0) && (lvls[((lvl+1)%8)] == 0)) {
// //    lvl = (lvl + dir) % 8;
// //    if (lvl == 0) {
// //      sd = 1 - sd;
// //      Serial.print("lvl == 0 detected sd="); Serial.println(sd);
// //    }
// //    Serial.print("LVL: ");  Serial.println(lvl);
// //    lvls[lvl] = 1;
// //  }
// //  Serial.print("lvl: ");  Serial.println(lvl);
// //  for (int ll = 0; ll < 8; ll++) {
// //    Serial.println(ll);  
// //    if (lvls[ll] >= 0) {
// //      
// //      int r = sides[sd][ll][0];
// //      int o = sides[sd][ll][1];
// //      int l = sides[sd][ll][2];
// //      int d = sides[sd][ll][3];
// //      int lt = sides[sd][ll][4];
// //      int start = (r * 200) + o;
// //      
// //      if (d>0) {
// //      
// //        int j = start + lvls[ll];
// //        driver->setPixel(j, temp.r, temp.g, temp.b);
// //        if (j >= start+(d*l)) {
// //          lvls[ll] = 0;
// //          Serial.print("LL----------------------------->0 (++) "); Serial.println(ll); 
// //        } else {
// //          lvls[ll] = lvls[ll] + 1;
// //        }
// //      
// //      } else {
// //      
// //        int j = start - 1 - lvls[ll];
// //        driver->setPixel(j, temp.r, temp.g, temp.b);
// //        if (j <= (start+(d*l))) {
// //          lvls[ll] = 0;
// //          Serial.print("LL-------------------------->0 (--) "); Serial.println(ll); 
// //        } else {
// //          lvls[ll] = lvls[ll] + 1;
// //        }
// //      }
// //    }
// //  }
// //  counter = (counter + 1) % 10000;
// ////  delay(2);
// //}
