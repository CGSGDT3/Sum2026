/* FILE NAME: u_g3dm.c
 * PROGRAMMER: DT3
 * DATE: 18.06.2026
 * PURPOSE: unit model file in animation program.
 */ 

#include "units.h"

/* Structure of ball unit */
typedef struct tagdt3UNIT_G3DM
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  dt3PRIMS Model;     /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
} dt3UNIT_G3DM;

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
  DT3_RndPrimsLoad(&Uni->Model, "bin/models/x6.g3dm");
  Uni->Pos = VecSet1(0);
} /* End of 'DT3_UnitInit' function */ 

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
  DT3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrTranslate(Uni->Pos), MatrRotateX(-90)));
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
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateModel' function */ 

/* END OF 'u_g3dm.c' FILE */
