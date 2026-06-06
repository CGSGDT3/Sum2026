  /* FILE NAME: globe.c
 * PROGRAMMER: DT3
 * DATE: 06.06.2026
 * PURPOSE: Source file in drawing globe program.
 */
#include <math.h>
#include <time.h>

#include "globe.h"

#define GLB_GRID_W 30
#define GLB_GRID_H 18
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

static VEC GLB_Geom[GLB_GRID_H][GLB_GRID_W], GLB_GeomN[GLB_GRID_H][GLB_GRID_W];
static INT GLB_Ws, GLB_Hs, Ws, Hs;
static DBL GLB_ProjSize = 1, GLB_Wp, GLB_Hp, ProjDist = 1;

COLORREF ColorTo255( VEC Color )
{
  INT
    R = Color.X * 255,
    G = Color.Y * 255,
    B = Color.Z * 255;
  R = MIN(255, MAX(0, R));
  G = MIN(255, MAX(0, G));
  B = MIN(255, MAX(0, B));
  return RGB(R, G, B);
}

VOID GLB_Init( DBL R )
{
  INT i, j;
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      DBL theta = i * PI / (GLB_GRID_H - 1), phi = j * 2 * PI / (GLB_GRID_W - 1);
 
      GLB_Geom[i][j].X = R * sin(theta) * sin(phi);
      GLB_Geom[i][j].Y = R * cos(theta);
      GLB_Geom[i][j].Z = R * sin(theta) * cos(phi);
      GLB_GeomN[i][j].X = sin(theta) * sin(phi);
      GLB_GeomN[i][j].Y = cos(theta);
      GLB_GeomN[i][j].Z = sin(theta) * cos(phi);
    }
}

VEC RotateX( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);

  NewP.X = P.X;
  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  return NewP;
}

VEC RotateY( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);

  NewP.X = P.Z * si + P.X * co;
  NewP.Y = P.Y;
  NewP.Z = P.Z * co - P.X * si;
  return NewP;
}

VEC RotateZ( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);

  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
}

VOID GLB_Draw( HDC hDC )
{
  INT i, j;
  HPEN hPen;
  HBRUSH hBrush;
  static POINT pnts[GLB_GRID_H][GLB_GRID_W], ps[4]; 
  DBL t = 0.3 * (DBL)clock() / CLOCKS_PER_SEC, len, nl;
  VEC L, N, P, M;
  L.X = L.Y = L.Z = 1;
  len = sqrt(L.X * L.X + L.Y * L.Y + L.Z * L.Z);
  L.X /= len, L.Y /= len, L.Z /= len;

  GLB_Init(0.5);

  hPen = CreatePen(PS_SOLID, 5, RGB(125, 125, 0));
  SelectObject(hDC, hPen);

  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      P = GLB_Geom[i][j];
      P = RotateZ(P, 30 * t);
      P = RotateY(P, 47 * t);
      P = RotateX(P, 18 * t); 
      P.X = P.X * GLB_Ws / GLB_Wp;
      P.Y = P.Y * GLB_Hs / GLB_Hp;
      P.Z -= 2;
      P.X *= ProjDist / (-P.Z);
      P.Y *= ProjDist / (-P.Z);
      pnts[i][j].x = GLB_Ws / 2 + P.X;
      pnts[i][j].y = GLB_Hs / 2 - P.Y;
    }

  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {     
      N = GLB_GeomN[i][j];
      N = RotateZ(N, 30 * t);
      N = RotateY(N, 47 * t);
      N = RotateX(N, 18 * t);  
      nl = N.X * L.X + N.Y * L.Y + N.Z * L.Z;
        if (nl < 0.18)
          nl = 0.18;
      M.X = 0.47 * nl;
      M.Y = 0.8 * nl;
      M.Z = 0.30 * nl;
      hBrush = CreateSolidBrush(ColorTo255(M));
      SelectObject(hDC, hBrush);
      ps[0] = pnts[i][j];
      ps[1] = pnts[i][j + 1];
      ps[2] = pnts[i + 1][j + 1];
      ps[3] = pnts[i + 1][j];
      if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) +
          (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) +
          (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
          (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) <= 0)
        Polygon(hDC, ps, 4);
      DeleteObject(hBrush);
    }

      /*
  for (i = 0; i < GLB_GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 1; j < GLB_GRID_W; j++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  for (j = 0; j < GLB_GRID_W; j++)
  {
    MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
     for (i = 1; i < GLB_GRID_H; i++)
       LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }    */
  DeleteObject(hPen);
}

VOID GLB_Resize( INT W, INT H )
{
  GLB_Ws = W;
  GLB_Hs = H;

  if (GLB_Ws >= GLB_Hs)
    GLB_Wp = GLB_ProjSize * GLB_Ws / GLB_Hs, GLB_Hp = GLB_ProjSize;
  else
    GLB_Wp = GLB_ProjSize, GLB_Hp = GLB_ProjSize * GLB_Hs / GLB_Ws;
}

