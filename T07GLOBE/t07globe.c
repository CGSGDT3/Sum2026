 /* FILE NAME: t07globe.c
 * PROGRAMMER: DT3
 * DATE: 06.06.2026
 * PURPOSE: Main file in drawing globe program.
 */  
#include "globe.h"
#include "timer.h"

#define WND_CLASS_NAME "CGSG DT3!!!"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID FlipFullScreen( HWND hWnd );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

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
      SendMessage(hWnd, WM_TIMER, 30.47, 0);
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  static INT W, H;
  static HDC hMemDC;
  static HBITMAP hBm;
  HPEN hPen;
  HBRUSH hBrush;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:            
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    hDC = GetDC(hWnd);
    GLB_Init(1);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    GLB_TimerInit();
    SetTimer(hWnd, 30, 1, NULL);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    GLB_Resize(W, H);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == 'P')
      GLB_IsPause = !GLB_IsPause;
    if (wParam == 'F')
      FlipFullScreen(hWnd);
    return 0;
  case WM_TIMER:
    GLB_TimerResponse();
    hPen = CreatePen(PS_NULL, 5, RGB(0, 0, 0));
    hBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hMemDC, hPen);
    SelectObject(hMemDC, hBrush);

    Rectangle(hMemDC, 0, 0, W, H);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    GLB_Draw(hMemDC);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_CLOSE:
    break;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);

    KillTimer(hWnd, 30);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
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
/* End of 't07globe.c file */