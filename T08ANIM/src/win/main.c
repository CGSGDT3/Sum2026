 /* FILE NAME: main.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: Main file in animation program.
 */  

#include "def.h"
#include "anim/rnd/rnd.h"

#define WND_CLASS_NAME "CGSG DT3!!!"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT W, H;
  MINMAXINFO *minmax;
  HDC hDC;
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    DT3_RndInit(hWnd);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    DT3_RndResize(W, H);
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;
  case WM_KEYDOWN:
    return 0;
  case WM_TIMER:
    DT3_RndStart();
    InvalidateRect(hWnd, NULL, FALSE); 
    return 0;
  case WM_PAINT:   
    hDC = GetDC(hWnd);
    DT3_RndCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_CLOSE:
    break;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'main.c file */