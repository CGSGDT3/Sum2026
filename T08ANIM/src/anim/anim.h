#include "def.h"
#define dt3_MAX_UNITS 47
typedef struct tagdt3UNIT dt3UNIT;
typedef struct tagdt3ANIM dt3ANIM;
typedef struct tagdt3ANIM
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

struct tagdt3UNIT
{
  VOID (*Init)( dt3UNIT *Uni, dt3ANIM *Ani );
  VOID (*Close)( dt3UNIT *Uni, dt3ANIM *Ani );
  VOID (*Response)( dt3UNIT *Uni, dt3ANIM *Ani );
  VOID (*Render)( dt3UNIT *Uni, dt3ANIM *Ani );
};