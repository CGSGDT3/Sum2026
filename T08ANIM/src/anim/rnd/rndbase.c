 /* FILE NAME: rndbase.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: Functional file in animation program.
 */

#include "def.h" 
#include "rnd.h"

/* Closing work window function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndClose( VOID )
{
  DeleteDC(DT3_hRndDCFrame);
  DeleteObject(DT3_hRndBmFrame);
} /* End of 'DT3_RndClose' function */ 

/* Work window projection set function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
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
} /* End of 'DT3_RndProjSet' function */ 

/* Work window resizing function.
 * ARGUMENTS:
 *   New window width and height:
 *    INT W, H;
 * RETURNS:
 *   None.
 */
VOID DT3_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(DT3_hRndWnd);
/*  Not working fragment. Coloring isnt' working. But why?
if (DT3_hRndDCFrame != NULL)
    DeleteObject(DT3_hRndDCFrame);
  DT3_hRndDCFrame = CreateCompatibleDC(hDC);*/

  if (DT3_hRndBmFrame != NULL)
    DeleteObject(DT3_hRndBmFrame);
  DT3_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(DT3_hRndWnd, hDC);		
  SelectObject(DT3_hRndDCFrame, DT3_hRndBmFrame);

  DT3_RndFrameW = W;
  DT3_RndFrameH = H;

  DT3_RndProjSet();
} /* End of 'DT3_RndResize' function */ 

/* Camera set function.
 * ARGUMENTS:
 *   Vectors of camera position:
 *    VEC Loc, At, Up;
 * RETURNS:
 *   None.
 */
VOID DT3_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  DT3_RndMatrView = MatrView(Loc, At, Up);
  DT3_RndMatrVP = MatrMulMatr(DT3_RndMatrView, DT3_RndMatrProj);
} /* End of 'DT3_RndCamSet' function */

/* Work window init function.
 * ARGUMENTS:
 *   handler of window:
 *    HWND hWnd;
 * RETURNS:
 *   None.
 */
VOID DT3_RndInit( HWND hWnd )
{     
  HDC hDC;      
  DT3_hRndWnd = hWnd;

  hDC = GetDC(DT3_hRndWnd);
  DT3_hRndDCFrame = CreateCompatibleDC(hDC);

  ReleaseDC(hWnd, hDC);
  DT3_RndResize(100, 100);
  DT3_RndCamSet(VecSet1(5), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'DT3_RndInit' function */

/* Copying frame from work DC to differen DC function.
 * ARGUMENTS:
 *   Device context handle to copy:
 *    HDC hDC;
 * RETURNS:
 *   None.
 */
VOID DT3_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DT3_RndFrameW, DT3_RndFrameH,
    DT3_hRndDCFrame, 0, 0, SRCCOPY);
} /* End of 'DT3_RndCopyFrame' function */

/* Start work on window (erasing background) function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndStart( VOID )
{
  HPEN hPen = CreatePen(PS_NULL, 1, RGB(255, 0, 0));  
  HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));  
  SelectObject(DT3_hRndDCFrame, hBrush);       
  SelectObject(DT3_hRndDCFrame, hPen);
   
  Rectangle(DT3_hRndDCFrame, 0, 0, DT3_RndFrameW, DT3_RndFrameH);
  
  DeleteObject(hPen);
  DeleteObject(hBrush); 
} /* End of 'DT3_RndStart' function */ 

/* END OF 'rndbase.c' FILE */
