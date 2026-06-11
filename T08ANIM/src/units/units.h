#include "anim/anim.h"
#include "anim/rnd/rnd.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT *Uni, dt3ANIM *Ani );

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT *Uni, dt3ANIM *Ani );

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( dt3UNIT *Uni, dt3ANIM *Ani );

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT *Uni, dt3ANIM *Ani );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_AnimUnitCreate( INT Size );

dt3UNIT * DT3_UnitCreateBall( VOID );