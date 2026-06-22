/* FILE NAME: u_g3dm.c
 * PROGRAMMER: DT3
 * DATE: 18.06.2026
 * PURPOSE: unit model file in animation program.
 */ 

#include <time.h>

#include "units.h"

/* Structure of ball unit */
typedef struct tagdt3UNIT_G3DM
{
  UNIT_BASE_FIELDS;                 /* Basic unit functions */ 
  dt3PRIMS Model;                   /* Primitives to drawing */
  dt3PRIM Cylinder;                 /* Cyllinder of Earth */
  FLT SteerAngle, CylAngle, len, CylSpeed, VerticalPos, CarSpeed, Heading; 
} dt3UNIT_G3DM;

/* Unit car rotation by buttons function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_G3DM *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitCarRotation( dt3UNIT_G3DM *Uni, dt3ANIM *Ani )
{
  FLT phi_max = 35, Aphi = 180, Acyll = 1, MaxCyl = 30;

  if (Ani->Keys['D'])
    Uni->SteerAngle += Aphi * Ani->DeltaTime;
  if (Ani->Keys['A'])
    Uni->SteerAngle -= Aphi * Ani->DeltaTime;      
  if (Uni->SteerAngle > phi_max)
    Uni->SteerAngle = phi_max;
  if (Uni->SteerAngle < -phi_max)
    Uni->SteerAngle = -phi_max; 
  if (!Ani->Keys['D'] && !Ani->Keys['A'])
  {
    Uni->SteerAngle *= 0.95;
    if (fabs(Uni->SteerAngle) < 0.1)
      Uni->SteerAngle = 0;
  }

  Uni->Heading += Uni->CarSpeed * Ani->DeltaTime * tan(D2R(Uni->SteerAngle));

  if (Ani->Keys['W'])    
  {
    Uni->CylSpeed += Acyll * cos(Uni->Heading) * Ani->DeltaTime;
    Uni->CarSpeed += Acyll * Ani->DeltaTime;
  }
  if (Ani->Keys['S'])
  {
    Uni->CylSpeed -= 2 * Acyll * cos(Uni->Heading) * Ani->DeltaTime;
    Uni->CarSpeed -= 2 * Acyll * Ani->DeltaTime;
  }

  if (!Ani->Keys['W'] && !Ani->Keys['S'])
  {
    if (Uni->CylSpeed > 0)
    {
      Uni->CylSpeed -= 10 * Ani->DeltaTime;
      if (Uni->CylSpeed < 0)
        Uni->CylSpeed = 0;
    }
    else if (Uni->CylSpeed < 0)
    {
      Uni->CylSpeed += 10 * Ani->DeltaTime;
      if (Uni->CylSpeed > 0)
        Uni->CylSpeed = 0;
    }

    if (Uni->CarSpeed > 0)
    {
      Uni->CarSpeed -= 10 * Ani->DeltaTime;
      if (Uni->CarSpeed < 0)
        Uni->CarSpeed = 0;
    }
    else if (Uni->CarSpeed < 0)
    {
      Uni->CarSpeed += 10 * Ani->DeltaTime;
      if (Uni->CarSpeed > 0)
        Uni->CarSpeed = 0;
    }
    if (fabs(Uni->CarSpeed) < 0.08)
        Uni->CarSpeed = 0;
    if (fabs(Uni->CylSpeed) < 0.08)
        Uni->CylSpeed = 0;
  }

  if (Uni->CylSpeed > MaxCyl)
    Uni->CylSpeed = MaxCyl;
  if (Uni->CylSpeed < -MaxCyl)
    Uni->CylSpeed = -MaxCyl;     
  if (Uni->CarSpeed > MaxCyl)
    Uni->CarSpeed = MaxCyl;
  if (Uni->CarSpeed < -MaxCyl)
    Uni->CarSpeed = -MaxCyl;     

  Uni->CylAngle += Uni->CylSpeed * Ani->DeltaTime;
  Uni->len += Uni->CarSpeed * Ani->DeltaTime;

  Uni->Heading = DT3_NormalizeAngle(Uni->Heading);
  Uni->len = DT3_NormalizeAngle(Uni->len);
  Uni->CylAngle = DT3_NormalizeAngle(Uni->CylAngle);   
} /* End of ' DT3_UnitCarTransformation' function */ 

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_G3DM *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_G3DM *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimsLoad(&Uni->Model, "bin/models/Lexus.g3dm");
  DT3_RndPrimCreateCyll(&Uni->Cylinder, 1000, 120, 1000, 50);
  Uni->SteerAngle = 0, Uni->len = 0, Uni->CylSpeed = 0, Uni->VerticalPos = 0; 
  Uni->CylAngle = 0, Uni->CarSpeed = 0, Uni->Heading = 0;
  Uni->Cylinder.MtlNo = 3;
  DT3_RndMaterials[Uni->Cylinder.MtlNo].ShdNo = DT3_RndShdAdd("earth");
  DT3_RndMaterials[Uni->Cylinder.MtlNo].Tex[0] = DT3_RndTexAddFromFile("bin/textures/race1.BMP");
} /* End of 'DT3_UnitInit' function */ 

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_G3DM *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT_G3DM *Uni, dt3ANIM *Ani )
{
  VEC center;
  INT Ind[4][5] = {{103, 104, 105, 106, 107},
  {108, 109, 110, 111, 112},
  {113, 114, 115, 116, 117},
  {118, 119, 120, 121, 122}}, i;
  INT IndW[] = {105, 110, 115, 120}, j;                      

  DT3_UnitCarRotation(Uni, Ani);
  for (j = 0; j < 4; j++)
  {
    center = VecDivNum(VecAddVec(Uni->Model.Prims[IndW[j]].MaxBB, 
      Uni->Model.Prims[IndW[j]].MinBB), 2);
    for (i = 0; i < 5; i++)       
      Uni->Model.Prims[Ind[j][i]].Trans =  MatrMulMatr3(MatrTranslate(VecNeg(center)),
      MatrRotateX(R2D(Uni->len)), MatrTranslate(center));           
  }
  for (j = 0; j < 2; j++)
  {
    center =  VecDivNum(VecAddVec(Uni->Model.Prims[IndW[j]].MaxBB, 
      Uni->Model.Prims[IndW[j]].MinBB), 2);
    for (i = 0; i < 5; i++)       
      Uni->Model.Prims[Ind[j][i]].Trans =  MatrMulMatr(Uni->Model.Prims[Ind[j][i]].Trans, 
        MatrMulMatr3(MatrTranslate(VecNeg(center)),
        MatrRotateZ(-Uni->SteerAngle), MatrTranslate(center)));    
  }
   Uni->VerticalPos += -fabs(Uni->CarSpeed) * sin(Uni->Heading) * Ani->DeltaTime;
   if (Uni->VerticalPos > 53)
    Uni->VerticalPos = 53;
   else if (Uni->VerticalPos < -53)
    Uni->VerticalPos = -53;      
 
  Uni->Model.Trans = MatrMulMatr(MatrScale(VecSet1(0.098)), MatrRotateX(-90));
  Uni->Cylinder.Trans = MatrMulMatr3(MatrRotateY(-R2D(Uni->CylAngle / 30)), 
    MatrTranslate(VecSet(0, -60 - Uni->VerticalPos, -1000)),
    MatrMulMatr(MatrRotateZ(-90 + R2D(Uni->Heading)), MatrRotateX(-90)));
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_G3DM *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_G3DM *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimsDraw(&Uni->Model, MatrIdentity());
  DT3_RndPrimDraw(&Uni->Cylinder, MatrIdentity());
/*  if (!Ani->IsPause && !Ani->Keys['K'] && !Ani->Keys['Y'])
    DT3_RndCamSet(VecSet(3.7166979, 6.7829499, -10.822050), VecSet(0, 0, 1), VecSet(0, 1, 0));
  else if (!Ani->IsPause && Ani->Keys['K'])
    DT3_RndCamSet(VecSet(-0.12176228, 6.7829499, -11.391928), VecSet(0, 0, 1), VecSet(0, 1, 0));   */
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_G3DM *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_G3DM *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimsFree(&Uni->Model);
  DT3_RndPrimFree(&Uni->Cylinder);
} /* End of 'DT3_UnitClose' function */ 

/* Unit model creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateModel( VOID )
{
  dt3UNIT_G3DM *Uni;

  if ((Uni = (dt3UNIT_G3DM *)DT3_AnimUnitCreate(sizeof(dt3UNIT_G3DM))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateModel' function */ 

/* END OF 'u_g3dm.c' FILE */