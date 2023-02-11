// #ifndef LEDS_TRIGGER_LEVEL
// #define LEDS_TRIGGER_LEVEL
// #include <FastLED.h>
// #include "I2SClocklessLedDriver.h"

// #include "common.h"
// #include "runnable.h"
// #include "colors.h"
// #include "modes.h"


// class LEDsTriggerLevel {

// private:
//   I2SClocklessLedDriver* driver_;
//   CHSV currentHue_;
//   CRGB currentRGB_;
//   int currentLevel_;
  

// public:
//   LEDsChasersTest(I2SClocklessLedDriver *driver) {
//     driver_ = driver;
//     // currentHue_ = CHSV(0,0,0);
//     // currentRGB_ = CRGB(0,0,0);
//     currentLevel_ = 0;
    
//   };

//   void init() {
//     this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
//     hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
//     this->currentLevel_ = 0;
//   }

//   void setup() {
//     this->init();
//   };

//   void loop() {


//    if (random(1000) < this->prob) {
//      this->currentHue_ = CHSV(this->currentHue_.h + uint8_t(random(12)), this->currentHue_.s + uint8_t(random(32)), this->currentHue_.v + uint8_t(random(5)) );
//      hsv2rgb_rainbow( this->currentHue_, this->currentRGB_);
   
//      Serial.print(this->currentHue_.h); Serial.print(" ");
//      Serial.print(this->currentHue_.s); Serial.print(" ");
//      Serial.print(this->currentHue_.v); Serial.print(" ");
//      Serial.println("");
//    }
   
//    for (int j = 0; j < NUMSTRIPS; j++) {
//      driver_->setPixel(((j * NUM_LEDS_PER_STRIP) + this->index) % (NUMSTRIPS * NUM_LEDS_PER_STRIP), this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b, MASTER_BRIGHTNESS);
//      driver_->setBrightness(MASTER_BRIGHTNESS);
//    }
//    driver_->showPixels(); 
//  }
// };
// #endif




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
