#ifndef __TIMER_H_
#define __TIMER_H_

VOID GLB_TimerInit( VOID );
VOID GLB_TimerResponse( VOID );

DOUBLE
  GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
  Time, DeltaTime,             /* Time with pause and interframe interval */
  FPS;                         /* Frames per second value */

BOOL
  GLB_IsPause;                     /* Pause flag */

#endif __TIMER_H_