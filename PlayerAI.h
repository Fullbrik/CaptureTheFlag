//A set of flags for the AI's state
//Bytes go as follows: AI_Enabled
byte AIState;

#define AI_ENABLED  0x01


//The AI's current goal
byte GoalX, GoalY;

//The set of moves to get to that goal
byte currentMove;
byte Moves[];

byte InvalidLocationsX[];
byte InvalidLocationsY[];

void SetGoal(byte x, byte y)
{
  GoalX = x;
  GoalY = y;
  
  //Reset the currrent goal
  currentMove = 0;
  
  //Pathfind to that goal
}

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