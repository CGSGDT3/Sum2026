#include "def.h"
#include "rnd/rnd.h"

#define dt3_MAX_UNITS 3000
typedef struct tagdt3UNIT dt3UNIT;
typedef struct tagdt3ANIM dt3ANIM;

struct tagdt3ANIM
{
  HWND hWnd;                      
  HDC hDC;                        
  INT W, H;                       

  dt3UNIT * Units[dt3_MAX_UNITS]; 
  INT NumOfUnits;                 

  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */
};

#define UNIT_BASE_FIELDS \
  VOID (*Init)( dt3UNIT *Uni, dt3ANIM *Ani );      \
  VOID (*Close)( dt3UNIT *Uni, dt3ANIM *Ani );     \
  VOID (*Response)( dt3UNIT *Uni, dt3ANIM *Ani );  \
  VOID (*Render)( dt3UNIT *Uni, dt3ANIM *Ani )

struct tagdt3UNIT
{
  UNIT_BASE_FIELDS;
};               

VOID  DT3_AnimInit( HWND hWnd );

VOID DT3_AnimClose( VOID );

VOID DT3_AnimResize( INT W, INT H );

VOID DT3_AnimCopyFrame( HDC hDC );

VOID DT3_AnimRender( VOID );

VOID DT3_AnimUnitAdd( dt3UNIT *Uni );

VOID dt3TimerInit( VOID );

VOID dt3TimerResponse( VOID );

extern dt3ANIM DT3_Anim;
