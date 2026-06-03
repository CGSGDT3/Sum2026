 /* FILE NAME: t02arrow.c
 * PROGRAMMER: DT3
 * DATE: 02.06.2026
 * PURPOSE: arrows.
 */
#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "CGSG DT3!!!"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, HWND hWnd );

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
  HDC hDC;
  PAINTSTRUCT ps;
  static INT W, H;
  INT i, j;
  POINT pt;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
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
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);

    SetDCBrushColor(hMemDC, RGB(18 * 255 / 100, 30 * 255 / 100, 47 * 255 / 100));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));  
    Rectangle(hMemDC, 0, 0, W, H); 
  
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
  
    for (i = 0; i < W; i += 125)
      for (j = 0; j < H; j += 125)
        DrawArrow(hMemDC, i, j, 120, 30.47, hWnd);       
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

void DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, HWND hWnd )
{
  POINT pt, pnts[] =
  {
    {0, 30 * 2}, {0, -47}, {47, 0} 
  };
  POINT pnts_res[3];
  INT I;
  DOUBLE Le, cosa, sina;
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  Le = hypot((pt.x - Xc), (pt.y - Yc));
  sina = (pt.x - Xc) / Le, cosa = -(pt.y - Yc) / Le;
  for (I = 0; I < 3; I++)               
  {
    pnts_res[I].x = pnts[I].x * cosa + pnts[I].y * sina + Xc;
    pnts_res[I].y = -(pnts[I].y * cosa - pnts[I].x * sina) + Yc;       
  }
  SetDCBrushColor(hDC, RGB(47 * 255 / 100, 18 * 255 / 100, 30 * 255 / 100));

  Polygon(hDC, pnts_res, 3);
}