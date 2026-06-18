 /* FILE NAME: rnddata.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: Data file in animation program.
 */  

#include "def.h"

HWND DT3_hRndWnd;        /* Work window handle */
HDC DT3_hRndDC;     /* Work window memory device context  */
HGLRC DT3_hRndGLRC;
INT DT3_RndFrameW, DT3_RndFrameH; /* Work window size */

DBL
  DT3_RndProjSize = 0.1,     /* Project plane fit square */
  DT3_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  DT3_RndProjFarClip = 30000;  /* Distance to project far clip plane (far) */
VEC
  DT3_RndCamAt, DT3_RndCamLoc, DT3_RndCamRight, DT3_RndCamUp; /* Camera position */

MATR
  DT3_RndMatrView, /* View coordinate system matrix */
  DT3_RndMatrProj, /* Projection coordinate system matrix */
  DT3_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Additional shader parameters */

INT DT3_RndShdAddonI[8];
VEC DT3_RndShdAddonV[8];
FLOAT DT3_RndShdAddonF[8];

/* END OF 'rnddata.c' FILE */