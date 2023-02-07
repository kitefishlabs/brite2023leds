#include <FastLED.h>
#include "I2SClocklessLedDriver.h"
#include "FastLED_RGBW.h"
#include "Arduino.h"
#include "WiFi.h"
#include <ETH.h>
#include "AsyncUDP.h"
// #include <OSCMessage.h>  /// https://github.com/CNMAT/OSC
// #include <OSCBundle.h>  /// https://github.com/CNMAT/OSC

#include "common.h"

//#define ETHERNET_ACTIVE
#define FULL_DMA_BUFFER


int _mode = LIGHT_SIDES; // default = off
int __mode = _mode;

int sd = 0;

/* layout 

BOTTOM - rough estimate, TODO plug in actual data

2 sides, 8* strands, each strand has 2 levels and 4 panels, + blank verticals

3 31 * 4 / 5          7 31 * 4 / 5
2 34 * 4 / 5          6 34 * 4 / 5
1 37 * 4 / 5          5 37 * 4 / 5
0 40 * 4 / 5          4 40 * 4 / 5

see pin mapping in pins array

TOP - TBD, likely 3-4 loops around the structure, rough estimates below

4 strands, each strand has 6 sections, no blank verticals

3 36 * 6
2 30 * 6
1 30 * 6
0 30 * 6

*/

// IPAddress IP_ADDRESS = IPAddress(192,168,0,120); // client IP address (ie the ESP32) - INGENUITY - SKIDMARK
IPAddress IP_ADDRESS = IPAddress(192,168,0,122); // client IP address (ie the ESP32) - INGENUITY - SKIDMARK

I2SClocklessLedDriver driver;

// PINS
// 8 total pins in the full setup
int pins[NUMSTRIPS]={4,2, 13,12, 15,14, 33,32};

// LEDs and states
CRGBW leds[NUM_COLOR_CHANNELS*TOTAL_LEDS];

CHSV states_colors[LIGHTABLE_LEDS];

// mark leds when they change (certain modes)
bool states_dirty__flag[LIGHTABLE_LEDS];

long counter = 0L;


AsyncUDP udp;

static bool eth_connected = false;

CHSV default_color = CHSV(0, 255, 128);

CHSV currentHue = CHSV(0,0,0);
CRGB currentRGB = CRGB::Black;

CHSV example_blue = CHSV(0, 120,64);

// 190 = 44 + 44 + 46 + 46 + 10   PW N = Width of PANEL at LEVEL N
// 200 = 47 + 47 + 48 + 48 + 10
// 212 = 50 + 50 + 51 + 51 + 10
// 222 = 52 + 52 + 54 + 54 + 10

int PW[16] = {47,47,45,43,42,41,38,38, 47,47,45,43,42,41,38,38};
int H = 14;
int D[8] = { 8, 9, 9, 8, 8, 9, 8, 8 };



int sides[2][8][5] = {
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

int levels[8][2][5] = {
  {{3, (H + (PW[7]*2))       , (PW[7]*2), -1, 1},      {7,  H                              , (PW[15]*2), -1, 1}}, // could knock out zeros as documentation here and all further... todo
  {{3, (H + (PW[7]*2) + D[3]), (PW[6]*2),  1, 1},      {7, (H + (PW[14]*2)+D[7]+(PW[15]*2)), (PW[14]*2),  1, 1}},
  {{2,      (PW[5]*2)        , (PW[5]*2), -1, 1},      {6,  0                              , (PW[13]*2), -1, 1}},
  {{2, (    (PW[5]*2) + D[2]), (PW[4]*2),  1, 1},      {6, (    (PW[12]*2)+D[6]+(PW[13]*2)), (PW[12]*2),  1, 1}},
  {{2,      (PW[3]*2)        , (PW[3]*2), -1, 1},      {5,  0                              , (PW[11]*2), -1, 1}},
  {{1, (    (PW[3]*2) + D[1]), (PW[2]*2),  1, 1},      {5, (    (PW[10]*2)+D[5]+(PW[11]*2)), (PW[10]*2),  1, 1}},
  {{0,      (PW[1]*2)        , (PW[1]*2), -1, 1},      {4,  0                              , (PW[9]*2),  -1, 1}},
  {{0, (    (PW[1]*2) + D[0]), (PW[0]*2),  1, 1},      {4, (   (PW[8]*2)+D[4]+(PW[9]*2))   , (PW[8]*2),   1, 1}}
};


/*  SEE END OF DOCUMENT  */

void clear_leds() {
  for (int i = 0; i < (NUMSTRIPS * NUM_LEDS_PER_STRIP); i++) {
    driver.setPixel(i, 0, 0, 0);
  }
   driver.showPixels();
}

void reset() {

 Serial.print("RESET() MODE = "); Serial.println(_mode);
 clear_leds();
// delay(50);
}

void setup() {

 Serial.begin(115200);
 Serial.println("Serial - initialize");

#ifdef ETHERNET_ACTIVE 
 WiFi.onEvent(WiFiEvent);
 Serial.println("WiFi.onEvent");

 ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);
 Serial.println("ETH begin");
 while (!eth_connected) {
   Serial.print('.');
   delay(500);
 }
 Serial.println();
#endif

 Serial.print("LEDs per strip: "); Serial.println(NUM_LEDS_PER_STRIP);
 driver.initled((uint8_t*)leds,pins,NUMSTRIPS,NUM_LEDS_PER_STRIP,ORDER_GRBW); // (uint8_t*) leds are 4-uint8-wide
 driver.setBrightness(MASTER_BRIGHTNESS);
 Serial.print("master brightness: "); Serial.println(MASTER_BRIGHTNESS);

 reset();

#ifdef ETHERNET_ACTIVE
 if(udp.listen(IP_ADDRESS, PORT)) {
   Serial.println("UDP connected");
   udp.onPacket([](AsyncUDPPacket packet) {
    //  report_and_respond(packet, true, false);
    //  OSCMessage msgIN;
    //  for (int i=0; i<packet.length(); i++) {
    //    msgIN.fill(*(packet.data()+i));
    //  }
    //  if(!msgIN.hasError()) {
    //     msgIN.dispatch("/mode", receiveMode);
    //  }
   });
   delay(500);
 }
#endif
   Serial.println("END SETUP");
   counter = 0;
   
}


int speed = 1;


void trigger_side() {

  sd = (sd + 1) % 2;

  CRGB temp = CRGB(0, 0, 0);
  example_blue = CHSV(beatsin8(3*speed,0,255), beatsin8(5*speed,120,240), beatsin8(7*speed,48,200));
  hsv2rgb_rainbow( example_blue, temp);

  Serial.print(example_blue.hue); Serial.print(" "); Serial.print(example_blue.saturation); Serial.print(" "); Serial.println(example_blue.value); Serial.print(" "); Serial.println(sd);
  
  for (int i=0; i<8; i++) {
    int r = sides[sd][i][0];
    int o = sides[sd][i][1];
    int l = sides[sd][i][2];
    int d = sides[sd][i][3];
    int lt = sides[sd][i][4];
    int start = (r * 200) + o;
    // Serial.print(r); Serial.print(" "); Serial.print(o); Serial.print(" "); Serial.print(l); Serial.print(" "); Serial.print(d); Serial.print("  - "); Serial.println(start);
    // d can be negative!
    if (d > 0) {  
      Serial.print("A: "); Serial.print(temp.r); Serial.print(" ");  Serial.print(temp.g); Serial.print(" ");  Serial.print(temp.b); Serial.print(" ");  Serial.print(start); Serial.print(" "); Serial.println(start+(d*l));
      for (int j=start; j<(start+(d*l)); j++) {
//        driver.setPixel(j, 0, 0, 0);
          driver.setPixel(j, temp.r, temp.g, temp.b); //, MASTER_BRIGHTNESS/4);
      }
    } else {
      Serial.print("B: "); Serial.print(temp.r); Serial.print(" ");  Serial.print(temp.g); Serial.print(" ");  Serial.print(temp.b); Serial.print(" ");  Serial.print(start+(d*l)); Serial.print(" "); Serial.println(start);
      for (int j=start-1; j>=(start+(d*l)); j--) {
//        driver.setPixel(j, 0, 0, 0);
//        Serial.print("[B]: "); Serial.print(temp.r); Serial.print(" ");  Serial.print(temp.g); Serial.print(" ");  Serial.print(temp.b); Serial.print(" ");  Serial.println(j);
        driver.setPixel(j, temp.r, temp.g, temp.b); //, MASTER_BRIGHTNESS/4);
      }
    }
    
//    r = sides[1-sd][i][0];
//    o = sides[1-sd][i][1];
//    l = sides[1-sd][i][2];
//    d = sides[1-sd][i][3];
//    lt = sides[1-sd][i][4];
//    start = (r * 200) + o;
//    // Serial.print(r); Serial.print(" "); Serial.print(o); Serial.print(" "); Serial.print(l); Serial.print(" "); Serial.print(d); Serial.print("  - "); Serial.println(start);
//    // d can be negative!
//    if (d > 0) {
//      Serial.print("C: "); Serial.print(start); Serial.print(" "); Serial.println(start+(d*l));
//      for (int j=start; j<(start+(d*l)); j++) {
//        driver.setPixel(j, 0, 0, 0);
//      }
//    } else {
//      Serial.print("D: "); Serial.print(start+(d*l)); Serial.print(" "); Serial.println(start);
//      for (int j=start-1; j>=(start+(d*l)); j--) {
//        driver.setPixel(j, 0, 0, 0);
//      }
//    }
  }
}



void trigger_level() {
  
  int lvl = (lvl + 1) % 8;

  CRGB temp = CRGB(0, 0, 0);
  example_blue = CHSV(beatsin8(3*speed,0,255), beatsin8(5*speed,120,240), beatsin8(7*speed,48,200));
  hsv2rgb_rainbow( example_blue, temp);

  Serial.print(example_blue.hue); Serial.print(" "); Serial.print(example_blue.saturation); Serial.print(" "); Serial.println(example_blue.value); Serial.print(" "); Serial.println(sd);

//  for (int i=0; i<2; i++) {
//    int r = sides[i][lvl][0];
//    int o = sides[i][lvl][1];
//    int l = sides[i][lvl][2];
//    int d = sides[i][lvl][3];
//    int lt = sides[i][lvl][4];
//
//
//  for (int i=0; i<2; i++) {
//    int r = sides[i][lvl][0];
//    int o = sides[i][lvl][1];
//    int l = sides[i][lvl][2];
//    int d = sides[i][lvl][3];
//    int lt = sides[i][lvl][4];
//
//    
//    
//    if (d > 0) {  
//      for (int j=start; j<(start+(d*l)); j++) {
//        if (lt > 0) {
//          driver.setPixel(j, temp.r, temp.g, temp.b, MASTER_BRIGHTNESS/4);
//        } else {
//          driver.setPixel(j, temp.r, temp.g, temp.b, 0);
//        }
//      }
//    } else {
//      for (int j=start; j>(start+(d*l)); j--) {
//        if (lt > 0) {
//          driver.setPixel(j, temp.r, temp.g, temp.b, MASTER_BRIGHTNESS/4);
//        } else {
//          driver.setPixel(j, temp.r, temp.g, temp.b, 0);
//        }
//      }
//    }
//  }
//
//  driver.showPixels();
//  delay(dly_t);
}

//int off=0;
//long time1,time2,time3;

void loop() {

//  time1=ESP.getCycleCount();

//  Serial.print("Mode: "); Serial.println(_mode);

  if (__mode != _mode) {
    Serial.print("Mode changed: "); Serial.println(_mode);
    _mode = __mode;
    reset();
    counter = 0;
  }
  
  if (_mode == DARK) {

    clear_leds();
    
//  } 
//  else if (_mode == ITERATE_TEST) {
//
//    for (int x=0; x<8; x++) {
//      trigger_level(x);
//      driver.showPixels();
//      delay(500);
//    }

  } else if (_mode == TEST_CHASERS) {

    clear_leds();
    test_all_with_chasers();
    // calls show and delay internally

  } else if (_mode == LIGHT_SIDES) {

    EVERY_N_MILLISECONDS(ALTERNATE_SIDES_PERIOD_MS) {
      clear_leds();
      trigger_side();
      driver.showPixels();
    }

  } else if (_mode == TRIGGER_LEVELS_UP) {

    for (int x=0; x<8; x++) {
      clear_leds();
      trigger_level();
    }


  } else if (_mode == TRIGGER_LEVELS_DOWN) {
    
    for (int x=0; x<8; x++) {
      clear_leds();
      // invert
      trigger_level();
    }


  } else {
    ;
  }

//  counter++;
//  time2=ESP.getCycleCount();
//  driver.showPixels();
//  time3=ESP.getCycleCount();
//  Serial.printf("Calcul pixel fps:%.2f   showPixels fps:%.2f   Total fps:%.2f \n",(float)240000000/(time2-time1),(float)240000000/(time3-time2),(float)240000000/(time3-time1));
//  off++;
  // delay(2000);

  delay(10);

}


#ifdef ETHERNET_ACTIVE
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
    
 }
}
#endif

// OSC Callback Functions (if necessary)



// void receiveMode(OSCMessage &msg) {
//   __mode = int(msg.getInt(0));
//   Serial.print("mode to be: "); Serial.println(__mode);
// }
















/*

CALCULATE THE ROLDs for each logical segment
TODO: RECALCULATE FOR NEW LAYOUT!!!
S - 
0 3 ((PW[6]*2) + D[3]) (PW[7]*2) *
  3  (PW[6]*2)       (PW[6]*2) *
  2 ((PW[4]*2) + D[2]) (PW[5]*2) *
  2  (PW[4]*2)       (PW[4]*2) *
  1 ((PW[2]*2) + D[1]) (PW[3]*2) *
  1  (PW[2]*2)       (PW[2]*2) *
  0 ((PW[0]*2) + D[0]) (PW[1]*2) *
  0  (PW[0]*2)       (PW[0]*2) *
1 7 ((PW[14]*2)+D[7]+(PW[15]*2)) (PW[15]*2) *
  7   0                          (PW[14]*2) *
  6 ((PW[12]*2)+D[6]+(PW[13]*2)) (PW[13]*2) *
  6   0                          (PW[12]*2) *
  5 ((PW[10]*2)+D[5]+(PW[11]*2)) (PW[11]*2) *
  5   0                          (PW[10]*2) *
  4 ((PW[8]*2)+D[4]+(PW[9]*2))    (PW[9]*2) *
  4   0                           (PW[8]*2) *

L  -  (r, o, l, d)
0  3 ((PW[6]*2) + D[3]) (PW[7]*2) +,      7 ((PW[14]*2)+D[7]+(PW[15*2))  (PW[15]*2) -
1  3  (PW[6]*2)         (PW[6]*2) -,      7   0                          (PW[14]*2) +
2  2 ((PW[4]*2) + D[2]) (PW[5]*2) +,      6 ((PW[12]*2)+D[6]+(PW[13]*2)) (PW[13]*2) -
3  2  (PW[4]*2)         (PW[4]*2) -,      6   0                          (PW[12]*2) +
4  1 ((PW[2]*2) + D[1]) (PW[3]*2) +,      5 ((PW[10]*2)+D[5]+(PW[11]*2)) (PW[11]*2) -
5  2  (PW[2]*2)         (PW[2]*2) -,      5   0                          (PW[10]*2) +
6  0 ((PW[0]*2) + D[0]) (PW[1]*2) +,      4 ((PW[8]*2)+D[4]+(PW[9]*2))   (PW[9]*2) -
7  0  (PW[0]*2)         (PW[0]*2) -,      4   0                          (PW[8]*2) +


P  -  (r,o,l,d)
0  3 ((PW[6]*2) + D[3]) (PW[7]*2) +
1  3  (PW[6]*2)         (PW[6]*2) -
2  2 ((PW[4]*2) + D[2]) (PW[5]*2) +
3  2  (PW[4]*2)         (PW[4]*2) -
4  1 ((PW[2]*2) + D[1]) (PW[3]*2) +
5  2  (PW[2]*2)         (PW[2]*2) -
6  0 ((PW[0]*2) + D[0]) (PW[1]*2) +
7  0  (PW[0]*2)         (PW[0]*2) -

8  7 ((PW[14]*2)+D[7]+(PW[15]*2)) (PW[15]*2) -
9  7   0                          (PW[14]*2) +
10 6 ((PW[12]*2)+D[6]+(PW[13]*2)) (PW[13]*2) -
11 6   0                          (PW[12]*2) +
12 5 ((PW[10]*2)+D[5]+(PW[11]*2)) (PW[11]*2) -
13 5   0                          (PW[10]*2) +
14 4 ((PW[8]*2)+D[4]+(PW[9]*2))   (PW[9]*2) -
15 4   0                          (PW[8]*2) +


G  -  (r,o,l,d)
0  3 ((PW[6]*2) + D[3]) PW[7] +
1  3  (PW[6]*2)         PW[6] -
2  2 ((PW[4]*2) + D[2]) PW[5] +
3  2  (PW[4]*2)         PW[4] -
4  1 ((PW[2]*2) + D[1]) PW[3] +
5  2  (PW[2]*2)         PW[2] -
6  0 ((PW[0]*2) + D[0]) PW[1] +
7  0  (PW[0]*2)         PW[0] -

 8  3 ((PW[6]*2) + D[3] + PW[7])  PW[7] +
 9  3  (PW[6]*2)      -   PW[6])  PW[6] -
10  2 ((PW[4]*2) + D[2] + PW[5])  PW[5] +
11  2  (PW[4]*2)      -   PW[4])  PW[4] -
12  1 ((PW[2]*2) + D[1] + PW[3])  PW[3] +
13  2  (PW[2]*2)      -   PW[2])  PW[2] -
14  0 ((PW[0]*2) + D[0] + PW[1])  PW[1] +
15  0  (PW[0]*2)      -   PW[0])  PW[0] -

16 7 ((PW[14]*2)+D[7]+(PW[15]*2)) PW[15] -
17 7   0                          PW[14] +
18 6 ((PW[12]*2)+D[6]+(PW[13]*2)) PW[13] -
19 6   0                          PW[12] +
20 5 ((PW[10]*2)+D[5]+(PW[11]*2)) PW[11] -
21 5   0                          PW[10] +
22 4 ((PW[8]*2)+D[4]+(PW[9]*2))   PW[9] -
23 4   0                          PW[8] +

24 7 ((PW[14]*2)+D[7]+PW[15]) PW[15] -
25 7   0             +PW[14]  PW[14] +
26 6 ((PW[12]*2)+D[6]+PW[13]) PW[13] -
27 6   0             +PW[12]  PW[12] +
28 5 ((PW[10]*2)+D[5]+PW[11]) PW[11] -
29 5   0             +PW[10]  PW[10] +
30 4 ((PW[8]*2)+D[4]+PW[9])   PW[9] -
31 4   0            +PW[8]    PW[8] +
*/



void test_all_with_chasers() {

  clear_leds();

  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {

    if (random(1000) < 200) {
      currentHue = CHSV(currentHue.h + uint8_t(random(12)), currentHue.s + uint8_t(random(32)), currentHue.v + uint8_t(random(5)) );
      hsv2rgb_rainbow( currentHue, currentRGB);
    
      Serial.print(currentHue.h); Serial.print(" ");
      Serial.print(currentHue.s); Serial.print(" ");
      Serial.print(currentHue.v); Serial.print(" ");
      Serial.println("");
    }
    
    for (int j = 0; j < NUMSTRIPS; j++) {
      driver.setPixel(((j * NUM_LEDS_PER_STRIP) + i) % (NUMSTRIPS * NUM_LEDS_PER_STRIP), currentRGB.r, currentRGB.g, currentRGB.b, MASTER_BRIGHTNESS);
      driver.setBrightness(MASTER_BRIGHTNESS);
    }
    driver.showPixels();
    delay(2);
    
  }
  delay(2000);
    
}
