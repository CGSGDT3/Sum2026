 /* FILE NAME: main.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: Main file in animation program.
 */  

#include "units/units.h"

/* Window class name */
#define WND_CLASS_NAME "CGSG DT3!!!"

/* Forward declaration */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* Main startup program function.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInstance;
 *   - previouse application instance handle
 *     (not used, alway NULL):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - command line window show parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) system error level value.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;
  SetDbgMemHooks();

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;                      

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register wnd class", "ERROR", MB_ICONERROR | MB_OK );
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "CGSG PML #30 FOREVER!!", WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
    0, 0, 500, 300, NULL, NULL, hInstance, NULL);

  DT3_AnimInit(hWnd);
  DT3_AnimUnitAdd(DT3_UnitCreateControl());
  DT3_AnimUnitAdd(DT3_UnitCreateCow());
  DT3_AnimUnitAdd(DT3_UnitCreateLand());

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 30, 0);
  return msg.wParam;
}

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message type (see WM_***):
 *       UINT Msg;
 *   - message 'word' parameter:
 *       WPARAM wParam;
 *   - message 'long' parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  MINMAXINFO *minmax;
  HDC hDC;   
  PAINTSTRUCT ps;   

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_ACTIVATE:
    DT3_Anim.IsActive = LOWORD(wParam) != WA_INACTIVE;
    return 0;
  case WM_ENTERSIZEMOVE:
    DT3_Anim.IsActive = FALSE;
    return 0;
  case WM_EXITSIZEMOVE:
    DT3_Anim.IsActive = TRUE;
  case WM_CREATE:
    return 0;
  case WM_SIZE:
    DT3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    return 0;
  case WM_TIMER:
    DT3_AnimRender();
    DT3_AnimCopyFrame();
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DT3_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_CLOSE:
    break;
  case WM_DESTROY:
    DT3_AnimClose();
    KillTimer(hWnd, 30);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */ 

/* END OF 'main.c FILE */
