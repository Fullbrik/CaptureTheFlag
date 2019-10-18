//Define player starting positions
#define P1StartX 1
#define P1StartY 1

#define P2StartX 1
#define P2StartY 1

//Player variables

//Macros to get drawing pos
#define DRAWX(x) ((x + 1) * 8)
#define DRAWY(y) (DRAWX(y) - 1)

//All bools needed for game. 4 Bytes for each player Bytes go: Shot projectile, Picked up flag, Died, {TBD}.
byte GameState;

//Macros to help with game state
#define P1_SHOT 0x01
#define P1_FLAG 0x02
#define P1_DIED 0x04
#define P1__TBT 0x08

#define P2_SHOT 0x10
#define P2_FLAG 0x20
#define P2_DIED 0x40
#define P2__TBT 0x80

//Easy setting of value
#define SETGAMESTATETRUE(state) GameState = GameState|state
#define SETGAMESTATEFALSE(state) GameState = GameState&(~state)

//p1
//health
const byte p1maxHP = 1;
sbyte p1hp;

//location
byte p1x;
byte p1y;

//gun
byte p1gun;

//projectile location
byte p1x_proj;
byte p1y_proj;



//p2
//health
const byte p2maxHP = 1;
sbyte p2hp;

//location
byte p2x;
byte p2y;

//gun
byte p2gun;

//projectile location
byte p2x_proj;
byte p2y_proj;


//byte representing the current frame
byte currentFrame;

//The gamepad
byte gamepad;

//Gun values
const byte GunSprites[] =
{
  0x00, 0xb9
};

const byte GunProjectileSprites[] =
{
  0x00, 0xb9
};

//Map constants
#define MapCount 2
#define MapXSize 10
#define MapYSize 19

//All the maps
const byte Maps[MapCount][MapXSize][MapYSize] =
{
  {
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }
  },
  {
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }
  }
};

//Easy way to get a tile from the current map
#define MAPTILE(x, y) Maps[map][x][y]

//Check if position collides with map
#define COLLIDING(x, y) (MAPTILE(x, y))

//The map that has been chosen
byte map;

//Used with for loops / temp variables
byte i;
byte j;

//Easy do damage to target
void DoDamageToP1(byte damage)
{
  //Put the damage in one of the temp variables
  p1hp -= damage;
  
  
  if(p1hp <= 0)
  {
    p1hp = 0;
    
    SETGAMESTATETRUE(P1_DIED);
  }
}

void DoDamageToP2(byte damage)
{
  //Put the damage in one of the temp variables
  p2hp -= damage;
  
  if(p2hp <= 0)
  {
    p2hp = 0;
    
    SETGAMESTATETRUE(P2_DIED);
  }
}




void start(void)
{
  //Nothing happened in the the game yet, so let's tell the code that
  GameState = 0;
  
  //Draw board
  ppu_off();
  
  map = 0x00;
  
  for(i = 0x00; i < MapXSize; i++)
  {
    for(j = 0; j < MapYSize; j++)
    {
      vram_adr(NTADR_A(i + 1, j + 1));
      vram_put(Maps[map][i][j]);
    }
  }
  
  ppu_on_all();
  
  //Setup player starting stats
  p1hp = p1maxHP;
  
  p1x = P1StartX;
  p1y = P1StartY;
  
  p1gun = 1;
  
  
  p2hp = p2maxHP;
  
  p2x = P2StartX;
  p2y = P2StartY;
  
  p2gun = 1;
  
  ppu_wait_frame();
}

void update(void)
{
  ppu_wait_frame();
  
  /*Rendering*/
  
  //Render right when vblank starts
  oam_off = 0x00;
  
  
  //Render player 1
  oam_off = oam_spr(DRAWX(p1x), DRAWY(p1y), 0xb0, 0x01, oam_off);
  
  //Render his gun
  oam_off = oam_spr(DRAWX(p1x + 1), DRAWY(p1y), GunSprites[p1gun], 0x00, oam_off);
  
  //Render his projectile if needed
  if(GameState&P1_SHOT) oam_off = oam_spr(DRAWX(p1x_proj), DRAWY(p1y_proj), GunProjectileSprites[p1gun], 0x00, oam_off);
  
  
  //Render player 2
  oam_off = oam_spr(((p2x + 1) * 8), ((p2y + 1) * 8) - 1, 0xb0, 0x42, oam_off);
  
  //Render his gun
  oam_off = oam_spr(((p2x) * 8), ((p2y + 1) * 8) - 1, GunSprites[p2gun], 0x40, oam_off);
  
  //Render his projectile if needed
  if(GameState&P2_SHOT) oam_off = oam_spr(DRAWX(p2x_proj), DRAWY(p2y_proj), GunProjectileSprites[p2gun], 0x40, oam_off);
  
  
  
  if(oam_off != 0) oam_hide_rest(oam_off);
  
  
  
  
  
  
  
  
  
  //Incrament the current frame to see what frame we are on
  ++currentFrame;
  
  
  
  
  
  
  
  
  
  
  /*Input*/
  //p1 input
  if(GameState&P1_DIED)
  {
  }
  else
  {
    gamepad = pad_poll(0);
  
    if(gamepad&PAD_A && p1gun && !GameState&P1_SHOT)
    {
      p1x_proj = p1x + 2;
      p1y_proj = p1y;
    
      SETGAMESTATETRUE(P1_SHOT);
    }
  
    if(currentFrame == 3 || currentFrame == 6)
    {
      //Do a Mele attack if we are close enough
      if(gamepad&PAD_B)
      {
        //Make sure the opponent is in front of us
        if(p2y == p1y && p2x == p1x + 1)
        {
          DoDamageToP2(1);
        }
      }
    }
  
    //We will only move every 6 frames, this way we slow down movement
    if(currentFrame == 6)
    {
      //Move p1
      if(gamepad&PAD_UP && !COLLIDING(p1x, p1y - 1)) --p1y;
      if(gamepad&PAD_DOWN && !COLLIDING(p1x, p1y + 1)) ++p1y;
  
      if(gamepad&PAD_RIGHT && !COLLIDING(p1x + 1, p1y)) ++p1x;
  
      if(gamepad&PAD_LEFT && !COLLIDING(p1x - 1, p1y)) --p1x;
    }
  }
  
  

  
  
  //p2 input
  if(GameState&P2_DIED)
  {
    
  }
  else
  {
    gamepad = pad_poll(1);
  
    if(gamepad&PAD_A && p2gun && !((GameState&P2_SHOT) >> 4))
    {
      p2x_proj = p2x - 2;
      p2y_proj = p2y;
    
      SETGAMESTATETRUE(P2_SHOT);
    }
  
    if(currentFrame == 3 || currentFrame == 6)
    {
      //Do a Mele attack if we are close enough
      if(gamepad&PAD_B)
      {
        //Make sure the opponent is in front of us
        if(p1y == p2y && p1x == p2x - 1)
        {
          DoDamageToP1(1);
        }
      }
    }
  
    //We will only move every 6 frames, this way we slow down movement
    if(currentFrame == 6)
    {
      //Move p2
      if(gamepad&PAD_UP && !COLLIDING(p2x, p2y - 1)) --p2y;
      if(gamepad&PAD_DOWN && !COLLIDING(p2x, p2y + 1)) ++p2y;
  
      if(gamepad&PAD_RIGHT && !COLLIDING(p2x + 1, p2y)) ++p2x;
      if(gamepad&PAD_LEFT && !COLLIDING(p2x - 1, p2y)) --p2x;
    }
  }
  
  
  
  
  
  
  /*Move the projectiles*/
  if(currentFrame == 5)
  {
    
    //Start with p1
    if(GameState&P1_SHOT)
    {
      //If we collided, stop the projectile
      if(COLLIDING(p1x_proj + 1, p1y_proj))
      {
        SETGAMESTATEFALSE(P1_SHOT);
      }
      else if(p1x_proj == p2x && p1y_proj == p2y)
      {
        SETGAMESTATEFALSE(P1_SHOT);
      }
      else
      {
        ++p1x_proj;
      }
    }
    
    //Then do p2
    if(GameState&P2_SHOT)
    {
      //If we collided, stop the projectile
      if(COLLIDING(p2x_proj - 1, p2y_proj))
      {
        SETGAMESTATEFALSE(P2_SHOT);
      }
      else if(p2x_proj == p1x && p2y_proj == p1y)
      {
        SETGAMESTATEFALSE(P1_SHOT);
      }
      else
      {
        --p2x_proj;
      }
    }
  }
  
  
  //Reset current frame when the max is hit
  if(currentFrame == 6)
  {
    currentFrame = 0;
  }
  
}
