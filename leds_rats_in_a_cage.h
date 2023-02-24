#ifndef LEDS_RATS_IN_A_CAGE
#define LEDS_RATS_IN_A_CAGE
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"

#include "common.h"

#include "colors.h"
#include "modes.h"


class LEDsRatsInACage {

private:
  I2SClocklessLedDriver* driver_;
  CHSV currentHSV_;                 // current HSV color for periodic sampling whenever the light-side function is called
  CRGB currentRGB_ = CRGB(0,0,0);
  uint8_t *leds_state_;   // 1 if active, 0 otherwise
  bool *dirty_state_;     // true per led if a swap was performed
  bool dirty_;
  
public:
  int speed_;
  int spacer_;      // control distribution of leaders
  int jitter_;      // control spread/randomness of leaders
  int prob_;        // prob of moving on step
  int hop_;         // highest possible move
  int tail_length_;
  
  LEDsRatsInACage(I2SClocklessLedDriver *driver, uint8_t *states, bool *dirty_states) {      // int sides[2][NUMSTRIPS][5]
    driver_ = driver;
    currentHSV_ = CHSV(0,0,0);
    currentRGB_ = CRGB(0,0,0);
    leds_state_ = states;
    dirty_state_ = dirty_states;
    speed_ = 1;
    jitter_ = 3;
    spacer_ = 32;
    prob_ = 64;
    hop_ = 3;
    tail_length_ = 1;
    dirty_ = false;
  };

  void clear_dirty_flags() {
    for (int i=0; i<TOTAL_LEDS; i++) { 
      this->dirty_state_[i] = false;
    }
  };

  void init() {
    
    this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_);
    
    int rnd_offset = random(this->jitter_) - (this->jitter_/2);
    this->clear_dirty_flags();
    
    for (int i=0; i<TOTAL_LEDS; i++) { 
      if ((i%this->spacer_) == 0) {
        this->dirty_state_[(i+rnd_offset)] = true;
        this->leds_state_[(i+rnd_offset)] = 1;
        rnd_offset = random(this->jitter_) - (this->jitter_/2);
      }
//      Serial.println(this->leds_state_[(i+rnd_offset)]);
    }
  };

  void update_model(LEDsPaletteController paletteCtl, int index) { 

    if (index < 8) {
      this->currentHSV_ = ColorFromPalette( paletteCtl.currentPalette_, index, 128, paletteCtl.currentBlending_);
    } else {
      this->currentHSV_ = CHSV(beatsin8(3*this->speed_,0,255), beatsin8(5*this->speed_,120,240), beatsin8(7*this->speed_,48,200));
    }

    int ii = -1;  

    this->dirty_ = false;
    this->clear_dirty_flags();// TODO: TEST THIS!!!

    for(int i=0;i<TOTAL_LEDS;i++) {
    
      if ((this->leds_state_[i] > 0) && (random8() < this->prob_)) { // update_rate = 5%

        if (this->hop_ > 0) {
          ii = (i + (int)random8(this->hop_)) % TOTAL_LEDS;
        }

        if ((this->leds_state_[i] == 1) && (this->leds_state_[ii] != 1)) {

          this->leds_state_[ii] = this->leds_state_[i];
          this->leds_state_[i] = 0;

          this->dirty_state_[ii] = true;
          this->dirty_state_[i] = true;

          this->dirty_ = true;
        }
      }
    }
  };

  void loop() {
  
    hsv2rgb_rainbow( this->currentHSV_, this->currentRGB_ );

    if (this->dirty_ == true) {
      for(int i=0;i<TOTAL_LEDS;i++) {
    
        if (this->dirty_state_[i] == true) {
      
          if (this->leds_state_[i] > 0) {
      
            // Serial.print("display pixel "); Serial.println(i);
      
            for(int t=0; t<this->tail_length_; t++) {
              this->driver_->setPixel(max((i - t), 0), this->currentRGB_.r, this->currentRGB_.g, this->currentRGB_.b);
            }
            
          }  else {
          
            this->driver_->setPixel(i, 0, 0, 0);
          
          }
          this->dirty_state_[i] = false;
        }
      }
      this->dirty_ = false;
      this->driver_->showPixels();
    }
  };

};
#endif
