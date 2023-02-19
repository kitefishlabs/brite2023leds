#ifndef LEDS_ROTATE_BANDS
#define LEDS_ROTATE_BANDS
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"
#include "runnable.h"
#include "colors.h"
#include "modes.h"


class LEDsRotateBands {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
//  uint8_t currentSide_;
  int speed_;
  int counter_;
  uint8_t subMode_;
  int angle_;
  
public:
  LEDsRotateBands(I2SClocklessLedDriver *driver) {      // int sides[2][NUMSTRIPS][5]
    driver_ = driver;
     currentHSV_ = CHSV(0,0,0);
     currentRGB_ = CRGB(0,0,0);
    // index = 0;
    speed_ = 1;
//    currentSide_ = 0;
    subMode_ = 2;
    angle_ = 0;
  };


  void init() {
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    this->angle_ = 0;
  };

  void setup() {
    this->init();
  };

  void loop() {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes
    
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );

    //for (int ang; ang<360; ang++) {
    this->angle_ = (this->angle_ + 1) % 360;
//    Serial.println(this->angle_);
    for (int lvl=0; lvl<NUM_LEVELS; lvl++) {
      int side = 1 - int(this->angle_ / 180);
      int* octo = LEVELMAP[lvl][side];
//      Serial.print(lvl); Serial.print(" "); Serial.println(side);
//      Serial.print(octo[1]);Serial.print(" "); Serial.println(octo[2]);
//      Serial.println(" ");
      if ((this->angle_ >= octo[1]) && (this->angle_ < octo[2])) {
        
        int theta_start = LEVELMAP[lvl][side][1];
        int theta_end = LEVELMAP[lvl][side][2];
        int stripid = LEVELMAP[lvl][side][0];
        int seg_offset = LEVELMAP[lvl][side][4];
        int seg_length = LEVELMAP[lvl][side][5];
        int dir = LEVELMAP[lvl][side][6];
        
        int index = angle_to_index(this->angle_, lvl, side, theta_start, theta_end, seg_offset, seg_length, dir);

        int j = (stripid * NUM_LEDS_PER_STRIP) + index;
        
//        Serial.println(stripid);
//        Serial.println(index);
//        Serial.println(j);
//        Serial.println(" ");
        
        this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
      }
    }
    this->driver_->showPixels(); 
    
//     Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//     Serial.println(this->currentSide_);
//     Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };
};

#endif
