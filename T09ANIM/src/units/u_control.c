/* FILE NAME: u_control.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit CONTROLLER file in animation program.
 */

#include "units.h"

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
  /* Transforming camera position */
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdx));
    
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
      (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));
  if (Ani->Keys['F'] == 1)
    FlipFullScreen(Ani->hWnd);
  if (Ani->Keys['P'] == 1 && Ani->KeysClick['P'] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->Keys[VK_ESCAPE] == 1)
    SendMessage(Ani->hWnd, WM_DESTROY, 30, 0);

  if (Ani->JBut[JOY_BUTTON7] == 1)
    FlipFullScreen(Ani->hWnd);
  if (Ani->JBut[JOY_BUTTON2] == 1 && Ani->JButClick[JOY_BUTTON2] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->JBut[JOY_BUTTON8] == 1)
    SendMessage(Ani->hWnd, WM_DESTROY, 30, 0);
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
    (2 * (Ani->JY + Ani->JZ))));

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
