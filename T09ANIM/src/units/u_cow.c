/* FILE NAME: u_cow.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit COW file in animation program.
 */ 

#include "units.h"

/* Structure of ball unit */
typedef struct tagdt3UNIT_COW
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  dt3PRIM COW;     /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
} dt3UNIT_COW;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_COW *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_COW *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimLoad(&Uni->COW, "bin/models/cow.obj");

  Uni->Pos = VecSet1(10 * rand0());
} /* End of 'DT3_UnitInit' function */ 

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_COW *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT_COW *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_COW *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_COW *Uni, dt3ANIM *Ani )
{
  INT i, i1, i2, S = 15;

  for (i = 0; i < S; i++)
    for (i1 = 0; i1 < S; i1++)
      for (i2 = 0; i2 < S; i2++)
        DT3_RndPrimDraw(&Uni->COW, MatrMulMatr3(MatrScale(VecSet1(0.1)), MatrTranslate(VecAddVec(Uni->Pos, VecSet(i * 2.5, i1 * 2.5, i2 * 2.5))),
    MatrRotateZ(30 * sin(Ani->Time))));
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_COW *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_COW *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimFree(&Uni->COW);
} /* End of 'DT3_UnitClose' function */ 

/* Unit ball creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateCow( VOID )
{
  dt3UNIT_COW *Uni;

  if ((Uni = (dt3UNIT_COW *)DT3_AnimUnitCreate(sizeof(dt3UNIT_COW))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateBall' function */ 

/* END OF 'u_cow.c' FILE */
