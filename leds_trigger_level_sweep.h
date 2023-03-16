#ifndef LEDS_TRIGGER_LEVEL_SWEEP
#define LEDS_TRIGGER_LEVEL_SWEEP
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsTriggerLevelSweep {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
  uint8_t currentLevel_;
  uint8_t currentSide_;
  int counter_;
  uint8_t mirror_;
  int lvls_[NUM_LEVELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
public:
  int speed_;
  int offset_;
  uint8_t hueSpeed_;
  
  LEDsTriggerLevelSweep(I2SClocklessLedDriver *driver) {
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    currentLevel_ = 0;
    currentSide_ = 0;
    speed_ = 1;
    counter_ = 0;
    mirror_ = 2;
    offset_ = 40;
    hueSpeed_ = 2;
  };

  void init(int dir) {
    
    if (dir > 0) {
      this->currentLevel_ = 0;  
    } else {
      this->currentLevel_ = NUM_LEVELS - 1;  
    }
    
    this->currentSide_ = 0;
    this->speed_ = 1;
    this->counter_ = 0;
    this->mirror_ = 2;
    this->offset_ = 40;
    for (int i=0; i<NUM_LEVELS; i++) {
      this->lvls_[i] = 0;
    }
    // Serial.println("INIT");
    // for (int i=0; i<NUM_LEVELS; i++) {
    //   Serial.println(this->lvls_[i]);
    // }
  }

  void update_model(int dir, LEDsPaletteController paletteCtl, int index) {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes
    

//    Serial.print("counter: "); Serial.println(this->counter_);
//    Serial.print("cur lvl: "); Serial.println(this->currentLevel_);
//    Serial.print("lvl val: "); Serial.println(this->lvls_[((this->currentLevel_+dir)%NUM_LEVELS)]);
//    for (int i=0; i<NUM_LEVELS; i++) {
//      Serial.print(this->lvls_[i]); Serial.print(" ");
//    }
//    Serial.println("");

    if (((this->counter_ % this->offset_) == 0) && (this->lvls_[this->currentLevel_] == 0)) {
      
//       Serial.print("LVL: ");  Serial.println(this->currentLevel_);
      if (this->currentLevel_ == 0) {
        this->currentSide_ = 1 - this->currentSide_;
//        Serial.print("lvl == 0 detected sd="); Serial.println(this->currentSide_);
      }
      this->lvls_[this->currentLevel_] = 1;
      this->currentLevel_ = (this->currentLevel_ + dir + NUM_LEVELS) % NUM_LEVELS;
    }

    if (index < 14) {
      uint8_t hue_ = beatsin8(this->hueSpeed_, 0, 255);
      this->currentHSV_ = ColorFromPalette( paletteCtl.currentPalette_, hue_, 128, paletteCtl.currentBlending_);
      this->currentHSV_ = CHSV(this->currentHSV_.h, MIN((this->currentHSV_.s + random8(8)), 255), this->currentHSV_.v);
    } else {
      this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    }
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
    
    // Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//     Serial.println(this->currentSide_);
//     Serial.println(this->currentLevel_);
    // Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  void light_level() {

    int r = 0;
    int o = 0;
    int l = 0;
    int d = 0;
    int lt = 0;
    int start = 0;
    int j = 0;
  
//     Serial.print("lvl: "); Serial.println(this->currentLevel_);
    for (int ll = 0; ll < NUM_LEVELS; ll++) {
       
      if (this->lvls_[ll] > 0) {

//        Serial.print("LL: "); Serial.println(ll);  
        
        r = LEVELS[ll][this->currentSide_][0];
        o = LEVELS[ll][this->currentSide_][1];
        l = LEVELS[ll][this->currentSide_][2];
        d = LEVELS[ll][this->currentSide_][3];
        lt = LEVELS[ll][this->currentSide_][4];
        start = (r * NUM_LEDS_PER_STRIP) + o;
        
        if (d>0) {
        
          j = start + this->lvls_[ll];

//          Serial.print("j: "); Serial.print(j); Serial.print(", r: "), Serial.print(this->currentRGB_.r), Serial.print(", g: "), Serial.print(this->currentRGB_.g), Serial.print(", b: "), Serial.println(this->currentRGB_.b);

          this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
          if (j >= start+(d*l)) {
            this->lvls_[ll] = 0;
//             Serial.print("LL----------------------------->0 (++) "); Serial.println(ll);
          } else {
            this->lvls_[ll] = (this->lvls_[ll] + 1);
          }
        
        } else {
        
          j = start - 1 - this->lvls_[ll];
          this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
          if (j <= (start+(d*l))) {
            this->lvls_[ll] = 0;
//             Serial.print("LL-------------------------->0 (--) "); Serial.println(ll); 
          } else {
            this->lvls_[ll] = (this->lvls_[ll] + 1);
          }
        }
      }
    }
  };
  
  void loop() {
    
    if (this->mirror_ == 0) {
    
      this->currentSide_ = 0;
      this->light_level();
    
    } else if (this->mirror_ == 1) {

      this->currentSide_ = 1;
      this->light_level();
    
    } else if (this->mirror_ == 2) {
      
      for (int i=0; i<2; i++) {
        this->currentSide_ = i;
        this->light_level();
      }

    } else if (this->mirror_ == 3) {
      this->currentSide_ = 1 - this->currentSide_;
      this->light_level();
    }
    this->counter_ = (this->counter_ + 1) % 1000000;
    this->driver_->showPixels();
  };

};

#endif
