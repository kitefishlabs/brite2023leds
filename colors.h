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

  void chooseColorPalette(int choice) {
    int choice_ = min(choice, 16);
      switch (choice_)
      {
      case 0:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_BLUE, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 1:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_RED, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 2:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_ORANGE, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 3:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_YELLOW, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 4:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_GREEN, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 5:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_AQUA, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 6:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_BLUE, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
      case 7:
        fill_solid( this->currentPalette_, 16, CHSV(HUE_PURPLE, 144, 240));
        this->currentBlending_ = LINEARBLEND;
        break;
      
//      // case 8:
////        fill_gradient( this->currentPalette_, 0, 32, 16, 64);
//      //   this->currentBlending_ = LINEARBLEND;
//      //   break;
//      
//      // case 9:
////      fill_gradient( this->currentPalette_, 0, BLUE, 16, GREEN);
////         this->currentBlending_ = LINEARBLEND;
//      //   break;
      
      default:
        break;
    }
  };
};

#endif
