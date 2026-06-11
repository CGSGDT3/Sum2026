/* FILE NAME: anim.c
 * PROGRAMMER: DT3
 * DATE: 10.06.2026
 * PURPOSE: animation header file in animation program.
 */

#ifndef __anim_h_
#define __anim_h_

#include "def.h"
#include "rnd/rnd.h"

/* const of MAX size of UNIT array */
#define dt3_MAX_UNITS 3000

/* Forward declaration */
typedef struct tagdt3UNIT dt3UNIT;

typedef struct tagdt3ANIM
{
  HWND hWnd;                      
  HDC hDC;                        
  INT W, H, Mx, My, Mz, Mdx, Mdy, Mdz, NumOfUnits, JPov;                       

  dt3UNIT * Units[dt3_MAX_UNITS]; 

  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS,                         /* Frames per second value */
    JX, JY, JZ, JR;
  BOOL
    IsPause;                     /* Pause flag */
  BYTE Keys[256], KeysClick[256], KeysOld[256], JBut[256], JButOld[32], JButClick[32];
} dt3ANIM; 

/* Unit base functions declaration (definition) */
#define UNIT_BASE_FIELDS \
  VOID (*Init)( dt3UNIT *Uni, dt3ANIM *Ani );      \
  VOID (*Close)( dt3UNIT *Uni, dt3ANIM *Ani );     \
  VOID (*Response)( dt3UNIT *Uni, dt3ANIM *Ani );  \
  VOID (*Render)( dt3UNIT *Uni, dt3ANIM *Ani )

struct tagdt3UNIT
{
  UNIT_BASE_FIELDS;
};               

/* Animation initialization function.
 * ARGUMENTS:
 *   - handler of work window:
 *       HWND hWnd;
 * RETURNS: 
 *       None.
 */
VOID  DT3_AnimInit( HWND hWnd );

/* Animation deinitialization function.
 * ARGUMENTS:
 *     None.
 * RETURNS:
 *     None.
 */
VOID DT3_AnimClose( VOID );

/* Animation work window resizing function.
 * ARGUMENTS:
 *   - Window width and height:
 *       INT W, H;
 * RETURNS:
 *       None.
 */
VOID DT3_AnimResize( INT W, INT H );

/* Animation copying frame from work window to different device context function.
 * ARGUMENTS:
 *   - Handler of device context:
 *       HDC hDC;
 * RETURNS: 
 *       None.
 */
VOID DT3_AnimCopyFrame( HDC hDC );

/* Animation rendering function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *    None.
 */
VOID DT3_AnimRender( VOID );

/* Animation unit adding function.
 * ARGUMENTS:
 *   pointer to unit to be added:
 *    dt3UNIT *Uni;
 * RETURNS:
 *    None.
 */
VOID DT3_AnimUnitAdd( dt3UNIT *Uni );

/* Timer initialising function.
 * ARGUMENTS:
 *    None.
 * RETURNS:
 *    None.
 */                 
VOID DT3_TimerInit( VOID );

/* Timer responsing function.
 * ARGUMENTS:
 *    None.
 * RETURNS:
 *    None.
 */  
VOID DT3_TimerResponse( VOID ); 

extern dt3ANIM DT3_Anim;

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
