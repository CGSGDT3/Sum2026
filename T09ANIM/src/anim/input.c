/* FILE NAME: input.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: input initialising and responsing fuctional file in animation program.
 */

#include "anim.h"
#include <string.h> 
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Initialising keyboard function.
 * ARGUMENTS:
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS:
 *   None.
 */
VOID DT3_KeyBoardInit( dt3ANIM *Ani )
{
  /* KeyBoard init */
} /* End of 'DT3_KeyBoardInit' function */

/* Responsing keyboard function.
 * ARGUMENTS:
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS:
 *   None.
 */
VOID DT3_KeyBoardResponse( dt3ANIM *Ani )
{
  INT i;

  /* KeyBoard responsing */
  GetKeyboardState(Ani->Keys);

  for (i = 0; i < 256; i++)
  {
    Ani->Keys[i] >>= 7;
    Ani->KeysClick[i] = Ani->Keys[i] && !Ani->KeysOld[i];
  }
}  /* End of 'DT3_KeyBoardResponse' function */

/* Initialising mouse function.
 * ARGUMENTS:
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS:
 *   None.
 */
VOID DT3_MouseInit( dt3ANIM *Ani )
{
  /* Mouse init */
} /* End of 'DT3_MouseInit' function */

/* Responsing mouse function.
 * ARGUMENTS:
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS:
 *   None.
 */
VOID DT3_MouseResponse( dt3ANIM *Ani )
{
  POINT pt;
  /* Mouse responsing */
      /* Mouse responsing */
  GetCursorPos(&pt);
  ScreenToClient(Ani->hWnd, &pt);
  Ani->Mdx = pt.x - Ani->Mx;
  Ani->Mdy = pt.y - Ani->My;
  Ani->Mx = pt.x;
  Ani->My = pt.y;
} /* End of 'DT3_MouseRespone' function */

/* Initialising keyboard function.
 * ARGUMENTS:
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS:
 *   None.
 */
VOID DT3_JoyStickInit( dt3ANIM *Ani )
{
  /* JoyStick init */
} /* End of 'DT3_JoyStickInit' function */

/* Responsing joystick function.
 * ARGUMENTS:
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS:
 *   None.
 */
VOID DT3_JoyStickResponse( dt3ANIM *Ani )
{        
  INT i;

  /* JoyStick responsing */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        for (i = 0; i < 32; i++)
        {
          Ani->JBut[i] = (ji.dwButtons >> i) & 1;
          Ani->JButClick[i] = Ani->JBut[i] && !Ani->JButOld[i];
          Ani->JButOld[i] = Ani->JBut[i];
        }

        Ani->JX = DT3_GET_JOYSTIC_AXIS(X);
        Ani->JY = DT3_GET_JOYSTIC_AXIS(Y);
        Ani->JZ = DT3_GET_JOYSTIC_AXIS(Z);
        Ani->JR = DT3_GET_JOYSTIC_AXIS(R);
        Ani->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }                            
    }
  } 
} /* End of 'DT3_JoyStickResponse' function */

/* Initialising animation input function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_AnimInputInit( VOID )
{
  /* Initialising all input */
  DT3_KeyBoardInit(&DT3_Anim);
  DT3_MouseInit(&DT3_Anim);
  DT3_JoyStickInit(&DT3_Anim);
} /* End of 'DT3_AnimInputInit' function */

/* Responsing animation input function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_AnimInputResponse( VOID )
{
  /* Responsing all input */
  DT3_MouseResponse(&DT3_Anim);
  DT3_KeyBoardResponse(&DT3_Anim);
  DT3_JoyStickResponse(&DT3_Anim);
} /* End of 'DT3_AnimInputResponse' function */

/* END OF 'input.c' FILE */