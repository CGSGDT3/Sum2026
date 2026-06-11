/* FILE NAME: rnd.h
* PROGRAMMER: DT3
* DATE: 09.06.2026
* PURPOSE: render header file in animation program.
*/

#ifndef __rnd_h_
#define __rnd_h_ 

#include "def.h"

#define Gr_W 12
#define Gr_H 8

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

/* Work window init function.
 * ARGUMENTS:
 *   handler of window:
 *    HWND hWnd;
 * RETURNS:
 *   None.
 */
VOID DT3_RndInit( HWND hWnd );

/* Closing work window function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndClose( VOID );

/* Work window resizing function.
 * ARGUMENTS:
 *   New window width and height:
 *    INT W, H;
 * RETURNS:
 *   None.
 */
VOID DT3_RndResize( INT W, INT H );

/* Camera set function.
 * ARGUMENTS:
 *   Vectors of camera position:
 *    VEC Loc, At, Up;
 * RETURNS:
 *   None.
 */
VOID DT3_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Copying frame from work DC to differen DC function.
 * ARGUMENTS:
 *   Device context handle to copy:
 *    HDC hDC;
 * RETURNS:
 *   None.
 */
VOID DT3_RndCopyFrame( HDC hDC );

/* Start work on window (erasing background) function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndStart( VOID );

/* Creating primitive (memory allocation for vertices and indices) function.
 * ARGUMENTS:
 *   - pointer to primitive to be memory allocated:
 *       dt3PRIM *Pr;
 *   - number of vertices and indiced:
 *       INT NoofV, NoofI;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL dt3_RndPrimCreate( dt3PRIM *Pr, INT NoofV, INT NoofI );

/* Primitive free function.
 * ARGUMENTS:
 *   - pointer to primitive to be free:
 *       dt3PRIM *Pr;
 * RETURNS:
 *   None.
 */
VOID dt3_RndPrimFree( dt3PRIM *Pr );

/* Drawing primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to be draw:
 *       dt3PRIM *Pr;
 *   - world matrix:
 *       MATR World;
 * RETURNS:
 *   None.
 */
VOID dt3_RndPrimDraw( dt3PRIM *Pr, MATR World );

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       dt3PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL dt3_RndPrimLoad( dt3PRIM *Pr, CHAR *FileName );

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - sphere radius:
 *       DBL R;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL dt3_RndPrimCreateSphere( dt3PRIM *Pr, DBL R, INT W, INT H );

/* Create cyllinder primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - cyllinder radius:
 *       DBL R;
  *   - cyllinder height:
 *       DBL Z;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL dt3_RndPrimCreateCyll( dt3PRIM *Pr, DBL R, DBL Z, INT W, INT H );

/* Create thorus primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - inner radius:
 *       DBL Ri;
  *   - outer radius:
 *       DBL Ro;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL dt3_RndPrimCreateTh( dt3PRIM *Pr, DBL Ri, DBL Ro, INT W, INT H );

VOID DT3_RndEnd( VOID );

/* Flip window full screen mode function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */   
VOID FlipFullScreen( HWND hWnd );

#endif /* __rnd_h */
/* END OF 'rnd.h' FILE */