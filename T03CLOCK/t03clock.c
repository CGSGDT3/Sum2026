 /* FILE NAME: t03clock.c
 * PROGRAMMER: DT3
 * DATE: 03.06.2026
 * PURPOSE: clocks.
 */

#include <windows.h>
#include <math.h>
#include <time.h>
#define WND_CLASS_NAME "CGSG DT3!!!"    
#define PI 3.14159265359

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, INT H, HWND hWnd );

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

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  RECT rc;
  HDC hDC;
  HPEN hPen;
  HBRUSH hBrush;
  HFONT hFnt;
  CHAR Buf[100];
  PAINTSTRUCT ps;
  static INT W, H;
  DOUBLE a;
  SYSTEMTIME st;
  static HDC hMemDC, hDCClock;
  static HBITMAP hBm, hBmClock;
  static BITMAP bmpInfo;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hDCClock = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 1, NULL);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    if (hBmClock != NULL)
      DeleteObject(hBmClock);
    hBmClock = LoadImage(NULL, "clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    GetObject(hBmClock, sizeof(BITMAP), &bmpInfo);
 
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SelectObject(hDCClock, hBmClock);

    return 0;
  case WM_TIMER:
    hFnt = CreateFont(47, 0, 0, 0, FW_BOLD,
      TRUE, FALSE, FALSE, RUSSIAN_CHARSET,
      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, VARIABLE_PITCH | FF_ROMAN,
      "Times");

    rc.left = 18;
    rc.top = 18;
    rc.right = W - 18;
    rc.bottom = H - 18;
    hDC = GetDC(hWnd);

    hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hMemDC, hPen);
    SelectObject(hMemDC, hBrush);  
    
    Rectangle(hMemDC, 0, 0, W, H); 
  
    BitBlt(hMemDC, (W - bmpInfo.bmWidth) / 2, (H - bmpInfo.bmHeight) / 2, W, H, hDCClock, 0, 0, SRCCOPY);
    GetLocalTime(&st);

    hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    SelectObject(hMemDC, hPen);
    a = - PI * (st.wSecond % 60) / 30 + PI;

    MoveToEx(hMemDC, W / 2, H / 2, NULL);
    LineTo(hMemDC, W / 2 + 0.75 * bmpInfo.bmWidth * sin(a) / 2, 0.75 * bmpInfo.bmHeight * cos(a) / 2 + H / 2);

    hPen = CreatePen(PS_SOLID, 10, RGB(47 * 255 / 100, 18 * 255 / 100, 30 * 255 / 100));
    SelectObject(hMemDC, hPen);
    a = - PI * (st.wMinute + (st.wSecond % 60) / 60.0) / 30 + PI;                                   
    MoveToEx(hMemDC, W / 2, H / 2, NULL);
    LineTo(hMemDC, W / 2 + bmpInfo.bmWidth * sin(a) / 4, bmpInfo.bmHeight * cos(a) / 4 + H / 2);

    hPen = CreatePen(PS_SOLID, 15, RGB(18 * 255 / 100, 30 * 255 / 100, 47 * 255 / 100));
    SelectObject(hMemDC, hPen);
    a = - PI * (st.wHour % 12 + (st.wMinute % 60) / 60.0) / 6 + PI;
    MoveToEx(hMemDC, W / 2, H / 2, NULL);
    LineTo(hMemDC, W / 2 + bmpInfo.bmWidth * sin(a) / 8,  bmpInfo.bmHeight * cos(a) / 8 + H / 2);

    SelectObject(hMemDC, hFnt);
    DrawText(hMemDC, Buf, wsprintf(Buf, ">>> %i::%i::%i <<<", st.wHour, st.wMinute, st.wSecond), &rc,
      DT_CENTER);
    DeleteObject(hFnt);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hDC);  
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



