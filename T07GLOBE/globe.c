/* FILE NAME: globe.c
 * PROGRAMMER: DT3
 * DATE: 06.06.2026
 * PURPOSE: Source file in drawing globe program.
 */
#include <math.h>
#include <time.h>

#include "globe.h"
#include "timer.h"

#define GLB_GRID_W 30
#define GLB_GRID_H 18
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

/* statics arrays of geometry */
static VEC GLB_Geom[GLB_GRID_H][GLB_GRID_W], GLB_GeomN[GLB_GRID_H][GLB_GRID_W];

/* constants of screen size */
static INT GLB_Ws, GLB_Hs, Ws, Hs;

/* constants to projection */
static DBL GLB_ProjSize = 1, GLB_Wp, GLB_Hp, ProjDist = 1;

/* Vector coordinates to RGB function.
 * ARGUMENTS:
 *     vector:
 *       VEC Color;
 * RETURNS:
 *   (COLORREF) result color.
 */
COLORREF ColorTo255( VEC Color )
{
  INT
    R = (INT)(Color.X * 255),
    G = (INT)(Color.Y * 255),
    B = (INT)(Color.Z * 255);
  R = MIN(255, MAX(0, R));
  G = MIN(255, MAX(0, G));
  B = MIN(255, MAX(0, B));
  return RGB(R, G, B);
} /* End of 'ColorTo255' function */

DBL Power( DBL A, DBL B )
{
  if (A >= 0)
    return pow(A, B);
  else
    return -pow(-A, B);
}
/* Initialising of globe function.
 * ARGUMENTS:
 *     radius of globe:
 *       DBL R;
 * RETURNS:
 *   VOID.
 */
VOID GLB_Init( DBL R )
{
  INT i, j;
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      DBL theta = i * PI / (GLB_GRID_H - 1), phi = j * 2 * PI / (GLB_GRID_W - 1);  
      GLB_Geom[i][j].X = R * Power(sin(theta) * sin(phi), 0.47);
      GLB_Geom[i][j].Y = 2 * R * Power(cos(theta), 3);
      GLB_Geom[i][j].Z = R * Power(sin(theta) * cos(phi), 0.30);
    }

    for (i = 0; i < GLB_GRID_H - 1; i++)
      for (j = 0; j < GLB_GRID_W - 1; j++)
      {
        VEC N1 = VecNormalize(VecCrossVec(VecSubVec(GLB_Geom[i][j + 1], GLB_Geom[i][j]),
                                          VecSubVec(GLB_Geom[i + 1][j], GLB_Geom[i][j])));
        VEC N2 =
          VecNormalize(VecCrossVec(VecSubVec(GLB_Geom[i + 1][j], GLB_Geom[i + 1][j + 1]),
                                   VecSubVec(GLB_Geom[i][j + 1], GLB_Geom[i + 1][j + 1])));
        GLB_GeomN[i][j] = VecNeg(VecNormalize(VecAddVec(N1, N2)));
      }
} /* End of 'GLB_Init' function */

/* Drawing of globe function.
 * ARGUMENTS:
 *     deskriptor of DC:
 *       HDC hDC;
 * RETURNS:
 *   VOID.
 */
VOID GLB_Draw( HDC hDC )
{
  INT i, j;
  RECT rc;
  HPEN hPen;
  HBRUSH hBrush;
  static POINT pnts[GLB_GRID_H][GLB_GRID_W], ps[4]; 
  DBL t = 0.3 * Time, nl;
  CHAR Buf[100];
  VEC L = VecNormalize(VecSet(3, 1, 1)),
    L1 = VecNormalize(VecSet(-2, sin(t * 18), 1)), N, P, M;
  MATR m = MatrMulMatr(MatrMulMatr(MatrRotateZ(300 * t / 2), 
        MatrRotateY(470 * t / 2)), MatrRotateX(180 * t));

  rc.left = 18;
  rc.top = 18;
  rc.right = GLB_Ws - 18;
  rc.bottom = GLB_Hs - 18;
  DrawText(hDC, Buf, sprintf(Buf, "FPS: %f", FPS), &rc, DT_CENTER);   
  VecNormalize(L);


  GLB_Init(0.5);

  hPen = CreatePen(PS_NULL, 5, RGB(125, 125, 0));
  SelectObject(hDC, hPen);

  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      P = PointTransform(GLB_Geom[i][j], MatrMulMatr(m, MatrTranslate(VecSet(sin(t), - cos(2 * t), -3 - 7 * fabs(cos(t))))));
      P.X = P.X * GLB_Ws / GLB_Wp;
      P.Y = P.Y * GLB_Hs / GLB_Hp;
      P.X *= ProjDist / (-P.Z);
      P.Y *= ProjDist / (-P.Z);
      pnts[i][j].x = (LONG)(GLB_Ws / 2 + P.X);
      pnts[i][j].y = (LONG)(GLB_Hs / 2 - P.Y);
    }

  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {     
      m = MatrTranspose(MatrInverse(m));
      N = VectorTransform(GLB_GeomN[i][j], m);
      nl = VecDotVec(N, L);
        if (nl < 0.18)
          nl = 0.18;   
      M = VecMulNum(VecSet(1, 0.8, 0.30), nl);

      nl = VecDotVec(N, L1);
        if (nl < 0.18)
          nl = 0.18;
      M = VecAddVec(M, VecMulNum(VecSet(0.30, 0.8, 0.47), nl));
      hBrush = CreateSolidBrush(ColorTo255(M));
      SelectObject(hDC, hBrush);
      ps[0] = pnts[i][j];
      ps[1] = pnts[i][j + 1];
      ps[2] = pnts[i + 1][j + 1];
      ps[3] = pnts[i + 1][j];
      if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) +
          (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) +
          (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
          (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) >= 0)
        Polygon(hDC, ps, 4);
      DeleteObject(hBrush);
    }

  DeleteObject(hPen);
} /* End of 'GLB_Draw' function */

/* Resizing of globe function.
 * ARGUMENTS:
 *     screen width and height:
 *       INT W, H;
 * RETURNS:
 *   VOID.
 */
VOID GLB_Resize( INT W, INT H )
{
  GLB_Ws = W;
  GLB_Hs = H;

  if (GLB_Ws >= GLB_Hs)
    GLB_Wp = GLB_ProjSize * GLB_Ws / GLB_Hs, GLB_Hp = GLB_ProjSize;
  else
    GLB_Wp = GLB_ProjSize, GLB_Hp = GLB_ProjSize * GLB_Hs / GLB_Ws;
} /* End of 'GLB_Resize' function */      
/* End of 'globe.c' file */
