/* FILE NAME: u_cyl.c
 * PROGRAMMER: DT3
 * DATE: 19.06.2026
 * PURPOSE: unit CYL file in animation program.
 */ 

#include "units.h"

/* Structure of cyl unit */
typedef struct tagdt3UNIT_CYL
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  dt3PRIM Cyl;      /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
  DBL Size, Speed;  /* Size of cyl and his speed of jumping */
} dt3UNIT_CYL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CYL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_CYL *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(8 * rand1(), 1, 8 * rand1());
  Uni->Size = 1 + rand1() * 0.5;
  if (Uni->Size < 0.08)
    Uni->Size = 0.08;
  Uni->Speed = 30 * (DBL)rand() / RAND_MAX;
  DT3_RndPrimCreateCyll(&Uni->Cyl, Uni->Size, Uni->Size * 10, Gr_W, Gr_H);
} /* End of 'DT3_UnitInit' function */ 

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CYL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT_CYL *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(Uni->Pos.X, sin(Uni->Speed * Ani->Time), Uni->Pos.Z);
} /* End of 'DT3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CYL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_CYL *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimDraw(&Uni->Cyl, MatrMulMatr(MatrTranslate(Uni->Pos),
    MatrRotateY(90 * (Ani->JX + Ani->JR))));
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_CYL *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_CYL *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimFree(&Uni->Cyl);
} /* End of 'DT3_UnitClose' function */ 

/* Unit cyl creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateCyl( VOID )
{
  dt3UNIT_CYL *Uni;

  if ((Uni = (dt3UNIT_CYL *)DT3_AnimUnitCreate(sizeof(dt3UNIT_CYL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateCyl' function */ 

/* END OF 'u_cyl.c' FILE */
