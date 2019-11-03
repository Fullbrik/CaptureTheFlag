
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// BCD arithmetic support
#include "bcd.h"
//#link "bcd.c"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

#include "apu.h"
//#link "apu.c"

//Used with for loops / temp variables
byte i;
byte j;

//The gamepad
byte gamepad;

//The map that has been chosen
byte map;


#include "Maps.h"



#include "Guns.h"
#include "NumSprites.h"
#include "game.h"

#include "mainMenu.h"


#include "WinScreen.h"



/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x00,			// screen color

  0x0F,0x30,0x0F,0x00,	// background palette 0
  0x11,0x30,0x16,0x00,	// background palette 1
  0x00,0x10,0x20,0x00,	// background palette 2
  0x06,0x16,0x26,0x00,   // background palette 3

  0x0F,0x2D,0x30,0x00,	// sprite palette 0
  0x0F,0x2D,0x16,0x00,	// sprite palette 1
  0x0F,0x2D,0x02,0x00,	// sprite palette 2
  0x0F,0x30,0x0F	// sprite palette 3
};

// setup PPU and tables
void setup_graphics(void) {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
}

void game(void)
{
  start();
  
  while(1)
  {
    if(GameState&P1_CAPD)
    {
      update();
      ppu_wait_frame();
      break;
    }
    
    if(GameState&P2_CAPD)
    {
      update();
      ppu_wait_frame();
      break;
    }
    
    update();
  }
}

void main(void)
{
  apu_init();
  
  setup_graphics();
  // enable rendering
  ppu_on_all();
  
  while(1)
  {
    ppu_off();
    
    pal_all(PALETTE);
    
    oam_clear();
    
    //MainMenu();
    
    oam_clear();
    
    game();
    
    WinScreen();
    
    for(i = 0; i < 255; i++)
    {
      ppu_wait_frame();
      
      gamepad = pad_poll(0) | pad_poll(1);
      
      if(gamepad&PAD_START) break;
    }
  }
}
