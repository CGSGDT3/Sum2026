/* FILE NAME: u_control.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit CONTROLLER file in animation program.
 */

#include <stdio.h>

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

/* Camera transformation function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CONTROL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID CamTransf( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
  FLT sx, sy, Dist, Azimuth, Elevator, cosT, sinT, plen, cosP, sinP;
  VEC dv, NewLoc;
  INT Wp, Hp;
  Hp = Wp = DT3_RndProjSize;           
 
  DT3_RndCamLoc = VecAddVec(DT3_RndCamLoc, VecMulNum(VecNormalize(VecSubVec(DT3_RndCamAt, DT3_RndCamLoc)), Ani->DeltaTime * Uni->Speed *
      (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));
  Dist = VecLen(VecSubVec(DT3_RndCamAt, DT3_RndCamLoc));

  cosT = (DT3_RndCamLoc.Y - DT3_RndCamAt.Y) / Dist;
  sinT = sqrt(1 - cosT * cosT);

  plen = Dist * sinT;

  cosP = (DT3_RndCamLoc.Z - DT3_RndCamAt.Z) / plen;
  sinP = (DT3_RndCamLoc.X - DT3_RndCamAt.X) / plen;

  Azimuth = R2D(atan2(sinP, cosP));
  Elevator = R2D(acos(cosT));

  Azimuth += Ani->GlobalDeltaTime * 47 * (Ani->JX + Ani->JR + Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])
    - 4 * Ani->Keys[VK_LBUTTON] * Ani->Mdx;

  Elevator += Ani->GlobalDeltaTime * 47 * (Ani->JY + Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])
    - 4 * Ani->Keys[VK_RBUTTON] * Ani->Mdy;

   Dist += Ani->GlobalDeltaTime * (Ani->Mdz + 8 * (1 + Ani->Keys[VK_SHIFT] * 28)
     * (Ani->JZ + Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));

  if (Elevator < 0.08)
    Elevator = 0.08;
  if (Elevator > 178)
    Elevator = 178;
  if (Dist < 0.01)
    Dist = 0.01;

  NewLoc = PointTransform(VecSet(0, Dist, 0), MatrMulMatr3(MatrRotateX(Elevator),
    MatrRotateY(Azimuth),
    MatrTranslate(DT3_RndCamAt)));             

  NewLoc = VecAddVec(NewLoc, DT3_RndCamAt);  

  if (Ani->W > Ani->H)
    Wp *= (FLT)Ani->W / Ani->H;
  else
    Hp *= (FLT)Ani->H / Ani->W;        

  sx = Ani->Keys[VK_RBUTTON] * -Ani->Mdx * Wp / Ani->W * Dist / DT3_RndProjDist;
  sy = Ani->Keys[VK_RBUTTON] * -Ani->Mdy * Hp / Ani->H * Dist / DT3_RndProjDist;                 
  
  dv = VecAddVec(VecMulNum(DT3_RndCamRight, sx),
    VecMulNum(DT3_RndCamUp, sy));

  DT3_RndCamAt = VecAddVec(DT3_RndCamAt, dv);
  DT3_RndCamLoc = VecAddVec(DT3_RndCamLoc, dv);             

  DT3_RndCamSet(PointTransform(VecSet(0, Dist, 0), MatrMulMatr3(MatrRotateX(Elevator),
    MatrRotateY(Azimuth), MatrTranslate(DT3_RndCamAt))),
    DT3_RndCamAt,
    VecSet(0, 1, 0));
}

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
  INT modes[2];
  CHAR buf[100];

  /* Transforming camera position */
/*  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdx));
    
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
      (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));*/
  CamTransf(Uni, Ani);
  if (Ani->Keys['F'] == 1  && Ani->KeysClick['F'] == 1)
    FlipFullScreen(Ani->hWnd);
  if (Ani->Keys['P'] == 1 && Ani->KeysClick['P'] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->Keys[VK_ESCAPE] == 1)
    SendMessage(Ani->hWnd, WM_DESTROY, 30, 0);

  if (Ani->Keys[VK_SHIFT] == 1 && Ani->KeysClick['W'] == 1)
  {
    glGetIntegerv(GL_POLYGON_MODE, modes);

    if (modes[0] == GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  if (Ani->KeysClick['1'])
    glLineWidth(1);
  if (Ani->KeysClick['2'])
    glLineWidth(2);
  if (Ani->KeysClick['3'])
    glLineWidth(3);

  if (Ani->KeysClick[VK_F8])
  {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD NumCharsWritten;
    COORD Pos = {0};

    GetConsoleScreenBufferInfo(hCon, &info);
    FillConsoleOutputCharacter(hCon, ' ',
      (UINT)(info.dwSize.X * info.dwSize.Y), Pos, &NumCharsWritten);  
    FillConsoleOutputAttribute(hCon, 0x0F,
      (UINT)(info.dwSize.X * info.dwSize.Y), Pos, &NumCharsWritten);  
    SetConsoleCursorPosition(hCon, info.dwCursorPosition);
    SetConsoleCursorPosition(hCon, Pos);
  }

  sprintf(buf, "CGSG FOREVER!!!\nFPS: %f.", DT3_Anim.FPS);
  DT3_RndFntDraw(buf, VecSet1(10), 10);

  if (Ani->JBut[JOY_BUTTON7] == 1)
    FlipFullScreen(Ani->hWnd);
  if (Ani->JBut[JOY_BUTTON2] == 1 && Ani->JButClick[JOY_BUTTON2] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->JBut[JOY_BUTTON8] == 1)
    SendMessage(Ani->hWnd, WM_DESTROY, 30, 0);
/*  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
    (2 * (Ani->JY + Ani->JZ))));


  DT3_RndCamSet(Uni->CamLoc, Uni->CamDir, Uni->CamUp);     */
} /* End of 'DT3_UnitResponse' function */ 

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
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateControl' function */ 

/* END OF 'u_control.c' FILE */
