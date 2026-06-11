#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT *Uni, dt3ANIM *Ani )
{
} /* End of 'DT3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_AnimUnitCreate( INT Size )
{
  dt3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(dt3UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = DT3_UnitInit;
  Uni->Close = DT3_UnitClose;
  Uni->Response = DT3_UnitResponse;
  Uni->Render = DT3_UnitRender;

  return Uni;
} /* End of 'DT3_AnimUnitCreate' function */


