#ifndef LEDS_SNOWFALL
#define LEDS_SNOWFALL
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"
#include "runnable.h"
#include "colors.h"
#include "modes.h"


class LEDsSnowfall {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
  float *angle_state_;
  int speed_;
  int spacer_;
  int jitter_;
  
public:
  LEDsSnowfall(I2SClocklessLedDriver *driver, float *angle_state) {
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    angle_state_ = angle_state;
    speed_ = 1;
    spacer_ = 12;
    jitter_ = 4; 
    
  };

  void init() {
//    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
//    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    
    int rnd_offset = random(this->jitter_) - (this->jitter_/2);
    for (int ang=0; ang<360; ang++) {
      if ((ang%this->spacer_) == 0) {
        this->angle_state_[(ang+rnd_offset)%360] = float(NUM_LEVELS);
        Serial.println(ang);
        Serial.println((ang+rnd_offset)%360);
        Serial.println(angle_state_[(ang+rnd_offset)%360]);
        Serial.println(" ");
        rnd_offset = random(this->jitter_) - (this->jitter_/2);
      }
    }
  };

  void setup() {
    this->init();
  };

  void update_model() {

    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), 0);
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    
    for (int ang=0; ang<360; ang++) { 
      if ((this->angle_state_[ang] >= 0.5) && (random8() > 24)) {
        this->angle_state_[ang] -= 0.05 + (float(random8())/25600.0);
//        Serial.println(this->angle_state_[ang]);
      }
      if ((this->angle_state_[ang] < 0.5) && (this->angle_state_[ang] > 0.0)) {
        this->angle_state_[ang] = float(NUM_LEVELS);
//        this->angle_state_[ang] = 0.0;
//        this->angle_state_[(ang+rnd_offset)%360] = float(NUM_LEVELS);
//        rnd_offset = random(this->jitter_) - (this->jitter_/2);
      }
      
    }
  };
  
  void loop() {
    // sweep the angle-states array and render the current values as interpolated heights
    for (int ang=0; ang<360; ang++) {
      double curr_ang = this->angle_state_[ang];
//      Serial.println(curr_ang);
//      only operate on values 1.0 -> 12.0
      if (curr_ang > 0.99) {
        int val = int(floor(curr_ang)) - 1;
        int val_ = int(ceil(curr_ang)) - 1;
        double ratio = fmod(curr_ang, 1.0);
        double ratio_ = (1.0 - ratio);

        int side = 1 - (ang / 180);
        int lvl = val;
//        Serial.println("-------------");
//        Serial.println(val);
//        Serial.println(val_);
//        Serial.println(ratio);
//        Serial.println(ratio_);
//        Serial.println(side);
//        Serial.println(lvl);
//        Serial.println("-------------");
        
        int stripid = LEVELMAP[lvl][side][0];
        int theta_start = LEVELMAP[lvl][side][1];
        int theta_end = LEVELMAP[lvl][side][2];
        int seg_offset = LEVELMAP[lvl][side][4];
        int seg_length = LEVELMAP[lvl][side][5];
        int dir = LEVELMAP[lvl][side][6];
        
        int index = angle_to_index(ang, lvl, side, theta_start, theta_end, seg_offset, seg_length, dir);
        int j = (stripid * NUM_LEDS_PER_STRIP) + index;

        lvl = val_;
        stripid = LEVELMAP[lvl][side][0];
        theta_start = LEVELMAP[lvl][side][1];
        theta_end = LEVELMAP[lvl][side][2];
        seg_offset = LEVELMAP[lvl][side][4];
        seg_length = LEVELMAP[lvl][side][5];
        dir = LEVELMAP[lvl][side][6];
        
        index = angle_to_index(ang, lvl, side, theta_start, theta_end, seg_offset, seg_length, dir);

        int j_ = (stripid * NUM_LEDS_PER_STRIP) + index;

        this->currentHSV_ = CHSV(this->currentHSV_.h, this->currentHSV_.s, uint8_t(ratio_ * 256.0));
        hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
        this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);

        this->currentHSV_ = CHSV(this->currentHSV_.h, this->currentHSV_.s, uint8_t(ratio * 256.0));
        hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
        this->driver_->setPixel(j_, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);

//        Serial.println("----------------------------------------");
//        Serial.println(uint8_t(ratio * 256.0));
//        Serial.println(j);
//        Serial.println(uint8_t(ratio_ * 256.0));
//        Serial.println(j_);
      }
    }
    this->driver_->showPixels();
  };

};
#endif

//
//    //for (int ang; ang<360; ang++) {
//    this->angle_ = (this->angle_ + 1) % 360;
////    Serial.println(this->angle_);
//    for (int lvl=0; lvl<NUM_LEVELS; lvl++) {
//      int side = 1 - int(this->angle_ / 180);
//      int* octo = LEVELMAP[lvl][side];
////      Serial.print(lvl); Serial.print(" "); Serial.println(side);
////      Serial.print(octo[1]);Serial.print(" "); Serial.println(octo[2]);
////      Serial.println(" ");
//      if ((this->angle_ >= octo[1]) && (this->angle_ < octo[2])) {
//        
//        int theta_start = LEVELMAP[lvl][side][1];
//        int theta_end = LEVELMAP[lvl][side][2];
//        int stripid = LEVELMAP[lvl][side][0];
//        int seg_offset = LEVELMAP[lvl][side][4];
//        int seg_length = LEVELMAP[lvl][side][5];
//        int dir = LEVELMAP[lvl][side][6];
//        
//        int index = angle_to_index(this->angle_, lvl, side, theta_start, theta_end, seg_offset, seg_length, dir);
//
//        int j = (stripid * NUM_LEDS_PER_STRIP) + index;
//        
////        Serial.println(stripid);
////        Serial.println(index);
////        Serial.println(j);
////        Serial.println(" ");
//        
//        this->driver_->setPixel(j, this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
//      }
//    }
//    this->driver_->showPixels(); 
