#ifndef LEDS_CHASERS_TEST
#define LEDS_CHASERS_TEST
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"
#include "runnable.h"
#include "colors.h"
#include "modes.h"


class LEDsChasersTest {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHue_;
  CRGB currentRGB_;
  int index;
  int prob;
  int strip;

public:
  LEDsChasersTest(I2SClocklessLedDriver *driver) {
    driver_ = driver;
    // currentHue_ = CHSV(0,0,0);
    // currentRGB_ = CRGB(0,0,0);
    index = 0;
    prob = 200;
    strip = 0;
  };

  void setup() {
    // this->currentHue_ = CHSV(0,0,0);
    // this->currentRGB_ = CRGB(0,0,0);
    this->index = 0;
    this->prob = 200;
    this->strip = 0;
  }

  void update_model() {
    this->index = (this->index + 1) % NUM_LEDS_PER_STRIP;
    if (this->index >= NUM_LEDS_PER_STRIP-1) {
     this->strip = (this->strip + 1) % NUMSTRIPS;
     Serial.println(this->strip);
    }
  }

  void loop() {


   if (random(1000) < this->prob) {
     this->currentHue_ = CHSV(this->currentHue_.h + uint8_t(random(12)), this->currentHue_.s + uint8_t(random(32)), this->currentHue_.v + uint8_t(random(5)) );
     hsv2rgb_rainbow( this->currentHue_, this->currentRGB_);
   
//     Serial.print(this->currentHue_.h); Serial.print(" ");
//     Serial.print(this->currentHue_.s); Serial.print(" ");
//     Serial.print(this->currentHue_.v); Serial.print(" ");
//     Serial.println("");
   }
   
//   for (int j = 0; j < NUMSTRIPS; j++) {
     driver_->setPixel(((this->strip * NUM_LEDS_PER_STRIP) + this->index) % (NUMSTRIPS * NUM_LEDS_PER_STRIP), this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
//     driver_->setBrightness(MASTER_BRIGHTNESS);
//   }
   driver_->showPixels(); 
 }
};
#endif





//CHSV currentHue_;
//CRGB currentRGB_;
//
//void test_all_with_chasers(I2SClocklessLedDriver driver) {
//
//// clear_leds();
//
// for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
//
//   if (random(1000) < 200) {
//     currentHue_ = CHSV(currentHue_.h + uint8_t(random(12)), currentHue_.s + uint8_t(random(32)), currentHue_.v + uint8_t(random(5)) );
//     hsv2rgb_rainbow( currentHue_, currentRGB_);
//   
//     Serial.print(currentHue_.h); Serial.print(" ");
//     Serial.print(currentHue_.s); Serial.print(" ");
//     Serial.print(currentHue_.v); Serial.print(" ");
//     Serial.println("");
//   }
//   
//   for (int j = 0; j < NUMSTRIPS; j++) {
//     driver.setPixel(((j * NUM_LEDS_PER_STRIP) + i) % (NUMSTRIPS * NUM_LEDS_PER_STRIP), currentRGB_.r, currentRGB_.g, currentRGB_.b, MASTER_BRIGHTNESS);
//     driver.setBrightness(MASTER_BRIGHTNESS);
//   }
//   driver.showPixels(); 
// }
//}
