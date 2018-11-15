/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"
#include <stdio.h>

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
//static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

  LedPWM(RED0,   LED_PWM_100);
  LedPWM(GREEN0, LED_PWM_0);
  LedPWM(BLUE0,  LED_PWM_0);

  LedPWM(RED1,   LED_PWM_100);
  LedPWM(GREEN1, LED_PWM_0);
  LedPWM(BLUE1,  LED_PWM_0);

  LedPWM(RED2,   LED_PWM_100);
  LedPWM(GREEN2, LED_PWM_0);
  LedPWM(BLUE2,  LED_PWM_0);

  LedPWM(RED3,   LED_PWM_100);
  LedPWM(GREEN3, LED_PWM_0);
  LedPWM(BLUE3,  LED_PWM_0);    

  
} /* end UserApp2Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp2SM_Idle(void)
{
  #ifdef MPG2
    static LedNumberType aeCurrentLed[]  = {GREEN0, RED0, BLUE0, GREEN0, RED0, BLUE0};
    static LedNumberType aeCurrentLed1[] = {GREEN1, RED1, BLUE1, GREEN1, RED1, BLUE1};
    static LedNumberType aeCurrentLed2[] = {GREEN2, RED2, BLUE2, GREEN2, RED2, BLUE2};
    static LedNumberType aeCurrentLed3[] = {GREEN3, RED3, BLUE3, GREEN3, RED3, BLUE3};
  #endif
    
  static u8 u8CurrentLedIndex  = 0;
  static u8 u8LedCurrentLevel  = 0;
  static u8 u8DutyCycleCounter = 0;
  static u16 u16Counter = COLOR_CYCLE_TIME;
  static bool abLedRateIncreasing[]   = {TRUE,      FALSE,   TRUE,     FALSE,     TRUE,    FALSE};
  static bool bCyclingOn = TRUE;
   
  static u16 u16BlinkCount = 0;
  static u8 u8Counter = 0;
  static u8 u8ColorIndex = 0;
   
  if(bCyclingOn)
  {
    u16Counter--;
  }
  
  /* Check for update color every COLOR_CYCLE_TIME ms */  
  if(u16Counter == 0)
  {
    u16Counter = COLOR_CYCLE_TIME;

    /* Update the current level based on which way it's headed */

    /* Update the value to the current LED */   
    LedPWM( (LedNumberType)aeCurrentLed[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
    LedPWM( (LedNumberType)aeCurrentLed1[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
    LedPWM( (LedNumberType)aeCurrentLed2[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
    LedPWM( (LedNumberType)aeCurrentLed3[u8CurrentLedIndex], (LedRateType)u8LedCurrentLevel);
  }
  
  if( abLedRateIncreasing[u8CurrentLedIndex] )
    {
      u8LedCurrentLevel++;
    }
    else
    {
      u8LedCurrentLevel--;
    }

    /* Change direction once we're at the end */
    u8DutyCycleCounter++;
    if(u8DutyCycleCounter == 20)
    {
      u8DutyCycleCounter = 0;
      
      /* Watch for the indexing variable to reset */
      u8CurrentLedIndex++;
      if(u8CurrentLedIndex == sizeof(aeCurrentLed))
      {
        u8CurrentLedIndex = 0;
      }
      
      /* Set the current level based on what direction we're now going */
      u8LedCurrentLevel = 20;
      if(abLedRateIncreasing[u8CurrentLedIndex])
      {
         u8LedCurrentLevel = 0;
      }
    }
      
    /* Toggle cycling on and off */
  if( WasButtonPressed(BUTTON0) )
  {
    ButtonAcknowledge(BUTTON0);
    bCyclingOn = (bool)!bCyclingOn;
  }
  
# if 0
  u16BlinkCount++;
  
  if(u16BlinkCount == 500)
  {
    u16BlinkCount = 0;
    
    /* Update the counter and roll at 16 */
    u8Counter++;
    if(u8Counter == 16)
    {
      u8Counter = 0;
      
      LedOff((LedNumberType)(RED3 + (4 * u8ColorIndex)));
      LedOff((LedNumberType)(RED2 + (4 * u8ColorIndex)));
      LedOff((LedNumberType)(RED1 + (4 * u8ColorIndex)));
      LedOff((LedNumberType)(RED0 + (4 * u8ColorIndex)));
      
      u8ColorIndex++;
      if(u8ColorIndex == 3)
      {
        u8ColorIndex = 0;
      }
    }
  }
    if(u8Counter & 0x01)
    {
      LedOn(RED3 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(RED3 + (4 * u8ColorIndex));
    }

    if(u8Counter & 0x02)
    {
      LedOn(RED2 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(RED2 + (4 * u8ColorIndex));
    }

    if(u8Counter & 0x04)
    {
      LedOn(RED1 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(RED1 + (4 * u8ColorIndex));
    }

    if(u8Counter & 0x08)
    {
      LedOn(RED0 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(RED0 + (4 * u8ColorIndex));
    }
#endif
} /* end UserApp2SM_Idle() */

#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} /* end UserApp2SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
