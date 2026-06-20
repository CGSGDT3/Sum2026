/* FILE NAME: anim.c
 * PROGRAMMER: DT3
 * DATE: 10.06.2026
 * PURPOSE: animation functional file in animation program.
 */

#include "anim.h"

dt3ANIM DT3_Anim; /* Global animation variable */

UINT64
  StartTime,    /* Start program time */
  OldTime,      /* Previous frame time */
  OldFPSTime,   /* Old time FPS measurement */
  PauseTime,    /* Time during pause period */
  TimePerSec,   /* Timer resolution */
  FrameCounter; /* Frames counter */

/* Animation initialization function.
 * ARGUMENTS:
 *   - handler of work window:
 *       HWND hWnd;
 * RETURNS: 
 *       None.
 */
VOID  DT3_AnimInit( HWND hWnd )
{      
  memset(&DT3_Anim, 0, sizeof(dt3ANIM)); 
  DT3_RndInit(hWnd);
  DT3_TimerInit();
  DT3_Anim.hWnd = DT3_hRndWnd;
  DT3_Anim.hDC = DT3_hRndDC;
  DT3_Anim.W = DT3_RndFrameW;
  DT3_Anim.H = DT3_RndFrameH;
  DT3_Anim.Mx = DT3_Anim.My = DT3_Anim.Mz = DT3_Anim.Mdx = DT3_Anim.Mdy = DT3_Anim.Mdz =
    (INT)DT3_Anim.JX = (INT)DT3_Anim.JY = (INT)DT3_Anim.JZ = (INT)DT3_Anim.JR = DT3_Anim.JPov = 0;
  
} /* End of 'DT3_AnimInit' function */ 

/* Animation deinitialization function.
 * ARGUMENTS:
 *     None.
 * RETURNS:
 *     None.
 */
VOID DT3_AnimClose( VOID )
{
  INT i;
  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Close(DT3_Anim.Units[i], &DT3_Anim), free(DT3_Anim.Units[i]);
  DT3_RndClose();
  memset(&DT3_Anim, 0, sizeof(dt3ANIM)); 
} /* End of 'DT3_AnimClose' function */ 

/* Animation work window resizing function.
 * ARGUMENTS:
 *   - Window width and height:
 *       INT W, H;
 * RETURNS:
 *       None.
 */
VOID DT3_AnimResize( INT W, INT H )
{
  DT3_RndResize(W, H); 
  DT3_Anim.W = DT3_RndFrameW;
  DT3_Anim.H = DT3_RndFrameH;  

  DT3_AnimRender();
} /* End of 'DT3_AnimResize' function */ 

/* Animation copying frame from work window to different device context function.
 * ARGUMENTS:
 *       None.
 * RETURNS: 
 *       None.
 */
VOID DT3_AnimCopyFrame( VOID )
{
  DT3_RndCopyFrame();
} /* End of 'DT3_AnimCopyFrame' function */ 

/* Animation rendering function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *    None.
 */
VOID DT3_AnimRender( VOID )
{
  INT i;

  DT3_TimerResponse();
  DT3_RndShdUpdate();

  if (DT3_Anim.IsActive)
    DT3_AnimInputResponse();

  DT3_RndStart();
  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Response(DT3_Anim.Units[i], &DT3_Anim);

  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Render(DT3_Anim.Units[i], &DT3_Anim);
  DT3_RndEnd();
} /* End of 'DT3_AnimRender' function */ 

/* Animation unit adding function.
 * ARGUMENTS:
 *   pointer to unit to be added:
 *    dt3UNIT *Uni;
 * RETURNS:
 *    None.
 */
VOID DT3_AnimUnitAdd( dt3UNIT *Uni )
{
  if (DT3_Anim.NumOfUnits < dt3_MAX_UNITS)
    DT3_Anim.Units[DT3_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &DT3_Anim);
} /* End of 'DT3_AnimUnitAdd' function */ 

/* END OF 'anim.c' FILE */