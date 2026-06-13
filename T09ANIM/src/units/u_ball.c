/* FILE NAME: u_ball.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit BALL file in animation program.
 */ 

#include "units.h"

/* Structure of ball unit */
typedef struct tagdt3UNIT_BALL
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  dt3PRIM Ball;     /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
  DBL Size, Speed;  /* Size of ball and his speed of jumping */
} dt3UNIT_BALL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_BALL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(8 * rand1(), 1, 8 * rand1());
  Uni->Size = 1 + rand1() * 0.5;
  if (Uni->Size < 0.08)
    Uni->Size = 0.08;
  Uni->Speed = 30 * (DBL)rand() / RAND_MAX;
  DT3_RndPrimCreateSphere(&Uni->Ball, Uni->Size, Gr_W, Gr_H);
} /* End of 'DT3_UnitInit' function */ 

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_BALL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(Uni->Pos.X, sin(Uni->Speed * Ani->Time), Uni->Pos.Z);
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_BALL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimLoad(&Uni->Ball, MatrMulMatr(MatrTranslate(Uni->Pos),
    MatrRotateY(90 * (Ani->JX + Ani->JR))));
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_BALL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimFree(&Uni->Ball);
} /* End of 'DT3_UnitClose' function */ 

/* Unit ball creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateBall( VOID )
{
  dt3UNIT_BALL *Uni;

  if ((Uni = (dt3UNIT_BALL *)DT3_AnimUnitCreate(sizeof(dt3UNIT_BALL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateBall' function */ 

/* END OF 'u_ball.c' FILE */
