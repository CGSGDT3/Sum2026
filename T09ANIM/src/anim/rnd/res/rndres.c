/* FILE NAME: rndres.c
 * PROGRAMMER: DT3
 * DATE: 13.06.2026
 * PURPOSE: resource functional file in animation program.
 */

#include "rndres.h"

/* Resource initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndResInit( VOID )
{
  DT3_RndShdInit();
  DT3_RndMtlInit();
  DT3_RndTexInit();
  DT3_RndFntInit();
} /* End of 'DT3_RndResInit' function */

/* Resource deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndResClose( VOID )
{
  DT3_RndFntClose();
  DT3_RndShdClose();
  DT3_RndMtlClose();
  DT3_RndTexClose();
} /* End of 'DT3_RndResClose' function */

/* END OF 'rndres.c' FILE */