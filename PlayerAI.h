//A set of flags for the AI's state
//Bytes go as follows: AI_Enabled
byte AIState;

#define AI_ENABLED  0x01

byte AI_GetInput(void)
{
  return 0;
}

byte p2pad_poll(void)
{
  if(AIState&AI_ENABLED)
    return AI_GetInput();
  else
    return pad_poll(1);
}