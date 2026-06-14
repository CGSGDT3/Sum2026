 /* FILE NAME: rndbase.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: Functional file in animation program.
 */    

#include "rnd.h"

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

/* Closing work window function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndClose( VOID )
{
  DT3_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DT3_hRndGLRC);
  ReleaseDC(DT3_hRndWnd, DT3_hRndDC);
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
/*  Not working fragment. Coloring isnt' working. But why?
if (DT3_hRndDCFrame != NULL)
    DeleteObject(DT3_hRndDCFrame);
  DT3_hRndDCFrame = CreateCompatibleDC(hDC);*/
  glViewport(0, 0, W, H);

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
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  DT3_hRndWnd = hWnd;

  /* Prepare frame compatible device contesxt */
  DT3_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DT3_hRndDC, &pfd);

  DescribePixelFormat(DT3_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(DT3_hRndDC, i, &pfd);

  /* OpenGL init: rendering context setup */
  DT3_hRndGLRC = wglCreateContext(DT3_hRndDC);
  wglMakeCurrent(DT3_hRndDC, DT3_hRndGLRC);

  if (glewInit() != GLEW_OK)
    exit(0);

  #ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);

    OutputDebugString(glGetString(GL_VERSION));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_VENDOR));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_RENDERER));
    OutputDebugString("\n");
  #endif /* NDEBUG */

 

  /* Render parameters setup */
  glEnable(GL_DEPTH_TEST);

  DT3_RndProjSize = 0.1;
  DT3_RndProjDist = DT3_RndProjSize;
  DT3_RndProjFarClip = 300;
  DT3_RndFrameW = 470;
  DT3_RndFrameH = 470;
  DT3_RndCamSet(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));

  DT3_RndResInit();
} /* End of 'DT3_RndInit' function */

/* Copying frame from work DC to differen DC function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndCopyFrame( VOID )
{
  SwapBuffers(DT3_hRndDC);
} /* End of 'DT3_RndCopyFrame' function */

/* Start work on window (erasing background) function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   None.
 */
VOID DT3_RndStart( VOID )
{
  VEC4 ClearColor = {0.30, 0.47, 0.8, 1};
  FLT DepthClearValue = 1;

  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, &ClearColor.X);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
} /* End of 'DT3_RndStart' function */ 

VOID DT3_RndEnd( VOID )
{
  glFinish();
}

/* Flip window full screen mode function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */   
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRect);

    /* Obtain nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);

    GetMonitorInfo(hmon, &mi);

    /* Go to full screen mode */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);


    /* Expand window */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
    /* Restore from full screen mode */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* END OF 'rndbase.c' FILE */
