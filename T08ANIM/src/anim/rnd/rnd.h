#ifndef __rnd_h_
#define __rnd_h_          
#include "def.h"

VOID DT3_RndInit( HWND hWnd );
VOID DT3_RndClose( VOID );
VOID DT3_RndResize( INT W, INT H );
VOID DT3_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID DT3_RndCopyFrame( HDC hDC );
VOID DT3_RndStart( VOID );

extern HWND DT3_hRndWnd;        /* Work window handle */
extern HDC DT3_hRndDCFrame;     /* Work window memory device context  */
extern HBITMAP DT3_hRndBmFrame; /* Work window background bitmap handle */
extern INT DT3_RndFrameW, DT3_RndFrameH; /* Work window size */

extern DBL
  DT3_RndProjSize,     /* Project plane fit square */
  DT3_RndProjDist,     /* Distance to project plane from viewer (near) */
  DT3_RndProjFarClip;  /* Distance to project far clip plane (far) */

extern MATR
  DT3_RndMatrView, /* View coordinate system matrix */
  DT3_RndMatrProj, /* Projection coordinate system matrix */
  DT3_RndMatrVP;   /* Stored (View * Proj) matrix */

typedef struct tagdt3VERTEX
{
  VEC P;  /* Vertex position */
} dt3VERTEX;

typedef struct tagdt3PRIM
{
  dt3VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */

  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */

  MATR Trans;   /* Additional transformation matrix */
} dt3PRIM;

BOOL dt3_RndPrimCreate( dt3PRIM *Pr, INT NoofV, INT NoofI );
VOID dt3_RndPrimFree( dt3PRIM *Pr );
VOID dt3_RndPrimDraw( dt3PRIM *Pr, MATR World );
BOOL dt3_RndPrimLoad( dt3PRIM *Pr, CHAR *FileName );

#endif /* __rnd_h */
/* END OF 'rnd.h' FILE */