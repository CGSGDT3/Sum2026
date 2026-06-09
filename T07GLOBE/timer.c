#include <time.h>
#include <windows.h>

#include "timer.h"

static UINT64
  StartTime,    /* Start program time */
  OldTime,      /* Previous frame time */
  OldFPSTime,   /* Old time FPS measurement */
  PauseTime,    /* Time during pause period */
  TimePerSec,   /* Timer resolution */
  FrameCounter; /* Frames counter */

extern DOUBLE
  GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
  Time, DeltaTime,             /* Time with pause and interframe interval */
  FPS;                         /* Frames per second value */

VOID GLB_TimerInit( VOID )
{
  LARGE_INTEGER t;
  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldFPSTime = t.QuadPart;
  PauseTime = 0;
  FrameCounter = 0;
  GLB_IsPause = FALSE;
  Time = DeltaTime = 0;
  FPS = 30;
}

VOID GLB_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);

  /* Global time */
  GlobalTime = (DOUBLE)(t.QuadPart - StartTime) / TimePerSec;
  GlobalDeltaTime = (DOUBLE)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (!GLB_IsPause)
  {
    Time = (DOUBLE)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
    DeltaTime = GlobalDeltaTime;
  }
  else
  {
    DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }

  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldFPSTime > 3 * TimePerSec)
  {
    FPS = FrameCounter * TimePerSec / (DOUBLE)(t.QuadPart - OldFPSTime);
    OldFPSTime = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}