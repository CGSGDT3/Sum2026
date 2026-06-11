/* FILE NAME: u_ball.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit BALL file in animation program.
 */ 

#include "units.h"

typedef struct tagdt3UNIT_BALL
{
  UNIT_BASE_FIELDS;
  dt3PRIM Ball;
  VEC Pos;
  DBL Size, Speed;
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
  Uni->Pos = VecSet(3 * rand1(), 3 * rand1(), rand1());
  Uni->Size = (DBL)rand() / RAND_MAX;
  if (Uni->Size < 0.08)
    Uni->Size = 0.08;
  Uni->Speed = 5 * (DBL)rand() / RAND_MAX;
  dt3_RndPrimCreateSphere(&Uni->Ball, Uni->Size, Gr_W, Gr_H);
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
  Uni->Pos = VecSet(Uni->Pos.X, sin(Uni->Speed * 3 * Ani->Time), Uni->Pos.Z);
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
  dt3_RndPrimDraw(&Uni->Ball, MatrTranslate(Uni->Pos));
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_BALL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT *Uni, dt3ANIM *Ani )
{
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
