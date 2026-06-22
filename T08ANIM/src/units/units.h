/* FILE NAME: units.h
 * PROGRAMMER: DT3
 * DATE: 10.06.2026
 * PURPOSE: base unit functions template header file in animation program.
 */   

#ifndef __units_h_
#define __units_h_

#include "anim/anim.h"

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_AnimUnitCreate( INT Size );

/* Unit ball creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateBall( VOID );

/* Unit control creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateControl( VOID );

/* Unit cyl creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateCyl( VOID );

/* Unit thor creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateThor( VOID );

#endif /* __units_h_ */

/* END OF 'units.h' FILE */