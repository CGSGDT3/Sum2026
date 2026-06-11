/* FILE NAME: u_control.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit CONTROLLER file in animation program.
 */

#include "units.h"

#include <string.h> 
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Structure of controller unit */
typedef struct tagdt3UNIT_CONTROL
{
  UNIT_BASE_FIELDS;          /* Basic functions of unit */

  VEC CamLoc, CamDir, CamUp; /* Camera position */
  DBL Speed, AngleSpeed;     /* Angle and linear speed of camera */
} dt3UNIT_CONTROL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CONTROL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
  Uni->CamLoc = VecSet1(5), Uni->CamDir = VecSet1(1), Uni->CamUp = VecSet(0, 1, 0), Uni->Speed = 2, Uni->AngleSpeed = 12;
} /* End of 'DT3_UnitInit' function */ 

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CONTROL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
  POINT pt;
  INT i;

  /* Mouse responsing */
  GetCursorPos(&pt);
  ScreenToClient(Ani->hWnd, &pt);
  Ani->Mdx = pt.x - Ani->Mx;
  Ani->Mdy = pt.y - Ani->My;

  Ani->Mx = pt.x;
  Ani->My = pt.y;

  /* Transforming camera position */
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdx));
  
  /* KeyBoard responsing */
  GetKeyboardState(Ani->Keys);

  for (i = 0; i < 256; i++)
  {
    Ani->Keys[i] >>= 7;
    Ani->KeysClick[i] = Ani->Keys[i] && !Ani->KeysOld[i];
  }

  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
    (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));
  memcpy(Ani->KeysOld, Ani->Keys, 256); 

  if (Ani->Keys['F'] == 1)
    FlipFullScreen(Ani->hWnd);
  if (Ani->Keys['P'] == 1 && Ani->KeysClick['P'] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->Keys[VK_ESCAPE] == 1)
    SendMessage(Ani->hWnd, WM_DESTROY, 30, 0);

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
        Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
          (2 * (Ani->JY + Ani->JZ))));
        Ani->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }                            
    }
  }                    

  if (Ani->JBut[JOY_BUTTON1] == 1)
    FlipFullScreen(Ani->hWnd);
  if (Ani->JBut[JOY_BUTTON2] == 1 && Ani->JButClick[JOY_BUTTON2] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->JBut[JOY_BUTTON3] == 1)
    SendMessage(Ani->hWnd, WM_DESTROY, 30, 0);

  DT3_RndCamSet(Uni->CamLoc, Uni->CamDir, Uni->CamUp); 
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CONTROL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CONTROL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitClose' function */ 

/* Unit control creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateControl( VOID )
{
  dt3UNIT_CONTROL *Uni;

  if ((Uni = (dt3UNIT_CONTROL *)DT3_AnimUnitCreate(sizeof(dt3UNIT_CONTROL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateControl' function */ 

/* END OF 'u_control.c' FILE */
