//Gun number 0 is no gun
#define GunCount 7

//Gun values
const byte GunSprites[GunCount] =
{
  0x00, 0xb3, 0xb5, 0xb7, 0xb8, 0xba, 0xbc
};

const byte GunProjectileSprites[GunCount] =
{
  0x00, 0xb4, 0xb6, 0xb6, 0xb9, 0xbb, 0xbd
};

const byte GunDamages[GunCount] =
{
  0, 2, 5, 4, 3, 9, 4
};

const byte GunSpeeds[GunCount] =
{
  0, 1, 5, 3, 3, 10, 2
};

const byte GunSpeedMod[GunCount] =
{
  5, 6, 7, 6, 10, 8, 7
};
