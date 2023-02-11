#ifndef LEDS_LIGHT_SIDE
#define LEDS_LIGHT_SIDE
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"
#include "runnable.h"
#include "colors.h"
#include "modes.h"


class LEDsLightSide {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_;                 // standard temp color var conversion destination
  int speed;                        // speed of evolution of HSV color
  int currentSide_;                 // track which side we are on

public:
  LEDsLightSide(I2SClocklessLedDriver *driver) {      // int sides[2][8][5]
    driver_ = driver;
    // currentHSV_ = CHSV(0,0,0);
    // currentRGB_ = CRGB(0,0,0);
    // index = 0;
    speed = 1;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    currentSide_ = 0;
  };

  void init() {
    this->currentHSV_ = CHSV(beatsin8(3*this->speed,0,255), beatsin8(5*this->speed,120,240), beatsin8(7*this->speed,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    this->currentSide_ = 0;
  }

  void setup() {
    this->init();
  };

  void update_model() {
    // update model is called at the start of each main loop func call
    // calling rgb func here should cause rainbowish randomish effect
    // call it at the change of the side var to update color in sync with side changes
    this->currentSide_  = 1 - this->currentSide_;
    this->currentHSV_ = CHSV(beatsin8(3*this->speed,0,255), beatsin8(5*this->speed,120,240), beatsin8(7*this->speed,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );
    Serial.print(this->currentHSV_.hue); Serial.print(" "); Serial.print(this->currentHSV_.saturation); Serial.print(" "); Serial.println(this->currentHSV_.value); Serial.print(" "); 
    Serial.println(this->currentSide_);
    Serial.print(this->currentRGB_.r); Serial.print(" "); Serial.print(this->currentRGB_.g); Serial.print(" "); Serial.println(this->currentRGB_.b);
  };

  void loop() {
    
    // light-sides is a one-shot, so this loop is executed once when the whole side updates.
    // one-shot effects allow us to use conventional for-loops in the effect's loop() function
    
    for (int i=0; i<8; i++) {
      
      int r = SIDES[this->currentSide_][i][0];
      int o = SIDES[this->currentSide_][i][1];
      int l = SIDES[this->currentSide_][i][2];
      int d = SIDES[this->currentSide_][i][3];
      int lt = SIDES[this->currentSide_][i][4];
      
      int start = (r * 200) + o;
      
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
  }
};
#endif




// int speed = 1;


// void trigger_side() {

//   sd = (sd + 1) % 2;

//   CRGB temp = CRGB(0, 0, 0);
//   example_blue = CHSV(beatsin8(3*speed,0,255), beatsin8(5*speed,120,240), beatsin8(7*speed,48,200));
//   hsv2rgb_rainbow( example_blue, temp);

// //  Serial.print(example_blue.hue); Serial.print(" "); Serial.print(example_blue.saturation); Serial.print(" "); Serial.println(example_blue.value); Serial.print(" "); Serial.println(sd);
  
//   for (int i=0; i<8; i++) {
//     int r = sides[sd][i][0];
//     int o = sides[sd][i][1];
//     int l = sides[sd][i][2];
//     int d = sides[sd][i][3];
//     int lt = sides[sd][i][4];
//     int start = (r * 200) + o;
//     // Serial.print(r); Serial.print(" "); Serial.print(o); Serial.print(" "); Serial.print(l); Serial.print(" "); Serial.print(d); Serial.print("  - "); Serial.println(start);
//     // d can be negative!
//     if (d > 0) {  
// //      Serial.print("A: "); Serial.print(temp.r); Serial.print(" ");  Serial.print(temp.g); Serial.print(" ");  Serial.print(temp.b); Serial.print(" ");  Serial.print(start); Serial.print(" "); Serial.println(start+(d*l));
//       for (int j=start; j<(start+(d*l)); j++) {
//           driver->setPixel(j, temp.r, temp.g, temp.b); //, MASTER_BRIGHTNESS/4);
//       }
//     } else {
// //      Serial.print("B: "); Serial.print(temp.r); Serial.print(" ");  Serial.print(temp.g); Serial.print(" ");  Serial.print(temp.b); Serial.print(" ");  Serial.print(start+(d*l)); Serial.print(" "); Serial.println(start);
//       for (int j=start-1; j>=(start+(d*l)); j--) {
//         driver->setPixel(j, temp.r, temp.g, temp.b); //, MASTER_BRIGHTNESS/4);
//       }
//     }
//   }
// }
