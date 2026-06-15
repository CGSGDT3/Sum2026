#include "rnd.h"

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndGridCreate( dt3GRID *G, INT W, INT H )
{
  return FALSE;
} /* End of 'DT3_RndGridCreate' function */

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 * RETURNS: None.
 */
VOID DT3_RndGridFree( dt3GRID *G )
{
} /* End of 'DT3_RndGridFree' function */

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       dt3PRIM *Pr;
 *   - grid data:
 *       dt3GRID *G;
 * RETURNS: None.
 */
VOID DT3_RndPrimFromGrid( dt3PRIM *Pr, dt3GRID *G )
{
} /* End of 'DT3_RndPrimFromGrid' function */

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 * RETURNS: None.
 */
VOID DT3_RndGridAutoNormals( dt3GRID *G )
{
} /* End of 'DT3_RndGridAutoNormals' function */

/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndGridCreateSphere( dt3GRID *G, FLT R, INT W, INT H )
{
  return FALSE;
} /* End of 'DT3_RndGridCreateSphere' function */

/* END OF 'rndgrid.c' FILE */