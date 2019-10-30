//Macros to get drawing pos
#define DRAWX(x) ((x + 1) * 8)
#define DRAWY(y) (DRAWX(y) - 1)

//All bools needed for game. 4 Bytes for each player Bytes go: Shot projectile, Picked up flag, Died, Won the game.
byte GameState;

//Macros to help with game state
#define P1_SHOT 0x01
#define P1_FLAG 0x02
#define P1_DIED 0x04
#define P1_CAPD 0x08

#define P2_SHOT 0x10
#define P2_FLAG 0x20
#define P2_DIED 0x40
#define P2_CAPD 0x80

//Easy setting of value
#define SETGAMESTATETRUE(state) GameState = GameState|state
#define SETGAMESTATEFALSE(state) GameState = GameState&(~state)

//Player variables

//Max health for a player
#define MAXHP 9

//p1
//health
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

//byte representing how much time the player should be dead for
const byte RespawnTime = 120;

//byte representing how much time the player has been dead for
byte respawnCountdown;



//Gun pickup location and type
byte pickup_gunX;
byte pickup_gunY;
byte pickup_gunType;

//The next gun type to set the pickup to be
byte nextGunType;

//Simple macro to set gun pickup without having to load a function into memory
#define SETGUNPICKUP(type, x, y) pickup_gunType = type; pickup_gunX = x; pickup_gunY = y

//Simple macro to tell the game we picked up the gun
#define PICKUPGUN(player) p ## player ## gun = pickup_gunType; SETGUNPICKUP(0, 0, (rand() % 50) + 200)

//The gun's x is also used to count down until the gun needs to respawn, so we rename it here
#define pickup_gunRespawnCounter pickup_gunX

//The amount of time until the gun needs to respawn is stored in the y
#define pickup_gunRespawnTime pickup_gunY

//Easy way to get a tile from the current map
#define MAPTILE(x, y) Maps[map][x][y]

//Check if position collides with map
#define COLLIDING(x, y) (MAPTILE(x, y))

//Easy do damage to target
void DoDamageToP1(byte damage)
{
  //Put the damage in one of the temp variables
  p1hp -= damage;
  
  
  if(p1hp <= 0)
  {
    p1hp = 0;
    
    SETGAMESTATETRUE(P1_DIED);
    SETGAMESTATEFALSE(P1_FLAG);
    
    p1gun = 0;
    
    respawnCountdown = 0;
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
    SETGAMESTATEFALSE(P2_FLAG);
    
    p2gun = 0;
    
    respawnCountdown = 0;
  }
}




void start(void)
{
  //Nothing happened in the the game yet, so let's tell the code that
  GameState = 0;
  
  //Draw board
  ppu_off();
  
  vram_adr(NTADR_A(0, 1));
  vram_put(1);
  
  for(i = 0x00; i < MapXSize; ++i)
  {
    for(j = 0; j < MapYSize; ++j)
    {
      vram_adr(NTADR_A(i + 1, j + 1));
      vram_put(Maps[map][i][j]);
    }
  }
  
  vram_adr(NTADR_A(3, 1));
  vram_write("Red Health: ", 12);
  
  vram_adr(NTADR_A(16, 1));
  vram_write("Blue Health: ", 13);
  
  ppu_on_all();
  
  SETGUNPICKUP(1, (MapXSize >> 1) - 1, MapYSize / 2);
  
  //Setup player starting stats
  p1hp = MAXHP;
  
  p1x = P1StartX;
  p1y = P1StartY;
  
  p1gun = 0;
  
  
  p2hp = MAXHP;
  
  p2x = P2StartX;
  p2y = P2StartY;
  
  p2gun = 0;
  
  
  
  
  nextGunType = 1;
}

void update(void)
{
  //Render right when vblank starts
  ppu_wait_frame();
  
  /*Rendering*/
  
  //Reset the oam_off
  oam_off = 0x00;
  
  
  
  //Render the gun pickup if one is on the board
  if(pickup_gunType != 0) oam_off = oam_spr(DRAWX(pickup_gunX), DRAWY(pickup_gunY), GunSprites[pickup_gunType], 0x00, oam_off);
  
  
  
  
  
  if(!(GameState&P1_DIED))
  {
    //Render player 1
    oam_off = oam_spr(DRAWX(p1x), DRAWY(p1y), 0xb0, 0x01, oam_off);
    
  
    //Render his gun, or his knive
    gamepad = pad_poll(0);
    
    if(gamepad&PAD_B)
      oam_off = oam_spr(DRAWX(p1x + 1), DRAWY(p1y), 0xc0, 0x00, oam_off);
    else 
      oam_off = oam_spr(DRAWX(p1x + 1), DRAWY(p1y), GunSprites[p1gun], 0x00, oam_off);
  }
  
  //Render his health
  oam_off = oam_spr(DRAWX(13), DRAWY(0), NumSprites[p1hp], 0x00, oam_off);
  
  
  //Render his projectile if needed
  if(GameState&P1_SHOT) oam_off = oam_spr(DRAWX(p1x_proj), DRAWY(p1y_proj), GunProjectileSprites[p1gun], 0x00, oam_off);
  
  //Render his flag if it is not picked up, and if it is, render a spot for it to go on
  if(!(GameState&P2_FLAG)) oam_off = oam_spr(DRAWX(p1x_flag), DRAWY(p1y_flag), 0xb1, 0x01, oam_off);
  else oam_off = oam_spr(DRAWX(p1x_flag), DRAWY(p1y_flag), 0xb2, 0x01, oam_off);
  
  
  
  
  
  if(!(GameState&P2_DIED))
  {
    //Render player 2
    oam_off = oam_spr(((p2x + 1) * 8), ((p2y + 1) * 8) - 1, 0xb0, 0x42, oam_off);
    
  
    //Render his gun, or his knive
    gamepad = pad_poll(1);
    
    if(gamepad&PAD_B)
      oam_off = oam_spr(((p2x) * 8), ((p2y + 1) * 8) - 1, 0xc0, 0x40, oam_off);
    else
      oam_off = oam_spr(((p2x) * 8), ((p2y + 1) * 8) - 1, GunSprites[p2gun], 0x40, oam_off);
  }
  
  //Render his health
  oam_off = oam_spr(DRAWX(27), DRAWY(0), NumSprites[p2hp], 0x00, oam_off);
  
  //Render his projectile if needed
  if(GameState&P2_SHOT) oam_off = oam_spr(DRAWX(p2x_proj), DRAWY(p2y_proj), GunProjectileSprites[p2gun], 0x40, oam_off);
  
  //Render his flag if it is not picked up, and if it is, render a spot for it to go on
  if(!(GameState&P1_FLAG)) oam_off = oam_spr(DRAWX(p2x_flag), DRAWY(p2y_flag), 0xb1, 0x42, oam_off);
  else oam_off = oam_spr(DRAWX(p2x_flag), DRAWY(p2y_flag), 0xb2, 0x42, oam_off);
  
  
  
  
  if(oam_off != 0) oam_hide_rest(oam_off);
  
  
  
  
  
  
  
  
  
  //Incrament the current frame to see what frame we are on
  ++currentFrame;
  
  
  
  
  
  
  
  
  
  
  /*Input*/
  //p1 input
  if(GameState&P1_DIED)
  {
    ++respawnCountdown;
    
    if(respawnCountdown > RespawnTime)
    {
      p1x = P1StartX;
      p1y = P1StartY;
      
      SETGAMESTATEFALSE(P1_DIED);
      
      p1hp = MAXHP;
    }
  }
  else
  {
    gamepad = pad_poll(0);
  
    if(gamepad&PAD_A && !(gamepad&PAD_B) && p1gun && !(GameState&P1_SHOT))
    {
      p1x_proj = p1x + 2;
      p1y_proj = p1y;
    
      SETGAMESTATETRUE(P1_SHOT);
    }
  
    if(!(currentFrame % 4))
    {
      //Do a Mele attack if we are close enough
      if(gamepad&PAD_B)
      {
        //Make sure the opponent is in front of us
        if(p2y == p1y && p2x == p1x + 1)
        {
          DoDamageToP2(3);
        }
      }
    }
  
    //We will only move every 6 frames, this way we slow down movement
    if(!(currentFrame % GunSpeedMod[p1gun]))
    {
      //Move p1
      if(gamepad&PAD_UP && !COLLIDING(p1x, p1y - 1) && !(p1x == p2x && p1y - 1 == p2y)) --p1y;
      if(gamepad&PAD_DOWN && !COLLIDING(p1x, p1y + 1) && !(p1x == p2x && p1y + 1 == p2y)) ++p1y;
  
      if(gamepad&PAD_RIGHT && !COLLIDING(p1x + 1, p1y) && !(p1x + 1 == p2x && p1y == p2y)) ++p1x;
  
      if(gamepad&PAD_LEFT && !COLLIDING(p1x - 1, p1y) && !(p1x - 1 == p2x && p1y == p2y)) --p1x;
      
      //See if we have the opponents flag. If we do and are standing on the cap location, and the opponent doesn't have our flag, cap the flag
      if(GameState&P1_FLAG)
      {
        if(p1x == p1x_flag && p1y == p1y_flag && !(GameState&P2_FLAG))
        {
          SETGAMESTATEFALSE(P1_FLAG);
          SETGAMESTATETRUE(P1_CAPD);
        }
      }
      else
      {
        if(p1x == p2x_flag && p1y == p2y_flag)
        {
          SETGAMESTATETRUE(P1_FLAG);
        }
      }
      
      //If we are standing on the pickup, then pickup the gun
      if(pickup_gunType && p1x == pickup_gunX && p1y == pickup_gunY)
      {
        PICKUPGUN(1);
      }
    }
  }
  
  

  
  
  //p2 input
  if(GameState&P2_DIED)
  {
    ++respawnCountdown;
    
    if(respawnCountdown > RespawnTime)
    {
      p2x = P2StartX;
      p2y = P2StartY;
      
      SETGAMESTATEFALSE(P2_DIED);
      
      p2hp = MAXHP;
    }
  }
  else
  {
    gamepad = pad_poll(1);
  
    if(gamepad&PAD_A && !(gamepad&PAD_B) && p2gun && !((GameState&P2_SHOT) >> 4))
    {
      p2x_proj = p2x - 2;
      p2y_proj = p2y;
    
      SETGAMESTATETRUE(P2_SHOT);
    }
  
    if(!(currentFrame % 4))
    {
      //Do a Mele attack if we are close enough
      if(gamepad&PAD_B)
      {
        //Make sure the opponent is in front of us
        if(p1y == p2y && p1x == p2x - 1)
        {
          DoDamageToP1(3);
        }
      }
    }
  
    //We will only move every 6 frames, this way we slow down movement
    if(!(currentFrame % GunSpeedMod[p2gun]))
    {
      //Move p2
      if(gamepad&PAD_UP && !COLLIDING(p2x, p2y - 1) && !(p2x == p1x && p2y - 1 == p1y)) --p2y;
      if(gamepad&PAD_DOWN && !COLLIDING(p2x, p2y + 1) && !(p2x == p1x && p2y + 1 == p1y)) ++p2y;
  
      if(gamepad&PAD_RIGHT && !COLLIDING(p2x + 1, p2y) && !(p2x + 1 == p1x && p2y == p1y)) ++p2x;
      if(gamepad&PAD_LEFT && !COLLIDING(p2x - 1, p2y) && !(p2x - 1 == p1x && p2y == p1y)) --p2x;
      
      if(GameState&P2_FLAG)
      {
        if(p2x == p2x_flag && p2y == p2y_flag && !(GameState&P1_FLAG))
        {
          SETGAMESTATEFALSE(P2_FLAG);
          SETGAMESTATETRUE(P2_CAPD);
        }
      }
      else
      {
        if(p2x == p1x_flag && p2y == p1y_flag)
        {
          SETGAMESTATETRUE(P2_FLAG);
        }
      }
      
      if(pickup_gunType && p2x == pickup_gunX && p2y == pickup_gunY)
      {
        PICKUPGUN(2);
      }
    }
  }
  
  
  
  
  
  
  /*Move the projectiles*/
  if(GunSpeeds[p1gun] && !(currentFrame % GunSpeeds[p1gun]))
  {
    
    //Start with p1
    if(GameState&P1_SHOT)
    {
      ++p1x_proj;
      
      //If we collided, stop the projectile
      if(p1x_proj == p2x && p1y_proj == p2y)
      {
        SETGAMESTATEFALSE(P1_SHOT);
        
        DoDamageToP2(GunDamages[p1gun]);
      }
      else if(COLLIDING(p1x_proj + 1, p1y_proj))
      {
        SETGAMESTATEFALSE(P1_SHOT);
      }
    }
  }
  
  if(GunSpeeds[p2gun] && !(currentFrame % GunSpeeds[p2gun]))
  {
    //Then do p2
    if(GameState&P2_SHOT)
    {
      --p2x_proj;
      
      //If we collided, stop the projectile
      if(p2x_proj == p1x && p2y_proj == p1y)
      {
        SETGAMESTATEFALSE(P2_SHOT);
        
        DoDamageToP1(GunDamages[p2gun]);
      }
      else if(COLLIDING(p2x_proj - 1, p2y_proj))
      {
        SETGAMESTATEFALSE(P2_SHOT);
      }
    }
  }
  
  
  
  
  /*Respawn the gun pickup*/
  if(!pickup_gunType)
  {
    ++pickup_gunRespawnCounter;
    
    if(pickup_gunRespawnCounter >= pickup_gunRespawnTime)
    {
      pickup_gunX = MapXSize / 2 - (rand() % 2);
      pickup_gunY = MapYSize / 2;
      
      ++nextGunType;
      
      if(nextGunType >= GunCount)
        nextGunType = 0;
      
      pickup_gunType = nextGunType;
    }
  }
  
}
