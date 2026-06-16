/* FILE NAME: rnd.h
* PROGRAMMER: DT3
* DATE: 09.06.2026
* PURPOSE: render header file in animation program.
*/

#ifndef __rnd_h_
#define __rnd_h_ 

#define GLEW_STATIC

#include <glew.h>       

#include "res/rndres.h"

#define Gr_W 12
#define Gr_H 8

extern HWND DT3_hRndWnd;        /* Work window handle */
extern HDC DT3_hRndDC;     /* Work window device context  */
extern INT DT3_RndFrameW, DT3_RndFrameH; /* Work window size */
extern HGLRC DT3_hRndGLRC;

extern DBL
  DT3_RndProjSize,     /* Project plane fit square */
  DT3_RndProjDist,     /* Distance to project plane from viewer (near) */
  DT3_RndProjFarClip;  /* Distance to project far clip plane (far) */
extern VEC
  DT3_RndCamAt, DT3_RndCamLoc, DT3_RndCamRight, DT3_RndCamUp; /* Camera position */

extern MATR
  DT3_RndMatrView, /* View coordinate system matrix */
  DT3_RndMatrProj, /* Projection coordinate system matrix */
  DT3_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Primitive type */
typedef enum tagdt3PRIM_TYPE
{
  DT3_RND_PRIM_POINTS,   /* Array of points  – GL_POINTS */
  DT3_RND_PRIM_LINES,    /* Line segments (by 2 points) – GL_LINES */
  DT3_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles – GL_TRIANGLES */
  DT3_RND_PRIM_TRISTRIP,
} dt3PRIM_TYPE;

typedef struct tagdt3VERTEX
{
  VEC P;  /* Vertex position */

  VEC2 T;  /* Texture coordinate */
  VEC N;   /* normal */
  VEC4 C; /* Vertex color */

} dt3VERTEX;

/* Grid topology representation type */
typedef struct tagdt3GRID
{
  INT W, H;      /* Grid size (in vertices) */
  dt3VERTEX *V;  /* Array (2D) of vertex */
} dt3GRID;

/* Primitive representation type */
typedef struct tagdt3PRIM
{
  dt3PRIM_TYPE Type; /* Primitive type */

  INT
    VA,              /* Vertex array Id */
    VBuf,            /* Vertex buffer Id */
    IBuf;            /* Index buffer Id (if 0 - use only vertex buffer) */

  INT NumOfElements; /* Number of indices/vecrtices */

  VEC MinBB, MaxBB;  /* Bound box */

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
 *   None
 * RETURNS:
 *   None.
 */
VOID DT3_RndCopyFrame( VOID );

/* Start work on window (erasing background) function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndStart( VOID );

/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - primitive type:
 *       dt3PRIM_TYPE Type;
 *   - vertex attributes array:
 *       dt3VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID DT3_RndPrimCreate( dt3PRIM *Pr, dt3PRIM_TYPE Type,
                        dt3VERTEX *V, INT NoofV, INT *Ind, INT NoofI );

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be free:
 *       dt3PRIM *Pr;
 * RETURNS: None.
 */
VOID DT3_RndPrimFree( dt3PRIM *Pr );

/* Drawing primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to be draw:
 *       dt3PRIM *Pr;
 *   - world matrix:
 *       MATR World;
 * RETURNS:
 *   None.
 */
VOID DT3_RndPrimDraw( dt3PRIM *Pr, MATR World );

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
BOOL DT3_RndPrimCreateSphere( dt3PRIM *Pr, DBL R, INT W, INT H );

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
BOOL DT3_RndPrimCreateCyll( dt3PRIM *Pr, DBL R, DBL Z, INT W, INT H );

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
BOOL DT3_RndPrimCreateTh( dt3PRIM *Pr, DBL Ri, DBL Ro, INT W, INT H );

VOID DT3_RndEnd( VOID );

/* Flip window full screen mode function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */   
VOID FlipFullScreen( HWND hWnd );

/* Tri-mesh geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - vertex array:
 *       dt3VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *Ind;
 *   - index array size:
 *       INT NumOfI;
 */
VOID DT3_RndPrimTriMeshAutoNormals( dt3VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI );

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       dt3PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL DT3_RndPrimLoad( dt3PRIM *Pr, CHAR *FileName );


/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *       UINT Source;
 *   - error type:
 *       UINT Type;
 *   - error message id:
 *       UINT Id, 
 *   - message severity:
 *       UINT severity, 
 *   - message text length:
 *       INT Length, 
 *   - message text:
 *       CHAR *Message, 
 *   - user addon parameters pointer:
 *       VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message,
                             const VOID *UserParam );

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndGridCreate( dt3GRID *G, INT W, INT H );    

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 * RETURNS: None.
 */
VOID DT3_RndGridFree( dt3GRID *G );   

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       dt3PRIM *Pr;
 *   - grid data:
 *       dt3GRID *G;
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndPrimFromGrid( dt3PRIM *Pr, dt3GRID *G );     

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 * RETURNS: None.
 */
VOID DT3_RndGridAutoNormals( dt3GRID *G );

/* Create sphere grid function.
 * ARGUMENTS:
 *   - grid data:
 *       dt3GRID *G;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndGridCreateSphere( dt3GRID *G, FLT R, INT W, INT H );  


#endif /* __rnd_h */
/* END OF 'rnd.h' FILE */