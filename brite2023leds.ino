
#include <FastLED.h>
#include "I2SClocklessLedDriver.h"
#include "FastLED_RGBW.h"
#include "Arduino.h"
#include "WiFi.h"
#include <ETH.h>
#include "AsyncUDP.h"
#include <OSCMessage.h>  /// https://github.com/CNMAT/OSC
#include <OSCBundle.h>  /// https://github.com/CNMAT/OSC

/* c/o Rui Santos, Complete Project Details http://randomnerdtutorials.com */

#include "leds_chasers_test.h"
#include "leds_twinkle_stars.h"
#include "leds_rotate_bands.h"
#include "leds_rats_in_a_cage.h"
#include "leds_snowfall.h"
#include "leds_fake_spectra.h"
#include "leds_light_side.h"
#include "leds_trigger_level.h"
#include "leds_trigger_level_fade.h"
#include "leds_spiral_level_sweep.h"
#include "leds_trigger_level_sweep.h"

#include "leds_top_top.h"

#include "runner.h"

// These defines will increase the binary size and unlock the 2 networking possibilities.
//#define ETHERNET_ACTIVE

//#define SINGLE_MODE

#define FULL_DMA_BUFFER

// - INITIAL MODE
int _mode = TEST_CHASERS;
//int _mode = LIGHT_SIDES; // default = off
int __mode = _mode;

int chosen_preset[8];
int last_preset_ms = millis(); // MS
int current_preset_dur_ms = 10000;

int __param1 = 10;
int __param2 = 0;
int __param3 = 0;
int __param4 = 0;
int __param5 = 0;
int __param6 = 0;
int __param7 = 0;

// IPAddress IP_ADDRESS = IPAddress(192,168,0,120); // client IP address (ie the ESP32) - INGENUITY - SKIDMARK
IPAddress IP_ADDRESS = IPAddress(192, 168, 0, 101); // client IP address (ie the ESP32) - INGENUITY

I2SClocklessLedDriver *driver = new I2SClocklessLedDriver();

// PINS
// 8 total pins in the full setup
int pins[NUMSTRIPS] = {4, 2, 13, 12, 15, 14, 32, 33};

// LEDs and states
CRGBW leds[NUM_COLOR_CHANNELS * TOTAL_LEDS];

uint8_t leds_state_a[TOTAL_LEDS];
uint8_t leds_state_b[TOTAL_LEDS];
bool leds_state_c[TOTAL_LEDS];
float angles_state[360];

// mark leds when they change (certain modes)
//bool states_dirty__flag[LIGHTABLE_LEDS];

AsyncUDP udp;

static bool eth_connected = false;

CHSV currentHue = CHSV(0, 0, 0);
CRGB currentRGB = CRGB::Black;

LEDsPresetsRunner presets = LEDsPresetsRunner();

LEDsChasersTest chasersTest = LEDsChasersTest(driver);
LEDsLightSide lightSides = LEDsLightSide(driver);
LEDsTwinkleStars twinkleStars = LEDsTwinkleStars(driver, leds_state_a, leds_state_b);
LEDsRatsInACage ratsInACage = LEDsRatsInACage(driver, leds_state_a, leds_state_c);
LEDsRotateBands rotateBands = LEDsRotateBands(driver);
LEDsSnowfall snowfall = LEDsSnowfall(driver, angles_state);
LEDsFakeSpectra fakeSpectra = LEDsFakeSpectra(driver);
LEDsTriggerLevel triggerLevels = LEDsTriggerLevel(driver);
LEDsSpiralLevelSweep spiralLevelSweeps = LEDsSpiralLevelSweep(driver);
LEDsTriggerLevelSweep triggerLevelSweeps = LEDsTriggerLevelSweep(driver);
LEDsTriggerLevelFade triggerLevelFades = LEDsTriggerLevelFade(driver);

LEDsTopTop triggerTopTop = LEDsTopTop(driver);

LEDsPaletteController paletteCtl = LEDsPaletteController();

// OSC Callback Functions (if necessary)

void receiveMode(OSCMessage &msg) {
  __mode = int(msg.getInt(0));
  __param1 = int(msg.getInt(1));
  Serial.print("mode to be: "); Serial.println(__mode);
  Serial.print("Params: "); Serial.println(__param1);
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

void clear_leds() {
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
  driver->initled((uint8_t*)leds, pins, NUMSTRIPS, NUM_LEDS_PER_STRIP, ORDER_GRBW); // (uint8_t*) leds are 4-uint8-wide
  driver->setBrightness(MASTER_BRIGHTNESS);
  Serial.print("master brightness: "); Serial.println(MASTER_BRIGHTNESS);

  reset();

#ifdef ETHERNET_ACTIVE
  if (udp.listen(IP_ADDRESS, PORT)) {
    Serial.println("UDP connected");
    udp.onPacket([](AsyncUDPPacket packet) {
      //      report_and_respond(packet, true, false);
      OSCMessage msgIN;
      Serial.println(packet.length());
      for (int i = 0; i < packet.length(); i++) {
        msgIN.fill(*(packet.data() + i));
      }
      if (!msgIN.hasError()) {
        msgIN.dispatch("/mode", receiveMode);
      }
    });
    delay(500);
  }
#endif

  for (int i = 0; i < TOTAL_LEDS; i++) {
    leds_state_a[i] = 0;
    leds_state_b[i] = 0;
  }

  
  presets.init(10);

  
  lightSides.init();
  triggerLevels.init();
  triggerLevelSweeps.init(1);
  spiralLevelSweeps.init(1);
  triggerLevelFades.init(1);
  twinkleStars.init();
  rotateBands.init();
  ratsInACage.init();
  snowfall.init();
  fakeSpectra.init();

  triggerTopTop.init();

  last_preset_ms = millis(); // MS

  Serial.println("END SETUP");
}



void loop() {

  if ((millis() - last_preset_ms) > current_preset_dur_ms) {
  #ifndef SINGLE_MODE
//    int choice = presets.get_weighted_random_preset();
    int choice = presets.get_next_preset();
    int* chosen_preset = PRESETS[choice];
    //    Serial.println((int)chosen_preset[0]);
    __mode = (int)chosen_preset[0];
    __param1 = (int)chosen_preset[1];
    __param2 = (int)chosen_preset[2];
    __param3 = (int)chosen_preset[3];
    __param4 = (int)chosen_preset[4];
    __param5 = (int)chosen_preset[5];
    __param6 = (int)chosen_preset[6];
    __param7 = (int)chosen_preset[7];

    paletteCtl.chooseColorPalette(__param7);
#endif
    last_preset_ms = millis();
    current_preset_dur_ms = (random(10) + 2) * 2500; // 5 -> 30 seconds in 2.5 second increments

    triggerTopTop.update_model(paletteCtl, __param7);
    triggerTopTop.loop();

    if       (_mode == LIGHT_SIDES)                   {
      lightSides.init();
      lightSides.mirror_ = __param2;
      
    }
    
    else if       (_mode == TWINKLE_STARS)                   {
      twinkleStars.init();
      twinkleStars.density_ = __param2;
      twinkleStars.speed_lo_ = __param3;
      twinkleStars.speed_hi_ = __param4;

    }

    else if  (_mode == ROTATE_BANDS)                    {
      rotateBands.init();
      rotateBands.speed_ = __param2;
      rotateBands.num_bands_ = __param3;
      rotateBands.dir_ = __param3;
    }

    // else if  (_mode == ROTATE_SUM_OF_SINES)             { rotateSines.init(); rotateSines.angular_speed_ = __param2;  rotateSines.number_ = __param3;  rotateSines.offset_ = __param4;  rotateSines.thickness_ = __param5;  }

    else if  (_mode == RATS_IN_A_CAGE)                  {
      ratsInACage.init();
//      ratsInACage.speed_ = __param2;
      ratsInACage.spacer_ = __param2;
      ratsInACage.jitter_ = __param3;
      ratsInACage.prob_ = __param4;
      ratsInACage.hop_ = __param5;
      ratsInACage.tail_length_ = __param6;
    }

    else if  (_mode == SNOWFALL)                        {
      snowfall.init();
      snowfall.fall_speed_ = __param2;
      snowfall.spacer_ = __param3;
      snowfall.jitter_ = __param4;
      snowfall.restart_prob_ = __param5;
    }

    else if  (_mode == FAKE_SPECTRA)                    {
      fakeSpectra.init();
      fakeSpectra.speed_lo_ = __param2;
      fakeSpectra.speed_hi_ = __param3;
      fakeSpectra.extent_lo_ = __param4;
      fakeSpectra.extent_hi_ = __param5;
    }

    else if  (_mode == SNOWFALL)                        {
      snowfall.init();
      snowfall.fall_speed_ = __param2;
      snowfall.spacer_ = __param3;
      snowfall.jitter_ = __param4;
      snowfall.restart_prob_ = __param5;
    }

    else if  (_mode == TRIGGER_LEVELS_DOWN)             {
      triggerLevels.init();
      triggerLevels.speed_ = __param2;
    }

    else if  (_mode == TRIGGER_LEVELS_UP)               {
      triggerLevels.init();
      triggerLevels.speed_ = __param2;
    }

    else if  (_mode == LOOP_LEVELS_SWEEP_DOWN_INIT)     {
      triggerLevelSweeps.init(-1);
      triggerLevelSweeps.speed_ = __param2;
      triggerLevelSweeps.offset_ = __param3;
    }

    else if  (_mode == LOOP_LEVELS_SWEEP_UP_INIT)       {
      triggerLevelSweeps.init(1);
      triggerLevelSweeps.speed_ = __param2;
      triggerLevelSweeps.offset_ = __param3;
    }

    else if  (_mode == LOOP_LEVELS_FADE_DOWN_INIT)      {
      triggerLevelFades.init(-1);
      triggerLevelFades.speed_ = __param2;
      triggerLevelFades.offset_ = __param3;
      triggerLevelFades.pulse_speed_ = __param4;
    }

    else if  (_mode == LOOP_LEVELS_FADE_UP_INIT)        {
      triggerLevelFades.init(1);
      triggerLevelFades.speed_ = __param2;
      triggerLevelFades.offset_ = __param3;
            triggerLevelFades.pulse_speed_ = __param4;

    }

    else if  (_mode == SPIRAL_LEVELS_SWEEP_DOWN_INIT)   {
      spiralLevelSweeps.init(-1);
      spiralLevelSweeps.speed_ = __param2;
      spiralLevelSweeps.offset_ = __param3;
//      spiralLevelSweeps.pulse_speed_ = __param4;
    }

    else if  (_mode == SPIRAL_LEVELS_SWEEP_UP_INIT)     {
      spiralLevelSweeps.init(1);
      spiralLevelSweeps.speed_ = __param2;
      spiralLevelSweeps.offset_ = __param3;
//      spiralLevelSweeps.pulse_speed_ = __param4;
    }

  }

  if (__mode != _mode) {
    Serial.print("Mode changed from "); Serial.print(_mode); Serial.print(" to: "); Serial.println(__mode);
    _mode = __mode;
    reset();
  }

  if (_mode == DARK) {

    //    Serial.println("DARK");
    clear_leds();

  } else if (_mode == TEST_CHASERS) {

    chasersTest.update_model();
    chasersTest.loop();

  } else if (_mode == LIGHT_SIDES) {

    if ((millis() % __param1) < 10) {

      clear_leds();
      lightSides.update_model(paletteCtl, __param7);
      lightSides.loop();

    }

  } else if (_mode == TWINKLE_STARS) {

    if ((millis() % __param1) < 2) {

      twinkleStars.update_model(paletteCtl, __param7);
      twinkleStars.loop();

    }

  } else if (_mode == RATS_IN_A_CAGE) {

    if ((millis() % __param1) < 10) {

      //      clear_leds();

      ratsInACage.update_model(paletteCtl, __param7);
      ratsInACage.loop();

    }

  } else if (_mode == ROTATE_BANDS) {

    if ((millis() % __param1) < 10) {

      rotateBands.loop(paletteCtl, __param7);

    }

  } else if (_mode == SNOWFALL) {

    if ((millis() % __param1) < 10) {

      snowfall.update_model(paletteCtl, __param7);
      snowfall.loop();

    }

  } else if (_mode == FAKE_SPECTRA) {

    if ((millis() % __param1) < 10) {

      fakeSpectra.update_model();
      fakeSpectra.loop();

    }

  } else if (_mode == TRIGGER_LEVELS_UP) {

    if ((millis() % __param1) < 10) {
      clear_leds();

      // up is down, down is up, peace is war
      triggerLevels.update_model(1, paletteCtl, __param7);
      triggerLevels.loop();
    }

  } else if (_mode == TRIGGER_LEVELS_DOWN) {

    if ((millis() % __param1) < 10) {
      clear_leds();

      triggerLevels.update_model(-1, paletteCtl, __param7);
      triggerLevels.loop();
    }

  } else if (_mode == LOOP_LEVELS_SWEEP_UP_INIT) {

    Serial.println("LOOP_LEVELS_SWEEP_UP_INIT");
    triggerLevelSweeps.init(1);

    __mode = LOOP_LEVELS_SWEEP_UP;

  } else if (_mode == LOOP_LEVELS_SWEEP_UP) {

    if ((millis() % __param1) < 10) {
      //     clear_leds();
      triggerLevelSweeps.update_model(1, paletteCtl, __param7);
      triggerLevelSweeps.loop();
    }

  } else if (_mode == LOOP_LEVELS_SWEEP_DOWN_INIT) {

    Serial.println("LOOP_LEVELS_SWEEP_DOWN_INIT");
    __mode = LOOP_LEVELS_SWEEP_DOWN;
    triggerLevelSweeps.init(-1);

  } else if (_mode == LOOP_LEVELS_SWEEP_DOWN) {

    if ((millis() % __param1) < 10) {
      //     clear_leds();
      triggerLevelSweeps.update_model(-1, paletteCtl, __param7);
      triggerLevelSweeps.loop();
    }



  } else if (_mode == SPIRAL_LEVELS_SWEEP_UP_INIT) {

    Serial.println("SPIRAL_LEVELS_SWEEP_UP_INIT");
    spiralLevelSweeps.init(1);

    __mode = SPIRAL_LEVELS_SWEEP_UP;

  } else if (_mode == SPIRAL_LEVELS_SWEEP_UP) {

    if ((millis() % __param1) < 10) {
      //     clear_leds();
      spiralLevelSweeps.update_model(1, paletteCtl, __param7);
      spiralLevelSweeps.loop();
    }

  } else if (_mode == SPIRAL_LEVELS_SWEEP_DOWN_INIT) {

    Serial.println("SPIRAL_LEVELS_SWEEP_DOWN_INIT");
    __mode = SPIRAL_LEVELS_SWEEP_DOWN;
    spiralLevelSweeps.init(-1);

  } else if (_mode == SPIRAL_LEVELS_SWEEP_DOWN) {

    if ((millis() % __param1) < 10) {
      //     clear_leds();
      spiralLevelSweeps.update_model(-1, paletteCtl, __param7);
      spiralLevelSweeps.loop();
    }


  } else if (_mode == LOOP_LEVELS_FADE_UP_INIT) {

    Serial.println("LOOP_LEVELS_FADE_UP_INIT");
    triggerLevelFades.init(1);

    __mode = LOOP_LEVELS_FADE_UP;

  } else if (_mode == LOOP_LEVELS_FADE_UP) {

    if ((millis() % __param1) < 10) {
      //     clear_leds();
      triggerLevelFades.update_model(1, paletteCtl, __param7);
      triggerLevelFades.loop();
    }

  } else if (_mode == LOOP_LEVELS_FADE_DOWN_INIT) {

    Serial.println("LOOP_LEVELS_FADE_DOWN_INIT");
    __mode = LOOP_LEVELS_FADE_DOWN;
    triggerLevelFades.init(-1);

  } else if (_mode == LOOP_LEVELS_FADE_DOWN) {

    if ((millis() % __param1) < 10) {
      //     clear_leds();
      triggerLevelFades.update_model(-1, paletteCtl, __param7);
      triggerLevelFades.loop();
    }

  }
}
