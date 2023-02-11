#ifndef LEDS_COMMON
#define LEDS_COMMON
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

#define MASTER_BRIGHTNESS 128


///////

#define LIGHTABLE_SEGMENTS 32
#define BLANK_SEGMENTS 16
#define TOTAL_SEGMENTS 48



// #define PROB_FIVE_PERCENT 6
// #define PROB_TEN_PERCENT 13
// #define PROB_TWENTY_PERCENT 26

// #define TRIGGER_LEVELS_T 30
// #define LOOP_LEVELS_T 30


//
//int levels_down[8] = {7, 6, 5, 4, 3, 2, 1};
//int levels_up[8] = {0, 1, 2, 3, 4, 5, 6};

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



int PW[16] = {47,47,45,43,42,41,38,38, 47,47,45,43,42,41,38,38};
int H = 14;
int D[8] = { 8, 9, 9, 8, 8, 9, 8, 8 };

int SIDES[2][8][5] = {
  {
    {3,  (H + (PW[7]*2)),          (PW[7]*2), -1, 1},
    {3,  (H + (PW[7]*2) + D[3]),   (PW[6]*2),  1, 1},
    {2,       (PW[5]*2),           (PW[5]*2), -1, 1},
    {2,      ((PW[5]*2) + D[2]),   (PW[4]*2),  1, 1},
    {1,       (PW[3]*2),           (PW[3]*2), -1, 1},
    {1,      ((PW[3]*2) + D[1]),   (PW[2]*2),  1, 1},
    {0,       (PW[1]*2),           (PW[1]*2), -1, 1},
    {0,      ((PW[1]*2) + D[0]),   (PW[0]*2),  1, 1}
    
//    {3,  (H + (PW[7]*2)),          D[3], 1, 0},     // goal is to never use these, perhaps only use for testing
//    {2,  (H + (PW[5]*2)),          D[2], 1, 0},
//    {1,  (H + (PW[3]*2)),          D[1], 1, 0},
//    {0,  (H + (PW[1]*2)),          D[0], 1, 0}
  },

  // (3, (35 * 2) + 10, (34 * 2), 1) = 450 + 70 + 10, 68 = (522 590)   -- idx 0
  // (1, (39 * 2)     , (39 * 2), 1) = 150 + 78 + 10, 78 = (238 316)   -- idx 5
  // (0, (41 * 2)     , (41 * 2), 1) =   0 + 82 + 10, 82 = (92 174)   -- idx 7

  {
    {7,  H,                               (PW[15]*2),  1, 1},
    {7, (H + (PW[15]*2)+D[7]+(PW[14]*2)), (PW[14]*2), -1, 1},
    {6,  0,                               (PW[13]*2),  1, 1},
    {6, (    (PW[13]*2)+D[6]+(PW[12]*2)), (PW[12]*2), -1, 1},
    {5,  0,                               (PW[11]*2),  1, 1},
    {5, (    (PW[11]*2)+D[5]+(PW[10]*2)), (PW[10]*2), -1, 1},
    {4,  0,                               (PW[9]*2),   1, 1},
    {4, (    (PW[9]*2)+D[4]+(PW[8]*2)),   (PW[8]*2),  -1, 1} //,
    
//    {7,  (H + (PW[15]*2)),                D[7],       0, 0},
//    {6,  (H + (PW[13]*2)),                D[6],       0, 0},
//    {5,  (H + (PW[11]*2)),                D[5],       0, 0},
//    {4,  (H + (PW[9]*2)),                 D[4],       0, 0}    
  }
};


int LEVELS[8][2][5] = {
  {{3, (H + (PW[7]*2))       , (PW[7]*2), -1, 1},      {7,  H                              , (PW[15]*2),  1, 1}}, // could knock out zeros as documentation here and all further... todo
  {{3, (H + (PW[7]*2) + D[3]), (PW[6]*2),  1, 1},      {7, (H + (PW[15]*2)+D[7]+(PW[14]*2)), (PW[14]*2), -1, 1}},
  {{2,      (PW[5]*2)        , (PW[5]*2), -1, 1},      {6,  0                              , (PW[13]*2),  1, 1}},
  {{2, (    (PW[5]*2) + D[2]), (PW[4]*2),  1, 1},      {6, (    (PW[13]*2)+D[6]+(PW[12]*2)), (PW[12]*2), -1, 1}},
  {{1,      (PW[3]*2)        , (PW[3]*2), -1, 1},      {5,  0                              , (PW[11]*2),  1, 1}},
  {{1, (    (PW[3]*2) + D[1]), (PW[2]*2),  1, 1},      {5, (    (PW[11]*2)+D[5]+(PW[10]*2)), (PW[10]*2), -1, 1}},
  {{0,      (PW[1]*2)        , (PW[1]*2), -1, 1},      {4,  0                              , (PW[9]*2),   1, 1}},
  {{0, (    (PW[1]*2) + D[0]), (PW[0]*2),  1, 1},      {4, (   (PW[9]*2)+D[4]+(PW[8]*2))   , (PW[8]*2),  -1, 1}}
};






/*
 * Produces next value in series.
 * 
 * Can produce different patterns based on delta
 * Lo and Hi contrain the range
 * 
 * Delta can be +/-, larger values make larger leaps
 * 
 */
int iter_delta(int val, int delta, int lo=0, int hi=256) {
  return ((((val + delta) - lo) % (hi - lo)) + lo);
}




















#endif
