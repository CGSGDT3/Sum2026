/* FILE NAME: anim.c
 * PROGRAMMER: DT3
 * DATE: 10.06.2026
 * PURPOSE: animation functional file in animation program.
 */

#include "anim.h"
#include "rnd/rnd.h"
dt3ANIM DT3_Anim; /* Global animation variable */

static UINT64
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
  DT3_Anim.hWnd = DT3_hRndWnd;
  DT3_Anim.hDC = DT3_hRndDCFrame;
  DT3_Anim.W = DT3_RndFrameW;
  DT3_Anim.H = DT3_RndFrameH;
  DT3_Anim.Mx = DT3_Anim.My = DT3_Anim.Mz = DT3_Anim.Mdx = DT3_Anim.Mdy = DT3_Anim.Mdz =
    DT3_Anim.JX = DT3_Anim.JY = DT3_Anim.JZ = DT3_Anim.JR = DT3_Anim.JPov = 0;
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
 *   - Handler of device context:
 *       HDC hDC;
 * RETURNS: 
 *       None.
 */
VOID DT3_AnimCopyFrame( HDC hDC )
{
  DT3_RndCopyFrame(hDC);
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
  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Response(DT3_Anim.Units[i], &DT3_Anim);
  DT3_RndStart();
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

/* Timer initialising function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *    None.
 */
VOID DT3_TimerInit( VOID )
{
  LARGE_INTEGER t;
  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldFPSTime = t.QuadPart;
  PauseTime = 0;
  FrameCounter = 0;
  DT3_Anim.IsPause = FALSE;
  DT3_Anim.Time = DT3_Anim.DeltaTime = 0;
  DT3_Anim.FPS = 30;
} /* End of 'DT3_TimerInit' function */ 

/* Timer responsing function.
 * ARGUMENTS:
 *    None.
 * RETURNS:
 *    None.
 */
VOID DT3_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);

  /* Global time */
  DT3_Anim.GlobalTime = (DOUBLE)(t.QuadPart - StartTime) / TimePerSec;
  DT3_Anim. GlobalDeltaTime = (DOUBLE)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (!DT3_Anim.IsPause)
  {
    DT3_Anim.Time = (DOUBLE)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
    DT3_Anim.DeltaTime = DT3_Anim.GlobalDeltaTime;
  }
  else
  {
    DT3_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }

  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldFPSTime > 3 * TimePerSec)
  {
    DT3_Anim.FPS = FrameCounter * TimePerSec / (DOUBLE)(t.QuadPart - OldFPSTime);
    OldFPSTime = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'DT3_TimerResponse' function */ 

/* END OF 'anim.c' FILE */