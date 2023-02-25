#ifndef LEDS_ROTATE_BANDS
#define LEDS_ROTATE_BANDS
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsRotateBands {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
//  uint8_t currentSide_;
  int counter_;
  uint8_t mirror_;
  int angle_;
  
public:
  int speed_;
  int num_bands_;
  int dir_;
  int angle_offset_;
  int hueSpeed_;
  
  LEDsRotateBands(I2SClocklessLedDriver *driver) {      // int sides[2][NUMSTRIPS][5]
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    speed_ = 1;
    mirror_ = 2;
    angle_ = 0;
    num_bands_ = 11;
    dir_ = 1;
    angle_offset_ = 1;
    hueSpeed_ = 3;
  };

  void init() {
    this->angle_ = 0;
    this->num_bands_ = 11;
    this->dir_ = 1;
    this->angle_offset_ = 1;
    this->hueSpeed_ = 3;
  };

  void draw_at_angle(int angle, CRGB currentRGB) {
    for (int lvl=0; lvl<NUM_LEVELS; lvl++) {
      int side = 1 - int(angle / 180);
      int* octo = LEVELMAP[lvl][side];
//      Serial.println("--");
//      Serial.print(lvl); Serial.print(" "); Serial.print(side); Serial.print(" "); Serial.print(octo[1]); Serial.print(" "); Serial.println(octo[2]);
      if ((angle >= octo[1]) && (angle < octo[2])) {
        
        int theta_start = LEVELMAP[lvl][side][1];
        int theta_end = LEVELMAP[lvl][side][2];
        int stripid = LEVELMAP[lvl][side][0];
        int seg_offset = LEVELMAP[lvl][side][4];
        int seg_length = LEVELMAP[lvl][side][5];
        int dir = LEVELMAP[lvl][side][6];
        
        int index = angle_to_index(angle, lvl, side, theta_start, theta_end, seg_offset, seg_length, dir);

        int j = (stripid * NUM_LEDS_PER_STRIP) + index;
//          Serial.println("++++"); Serial.print(stripid); Serial.print(" "); Serial.print(index); Serial.print(" "); Serial.println(j);
        
        this->driver_->setPixel(j, currentRGB.r, currentRGB.g, currentRGB.b);
      }
    }
  };

  void loop(LEDsPaletteController paletteCtl, int index) {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes

        
    if (index < 14) {
      uint8_t hue_ = beatsin8(this->hueSpeed_, 0, 255);
      this->currentHSV_ = ColorFromPalette( paletteCtl.currentPalette_, hue_, 128, paletteCtl.currentBlending_);
      this->currentHSV_ = CHSV(this->currentHSV_.h, MIN((this->currentHSV_.s + random8(8)), 255), this->currentHSV_.v);
    } else {
      this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    }

    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );

    //for (int ang; ang<360; ang++) {
    this->angle_ = (this->angle_ + this->angle_offset_) % 360;
    
    int angle__ = this->angle_;
    if (this->dir_ < 0) {
      angle_ = 360 - this->angle_;
    }
//    Serial.println(this->angle_);
    
    for (int a=0; a<this->num_bands_; a++) {
      if ((a % 2) == 0) {
        this->draw_at_angle(((angle__ + (a * (360 / max(this->num_bands_, 1)))) % 360) , this->currentRGB_);
      } else {
        this->draw_at_angle(((angle__ + (a * (360 / max(this->num_bands_, 1)))) % 360) , CRGB::Gray);
      }
    }

    this->driver_->showPixels(); 
    
//     Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
//     Serial.println(this->currentSide_);
//     Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };
};

#endif
