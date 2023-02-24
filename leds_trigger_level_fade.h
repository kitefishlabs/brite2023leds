#ifndef LEDS_TRIGGER_LEVEL_FADE
#define LEDS_TRIGGER_LEVEL_FADE
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsTriggerLevelFade {

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
  int pulse_speed_;
  
  LEDsTriggerLevelFade(I2SClocklessLedDriver *driver) {
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    currentLevel_ = 0;
    currentSide_ = 0;
    pulse_speed_ = 1;
    speed_ = 1;
    counter_ = 0;
    mirror_ = 2;
    offset_ = 40;
    for (int i=0; i<NUM_LEVELS; i++) {
      lvls_[i] = 0;
    }
  };

  void init(int dir) {
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), 0);
    
    if (dir > 0) {
      this->currentLevel_ = 0;  
    } else {
      this->currentLevel_ = NUM_LEVELS - 1;
    }
    
    this->currentSide_ = 0;
    this->pulse_speed_ = 5;
    this->speed_ = 2;
    this->counter_ = 0;
    this->mirror_ = 2;
    this->offset_ = 25;
    for (int i=0; i<NUM_LEVELS; i++) {
      this->lvls_[i] = 0;
    }
    Serial.println("INIT");
    for (int i=0; i<NUM_LEVELS; i++) {
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
        
//    Serial.print("counter: "); Serial.println(this->counter_);
//    Serial.print("cur lvl: "); Serial.println(this->currentLevel_);
//    Serial.print("lvl val: "); Serial.println(this->lvls_[((this->currentLevel_+dir)%NUM_LEVELS)]);
//    for (int i=0; i<NUM_LEVELS; i++) {
//      Serial.println(this->lvls_[i]);
//    }

    if (((this->counter_ % this->offset_) == 0) && ((this->lvls_[this->currentLevel_] == 0) || (this->lvls_[this->currentLevel_] == 256))) {
      
//      Serial.print("LVL: ");  Serial.println(this->currentLevel_);
      if (this->currentLevel_ == 0) {
        this->currentSide_ = 1 - this->currentSide_;
//        Serial.print("lvl == 0 detected sd="); Serial.println(this->currentSide_);
      }
      this->lvls_[this->currentLevel_] = 1;
      if ((this->currentLevel_ + dir) < 0) {
        this->currentLevel_ = NUM_LEVELS - 1;
      } else {
        this->currentLevel_ = (this->currentLevel_ + dir) % NUM_LEVELS;
      }
    }
    
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), 0);

    
//    Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//    Serial.println(this->currentSide_);
//    Serial.println(this->currentLevel_);
//    Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  uint8_t attackDecayWave8( uint8_t i) {
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

  void light_level() {

//    Serial.print("lvl: "); Serial.println(this->currentLevel_);
    for (int ll = 0; ll < NUM_LEVELS; ll++) {
//      Serial.print("LL: "); Serial.print(ll);  Serial.print(" "); 
      if (this->lvls_[ll] > 0) {
        
        int r = LEVELS[ll][this->currentSide_][0];
        int o = LEVELS[ll][this->currentSide_][1];
        int l = LEVELS[ll][this->currentSide_][2];
        int d = LEVELS[ll][this->currentSide_][3];
        int lt = LEVELS[ll][this->currentSide_][4];
        int start = (r * NUM_LEDS_PER_STRIP) + o;

        uint8_t v_ = attackDecayWave8(MIN(this->lvls_[ll], 256));

//        Serial.print("   V_: "); Serial.println(v_);
        
        if (d>0) {

          for (int j=start; j<(start+(d*l)); j++) {
            
            this->currentHSV_ = CHSV(this->currentHSV_.h, this->currentHSV_.s, v_);
            hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
            this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
          }
          if (this->lvls_[ll] > 255) {
            this->lvls_[ll] = 256;
          } else {
            this->lvls_[ll] = MIN(this->lvls_[ll] + this->pulse_speed_, 256);
          }
        
        } else {
        
          for (int j=start-1; j>=(start+(d*l)); j--) {
            
            this->currentHSV_ = CHSV(this->currentHSV_.h, this->currentHSV_.s, v_);
            hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
            this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
          }
          if (this->lvls_[ll] > 255) {
            this->lvls_[ll] = 256;
          } else {
            this->lvls_[ll] = MIN(this->lvls_[ll] + this->pulse_speed_, 256);
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
    this->counter_ = (this->counter_ + 1) % 10000;
    this->driver_->showPixels();
  };

};

#endif
