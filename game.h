#define P1StartX 0x10
#define P1StartY 0x10

#define P2StartX 0x00
#define P2StartY 0x00

byte p1x;
byte p1y;

byte p2x;
byte p2y;

byte gamepad;

void start(void)
{
  p1x = P1StartX;
  p1y = P1StartY;
  
  p2x = P2StartX;
  p2y = P2StartY;
}

void update(void)
{
  gamepad = pad_poll(0);
  
  if(gamepad&PAD_UP) --p1y;
  if(gamepad&PAD_DOWN) ++p1y;
  
  if(gamepad&PAD_RIGHT) ++p1x;
  if(gamepad&PAD_LEFT) --p1x;
  
  ppu_wait_frame();
  
  oam_off = 0x00;
  
  oam_off = oam_spr(p1x, p1y, 0xb0, 0x01, oam_off);
  
  if(oam_off != 0) oam_hide_rest(oam_off);
}
