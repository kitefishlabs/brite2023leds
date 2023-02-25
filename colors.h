#ifndef LEDS_COLOR
#define LEDS_COLOR

#define   RED     0
#define   ORANGE  32
#define   YELLOW  64
#define   GREEN   96
#define   AQUA    128
#define   BLUE    160
#define   PURPLE  192
#define   PINK    224


class LEDsPaletteController {
  
public:
  CHSVPalette16 currentPalette_;
  TBlendType    currentBlending_;

  LEDsPaletteController() {
    fill_solid( currentPalette_, 16, CHSV(HUE_BLUE, 64, 240));
    currentBlending_ = LINEARBLEND;
  }

  void setupCustomPalette(uint8_t hueA, uint8_t hueB, uint8_t sat, uint8_t valu) {
      for (int i=0; i<16; i++) {
        if (hueB >= hueA) {
          this->currentPalette_[i] = CHSV(hueA + (i * ((hueB - hueA) / 16)), sat, valu);
        } else {
          this->currentPalette_[i] = CHSV(hueB + (i * ((hueA - hueB) / 16)), sat, valu);
        }
      }
      this->currentBlending_ = NOBLEND;
  }

  void chooseColorPalette(int choice) {
    int choice_ = min(choice, 16);
      switch (choice_)
      {
      case 0:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_BLUE, 144, 220));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 1:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_RED, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 2:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_ORANGE, 166, 230));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 3:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_YELLOW, 186, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 4:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_GREEN, 156, 230));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 5:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_AQUA, 144, 220));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 6:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_BLUE, 172, 220));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 7:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_PURPLE, 162, 220));
        this->currentBlending_ = LINEARBLEND;
        break;
      
       case 8:
        this->setupCustomPalette(HUE_BLUE, HUE_GREEN, 180, 200);
         break;
//      
       case 9:
         this->setupCustomPalette(HUE_BLUE, HUE_YELLOW, 180, 200);
         break;

       case 10:
        this->setupCustomPalette(HUE_YELLOW, HUE_RED, 180, 200);
         break;
//      
       case 11:
         this->setupCustomPalette(HUE_RED, HUE_BLUE, 180, 200);
         break;

        case 12:
        this->setupCustomPalette(HUE_PURPLE, HUE_RED, 180, 200);
         break;
//      
       case 13:
         this->setupCustomPalette(HUE_GREEN, HUE_PURPLE, 180, 200);
         break;
      
      default:
      
      fill_solid( this->currentPalette_, 16, CHSV(HUE_BLUE, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
    }
  };
};

#endif
