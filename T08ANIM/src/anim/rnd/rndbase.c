 /* FILE NAME: rndbase.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: Functional file in animation program.
 */

#include "def.h" 
#include "rnd.h"

VOID DT3_RndClose( VOID )
{
  DeleteDC(DT3_hRndDCFrame);
  DeleteObject(DT3_hRndBmFrame);
 }

VOID DT3_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = DT3_RndProjSize;
  /* Correct aspect ratio */
  if (DT3_RndFrameW > DT3_RndFrameH)
    rx *= (DBL)DT3_RndFrameW / DT3_RndFrameH;
  else
    ry *= (DBL)DT3_RndFrameH / DT3_RndFrameW;
  DT3_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      DT3_RndProjDist, DT3_RndProjFarClip);
  DT3_RndMatrVP = MatrMulMatr(DT3_RndMatrView, DT3_RndMatrProj);
}

VOID DT3_RndResize( INT W, INT H )
{
  HDC hDC = CreateCompatibleDC(GetDC(DT3_hRndWnd));

  if (DT3_hRndBmFrame != NULL)
    DeleteObject(DT3_hRndBmFrame);
  DT3_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(DT3_hRndWnd, hDC);		

  SelectObject(DT3_hRndDCFrame, DT3_hRndBmFrame);

  DT3_RndFrameW = W;
  DT3_RndFrameH = H;

  DT3_RndProjSet();
}

VOID DT3_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  DT3_RndMatrView = MatrView(Loc, At, Up);
  DT3_RndMatrVP = MatrMulMatr(DT3_RndMatrView, DT3_RndMatrProj);
}


VOID DT3_RndInit( HWND hWnd )
{     
  HDC hDC;      
  RECT rect;
  LONG W, H;
  GetWindowRect(hWnd, &rect);

  W = rect.right - rect.left;
  H = rect.bottom - rect.top;  
  DT3_hRndWnd = hWnd;
  hDC = GetDC(hWnd);
  DT3_hRndDCFrame = CreateCompatibleDC(hDC);
  DT3_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(hWnd, hDC);
  SelectObject(DT3_hRndDCFrame, DT3_hRndBmFrame);
  DT3_RndCamSet(VecSet1(5), VecSet1(0), VecSet(0, 1, 0));
}

VOID DT3_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DT3_RndFrameW, DT3_RndFrameH,
    DT3_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID DT3_RndStart( VOID )
{
  HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));  
  HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));  
  SelectObject(DT3_hRndDCFrame, hBrush);

  SelectObject(DT3_hRndDCFrame, hPen);
   
  Rectangle(DT3_hRndDCFrame, 0, 0, DT3_RndFrameW, DT3_RndFrameH);
  
  DeleteObject(hPen);
  DeleteObject(hBrush); 
} 
/* END OF 'rndbase.c' FILE */
