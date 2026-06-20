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
  dt3PRIM Cyll;                     /* Cyllinder of Earth */
  FLT phi, psi, len, Vcyll, Zcoord; /* Phi - rotation of wheels, psi - rotation of all car, 
                                     * len - sum rotation of wheels by drive, Vcyll - speed of earth cyllindre.
                                     * Angles in degrees */
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
  FLT phi_max = 59, Aphi = 1, Acyll = 0.1, MaxCyll = 18; /* Acceleration of cyllindre by time,
                                                          * constants of max angle of rotation, 
                                                          * angle acceeration of wheels */
  if (Ani->Keys['D'])
    Uni->phi += Aphi;
  if (Ani->Keys['A'])
    Uni->phi -= Aphi;      
  if (Uni->phi > phi_max)
    Uni->phi = phi_max;
  if (Uni->phi < -phi_max)
    Uni->phi = -phi_max; 
  if (Ani->Keys['W'])     
    Uni->Vcyll += Acyll;
  if (Ani->Keys['S'])
    Uni->Vcyll -= Acyll;

  if (!Ani->Keys['W'] && !Ani->Keys['S'])
  {
    if (Uni->Vcyll > 0)
    {
      Uni->Vcyll -= 1;
      if (Uni->Vcyll < 0)
        Uni->Vcyll = 0;
    }
    else if (Uni->Vcyll < 0)
    {
      Uni->Vcyll += 1;
      if (Uni->Vcyll > 0)
        Uni->Vcyll = 0;
    }
  }
  if (Uni->Vcyll > MaxCyll)
    Uni->Vcyll = MaxCyll;
  if (Uni->Vcyll < -MaxCyll)
    Uni->Vcyll = -MaxCyll;     
  Uni->len += Uni->Vcyll * Ani->DeltaTime;
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
  DT3_RndPrimCreateCyll(&Uni->Cyll, 1000, 1000, 1000, 1000);
  Uni->phi = 0, Uni->psi = 0, Uni->len = 0, Uni->Vcyll = 0, Uni->Zcoord = 0; 
  Uni->Cyll.MtlNo = 3;
  DT3_RndMaterials[Uni->Cyll.MtlNo].Tex[0] = DT3_RndTexAddFromFile("bin/textures/rock.BMP");
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
    center =  VecDivNum(VecAddVec(Uni->Model.Prims[IndW[j]].MaxBB, Uni->Model.Prims[IndW[j]].MinBB), 2);
    for (i = 0; i < 5; i++)       
      Uni->Model.Prims[Ind[j][i]].Trans =  MatrMulMatr3(MatrTranslate(VecNeg(center)),
      MatrRotateX(R2D(Uni->len)), MatrTranslate(center));           
  }
  for (j = 0; j < 2; j++)
  {
    center =  VecDivNum(VecAddVec(Uni->Model.Prims[IndW[j]].MaxBB, Uni->Model.Prims[IndW[j]].MinBB), 2);
    for (i = 0; i < 5; i++)       
      Uni->Model.Prims[Ind[j][i]].Trans =  MatrMulMatr(Uni->Model.Prims[Ind[j][i]].Trans, 
        MatrMulMatr3(MatrTranslate(VecNeg(center)),
        MatrRotateZ(-Uni->phi), MatrTranslate(center)));    
    Uni->Zcoord += -abs(Uni->Vcyll) * sin(Uni->phi) * Ani->DeltaTime / 18;
    if (Uni->Zcoord > 47)
      Uni->Zcoord = 47;
    else if (Uni->Zcoord < -47)
      Uni->Zcoord = -47;      
  } 
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_G3DM *Uni;
 *   - animation context:
 *       dt3ANIM *An  i;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_G3DM *Uni, dt3ANIM *Ani )
{
  MATR m_rot = MatrMulMatr(MatrRotateZ(-90), MatrRotateX(-90));

  DT3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrScale(VecSet1(0.098)),/* MatrTranslate(VecSet(Uni->Zcoord, 0, 0)),*/ MatrRotateX(-90)));
  DT3_RndPrimDraw(&Uni->Cyll, MatrMulMatr3(MatrRotateY(-R2D(Uni->len / 30)), 
    MatrTranslate(VecSet(0, -500 - Uni->Zcoord * 10, -1000)), m_rot));
  if (!Ani->IsPause && !Ani->Keys['K'] && !Ani->Keys['Y'])
    DT3_RndCamSet(VecSet(-0.12176228, 6.7829499, -11.391928), VecSet(0, 0, 1), VecSet(0, 1, 0));
  else if (!Ani->IsPause && Ani->Keys['K'])
    DT3_RndCamSet(VecSet(3.7166979, 6.7829499, -10.822050), VecSet(0, 0, 1), VecSet(0, 1, 0));
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
  DT3_RndPrimFree(&Uni->Cyll);
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
