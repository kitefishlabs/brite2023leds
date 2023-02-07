#include <FastLED.h>
#include "I2SClocklessLedDriver.h"
#include "FastLED_RGBW.h"
#include "Arduino.h"
#include "WiFi.h"
#include <ETH.h>
#include "AsyncUDP.h"
// #include <OSCMessage.h>  /// https://github.com/CNMAT/OSC
// #include <OSCBundle.h>

#include "colors.h"
#include "modes.h"


#define FULL_DMA_BUFFER

#define PORT 5005                           // client Port

#define ETH_ADDR        1
#define ETH_POWER_PIN   5
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#define ETH_TYPE        ETH_PHY_IP101


#define NUM_COLOR_CHANNELS 4  // RGBW

#define NUMSTRIPS 8 // 2 + 1 + 2, 2X
#define NUM_LEDS_PER_STRIP 200
// #define NUM_LEDS_PER_DOUBLE_STRIP 390
#define TOTAL_LEDS 1560
//  int8_t segments_path_follower[(LIGHTABLE_SEGMENTS * LOOKUP_COLS)] = {
// #define LOOKUP_COLS 4

#define LIGHTABLE_LEDS 1560

#define MASTER_BRIGHTNESS 200


///////

#define LIGHTABLE_SEGMENTS 32
#define BLANK_SEGMENTS 16
#define TOTAL_SEGMENTS 48



// #define PROB_FIVE_PERCENT 6
// #define PROB_TEN_PERCENT 13
// #define PROB_TWENTY_PERCENT 26

// #define TRIGGER_LEVELS_T 30
// #define LOOP_LEVELS_T 30



int levels_down[8] = {7, 6, 5, 4, 3, 2, 1};
int levels_up[8] = {0, 1, 2, 3, 4, 5, 6};

// int levels_vertical_inward[4][2]  = {{0,7}, {1,6}, {2,5}, {3,4}};
// int levels_vertical_outward[4][2] = {{3,4}, {2,5}, {1,6}, {0,7}};

// Data structures for Ps and Gs TBA




// idx, real idx
// next in path, INDEXED NEXT IN PATH, previous in path,
// next UP. next DOWN
// symmetry around 0-axis. symmetry around both axes, symmetry around 1-axis

// symmetry cols have no sign but probably should

// (14 * 2) + (7 * 4) + (8 * 2) = 28 + 28 + 16 = 72 * 11 per row = 864

// int8_t segments_path_follower[(LIGHTABLE_SEGMENTS * LOOKUP_COLS)] = { 
//    0,  0,  1, 31,
//    1,  1,  2,  0,
//    2,  3,  3,  1,
//    3,  4,  4,  2,
//    4,  5,  5,  3,
//    5,  6,  6,  4,
//    6,  8,  7,  5,
//    7,  9,  8,  6,
//    8, 10,  9,  7,
//    9, 11, 10,  8,
//   10, 13, 11,  9
// };



// determine sign of integer
int sign(int inp) {
  if (inp < 0) {
    return -1;
  } else {
    return 1;
  }
  
}

// 99                -> 0
// 100               -> 0
// 125 0 100 100 250 -> 17
// 250               -> 100
// 400               -> 100
// interpolate input into range rng, with optional base (offset from 0)
// note: args are "backwards"/target first
uint8_t interpolate8(uint8_t inp, uint8_t y0=0, uint8_t y1=100, uint8_t x0=0, uint8_t x1=255) {
  return (((max((min(inp, x1) - x0), 0) * (y1 - y0)) / (x1 - x0)) + y0);
}

// LOGGING

void log_leds(CRGBW leds[], int numLEDs) {
  Serial.println("LEDS");
  for (int i=0;i<numLEDs;i++) {
    Serial.print(leds[i].r); Serial.print(","); Serial.print(leds[i].g); Serial.print(","); Serial.print(leds[i].b); Serial.print("|");
    if ((i%10) == 9) {
      Serial.println(" "); 
    }
  }  
}

// CLEAR FUNCTIONS
//void clear_leds(I2SClocklessLedDriver driver, CRGB color = CRGB::Black) {
//  for(int i=0; i<TOTAL_LEDS ;i++) {
//    driver.setPixel(i,color.r,color.g,color.b, 0);
//  }
//}

void clear_hues(CHSV currentHues[], int num_hues, uint8_t hue=0, uint8_t sat=255, uint8_t valu=255) {
  for (int i=0;i<num_hues;i++) {
    currentHues[i] = CHSV(hue, sat, valu);
  }
}
