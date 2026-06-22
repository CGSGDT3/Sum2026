/* FILE NAME: u_thor.c
 * PROGRAMMER: DT3
 * DATE: 19.06.2026
 * PURPOSE: unit TH file in animation program.
 */ 

#include "units.h"

/* Structure of thor unit */
typedef struct tagdt3UNIT_TH
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  dt3PRIM Thor;     /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
  DBL Size, Speed;  /* Size of thor and his speed of jumping */
} dt3UNIT_TH;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_TH *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_TH *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(8 * rand1(), 1, 8 * rand1());
  Uni->Size = 1 + rand1() * 0.5;
  if (Uni->Size < 0.08)
    Uni->Size = 0.08;
  Uni->Speed = 30 * (DBL)rand() / RAND_MAX;
  DT3_RndPrimCreateTh(&Uni->Thor, Uni->Size, Uni->Size * 10, 30, 47);
} /* End of 'DT3_UnitInit' function */ 

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_TH *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT_TH *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(Uni->Pos.X, sin(Uni->Speed * Ani->Time), Uni->Pos.Z);
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_TH *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_TH *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimDraw(&Uni->Thor, MatrMulMatr3(MatrTranslate(Uni->Pos),
    MatrRotateX(90),
    MatrRotateY(90 * (Ani->JX + Ani->JR))));
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_TH *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_TH *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimFree(&Uni->Thor);
} /* End of 'DT3_UnitClose' function */ 

/* Unit thor creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateThor( VOID )
{
  dt3UNIT_TH *Uni;

  if ((Uni = (dt3UNIT_TH *)DT3_AnimUnitCreate(sizeof(dt3UNIT_TH))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateThor' function */ 

/* END OF 'u_thor.c' FILE */
