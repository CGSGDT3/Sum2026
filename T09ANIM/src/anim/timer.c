/* FILE NAME: timer.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: timer functional file in animation program.
 */ 

#include "timer.h"
#include "anim.h"

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

/* END OF 'timer.c' FILE */