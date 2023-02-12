#include <FastLED.h>
#include "I2SClocklessLedDriver.h"
#include "FastLED_RGBW.h"
#include "Arduino.h"
#include "WiFi.h"
#include <ETH.h>
#include "AsyncUDP.h"
 #include <OSCMessage.h>  /// https://github.com/CNMAT/OSC
 #include <OSCBundle.h>  /// https://github.com/CNMAT/OSC


#include "leds_chasers_test.h"
#include "leds_light_side.h"
#include "leds_trigger_level.h"
#include "leds_trigger_level_sweep.h"

#define ETHERNET_ACTIVE
#define FULL_DMA_BUFFER


int _mode = LOOP_LEVELS_SWEEP_UP_INIT; // default = off
int __mode = _mode;

int __param0 = 12;

int sd = 0;
int lvl = 0;
int offset = 10;
int lvls[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
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
IPAddress IP_ADDRESS = IPAddress(192,168,0,101); // client IP address (ie the ESP32) - INGENUITY - SKIDMARK

I2SClocklessLedDriver *driver = new I2SClocklessLedDriver();

// PINS
// 8 total pins in the full setup
int pins[NUMSTRIPS]={4,2, 13,12, 15,14, 33,32};

// LEDs and states
CRGBW leds[NUM_COLOR_CHANNELS*TOTAL_LEDS];

CHSV states_colors[LIGHTABLE_LEDS];

// mark leds when they change (certain modes)
bool states_dirty__flag[LIGHTABLE_LEDS];

int counter = 0;


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

//int PW[16] = {47,47,45,43,42,41,38,38, 47,47,45,43,42,41,38,38};
//int H = 14;
//int D[8] = { 8, 9, 9, 8, 8, 9, 8, 8 };

LEDsChasersTest chasersTest = LEDsChasersTest(driver);
LEDsLightSide lightSides = LEDsLightSide(driver);
LEDsTriggerLevel triggerLevels = LEDsTriggerLevel(driver);
LEDsTriggerLevelSweep triggerLevelSweeps = LEDsTriggerLevelSweep(driver);

// OSC Callback Functions (if necessary)

void receiveMode(OSCMessage &msg) {
  __mode = int(msg.getInt(0));
  __param0 = int(msg.getInt(1));
  Serial.print("mode to be: "); Serial.println(__mode);
  Serial.print("Params: "); Serial.println(__param0);
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


/*  SEE END OF DOCUMENT  */

void clear_leds(uint16_t rolloff=0) {
//  Serial.print("R:: "); Serial.println(rolloff);
  for (int i = 0; i < (NUMSTRIPS * NUM_LEDS_PER_STRIP); i++) {
    driver->setPixel(i, 0, 0, 0);
  }
   driver->showPixels();
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
 driver->initled((uint8_t*)leds,pins,NUMSTRIPS,NUM_LEDS_PER_STRIP,ORDER_GRBW); // (uint8_t*) leds are 4-uint8-wide
 driver->setBrightness(MASTER_BRIGHTNESS);
 Serial.print("master brightness: "); Serial.println(MASTER_BRIGHTNESS);

 reset();

#ifdef ETHERNET_ACTIVE
 if(udp.listen(IP_ADDRESS, PORT)) {
   Serial.println("UDP connected");
   udp.onPacket([](AsyncUDPPacket packet) {
//      report_and_respond(packet, true, false);
      OSCMessage msgIN;
      Serial.println(packet.length());
      for (int i=0; i<packet.length(); i++) {
        msgIN.fill(*(packet.data()+i));
      }
      if(!msgIN.hasError()) {
         msgIN.dispatch("/mode", receiveMode);
      }
   });
   delay(500);
 }
#endif
   Serial.println("END SETUP");
   counter = 0;


  lightSides.init();
  triggerLevels.init();
  triggerLevelSweeps.init(1);
}


void loop() {
  
  if (__mode != _mode) {
    Serial.print("Mode changed: "); Serial.println(_mode);
    _mode = __mode;
    reset();
    counter = 0;
  }
  
  if (_mode == DARK) {

    clear_leds(max(0, __param0));

  } else if (_mode == TEST_CHASERS) {

    chasersTest.update_model();
    chasersTest.loop();

  } else if (_mode == LIGHT_SIDES) {
    
    if ((millis() % __param0) < 10) {
      
      clear_leds();
      
      lightSides.update_model();
      lightSides.loop();
      
    }

  } else if (_mode == TRIGGER_LEVELS_UP) {

    if ((millis() % __param0) < 10) {
      clear_leds();

      // up is down, down is up, peace is war
      triggerLevels.update_model(-1);
      triggerLevels.loop();
    }

 } else if (_mode == TRIGGER_LEVELS_DOWN) {
   
   if ((millis() % __param0) < 10) {
     clear_leds();
     
     triggerLevels.update_model(1);
     triggerLevels.loop();
   }

 } else if (_mode == LOOP_LEVELS_SWEEP_UP_INIT) {

   Serial.println("LOOP_LEVELS_SWEEP_UP_INIT");
   triggerLevelSweeps.init(-1);
   
   __mode = LOOP_LEVELS_SWEEP_UP;

 } else if (_mode == LOOP_LEVELS_SWEEP_UP) {
   
   if ((millis() % __param0) < 10) {
     clear_leds();
     triggerLevelSweeps.update_model(-1);
     triggerLevelSweeps.loop(-1);
   }

 } else if (_mode == LOOP_LEVELS_SWEEP_DOWN_INIT) {

   Serial.println("LOOP_LEVELS_SWEEP_DOWN_INIT");
   __mode = LOOP_LEVELS_SWEEP_DOWN;
   triggerLevelSweeps.init(1);

 } else if (_mode == LOOP_LEVELS_SWEEP_DOWN) {
   
   if ((millis() % __param0) < 10) {
     clear_leds();
     triggerLevelSweeps.update_model(1);
     triggerLevelSweeps.loop(1);
   }

  } //else {
//    ;
//  }

}
