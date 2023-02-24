#ifndef LEDS_TRIGGER_LEVEL
#define LEDS_TRIGGER_LEVEL
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsTriggerLevel {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;
  CRGB currentRGB_;
  uint8_t currentLevel_;
  uint8_t currentSide_;
  uint8_t mirror_;

public:
  int speed_;

  LEDsTriggerLevel(I2SClocklessLedDriver *driver) {
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    currentLevel_ = 0;
    currentSide_ = 0;
    speed_ = 1;
    mirror_ = 2;
  };

  void init() {
    this->currentLevel_ = 0;
    this->currentSide_ = 0;
    this->mirror_ = 2;
  };
  
  void update_model(int dir) {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes
//    if (this->currentLevel_ == 0) {
//      this->currentLevel_  = NUM_LEVELS - 1;
//    } else {
    this->currentLevel_  = (this->currentLevel_ + dir + NUM_LEVELS) % NUM_LEVELS;
//    }
    
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
    //  Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
    //  Serial.println(this->currentSide_);
    //  Serial.println(this->currentLevel_);
    //  Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  void light_level() {

    Serial.print("light lvl: ");Serial.print(this->currentSide_); Serial.print(" "); Serial.println(this->currentLevel_);
      
    int r = LEVELS[this->currentLevel_][this->currentSide_][0];
    int o = LEVELS[this->currentLevel_][this->currentSide_][1];
    int l = LEVELS[this->currentLevel_][this->currentSide_][2];
    int d = LEVELS[this->currentLevel_][this->currentSide_][3];
    int lt = LEVELS[this->currentLevel_][this->currentSide_][4];
    
    int start = (r * NUM_LEDS_PER_STRIP) + o;

    // Serial.print(r); Serial.print(" ");
    // Serial.print(o); Serial.print(" ");
    // Serial.print(l); Serial.print(" ");
    // Serial.print(d); Serial.print(" ");
    // Serial.print(lt); Serial.print(" ");
    // Serial.println(start);
    
    if (d > 0) {

      for (int j=start; j<(start+(d*l)); j++) {

        this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
//        Serial.print("POSITIVE DIRECTION: "); Serial.print(" "); Serial.print(this->currentRGB_.r); Serial.print(" ");Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
      }
    
    } else {

      for (int j=start-1; j>=(start+(d*l)); j--) {

        this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
//        Serial.print("NEGATIVE DIRECTION: "); Serial.print(" "); Serial.print(this->currentRGB_.r); Serial.print(" ");Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
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
    
    this->driver_->showPixels();
  };

};
#endif
