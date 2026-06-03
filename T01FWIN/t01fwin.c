/* FILE NAME: t01fwin.c
 * PROGRAMMER: DT3
 * DATE: 01.06.2026
 * PURPOSE: WinAPI first sample.
 */
#include <windows.h>
#define WND_CLASS_NAME "CGSG DT3!!!"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
  CreateWindow("BUTTON", "Press me...", WS_CHILD | WS_VISIBLE,
    10, 10, 100, 30, hWnd, (HMENU)123, hInstance, NULL);
    CreateWindow("BUTTON", "Expand", WS_CHILD | WS_VISIBLE,
    10, 47, 100, 30, hWnd, (HMENU)124, hInstance, NULL);
  CreateWindow("BUTTON", "Select me", BS_CHECKBOX | WS_CHILD | WS_VISIBLE | WS_BORDER,
    10, 84, 100, 30, hWnd, (HMENU)125, hInstance, NULL);

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
  POINT pt;
  static BOOL is_select = FALSE;
  static INT W, H;
  DWORD style, state;
 
  switch (Msg)
  {
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    return 0;
 case WM_COMMAND:
    if (LOWORD(wParam) == 123)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (LOWORD(wParam) == 124)
    {
     style  = GetWindowLong(hWnd, GWL_STYLE);
 
      if (style & WS_MAXIMIZE)
        ShowWindow(hWnd, SW_NORMAL);
      else
        ShowWindow(hWnd, SW_MAXIMIZE);
    }
    else if (LOWORD(wParam) == 125)
    {
      state = SendMessage((HWND)lParam, BM_GETSTATE, 0, 0);
 
      is_select = !!(state & BST_CHECKED);
 
      if (is_select)
        SendMessage((HWND)lParam, BM_SETCHECK, BST_UNCHECKED, 0);
      else
        SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
      InvalidateRect(hWnd, NULL, FALSE);
    }
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
 
    SetDCBrushColor(hDC, RGB(18 * 255 / 100, 30 * 255 / 100, 47 * 255 / 100));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    Rectangle(hDC, 100, 100, 300, 200);

    SetDCBrushColor(hDC, RGB(47 * 255 / 100, 18 * 255 / 100, 30 * 255 / 100));
    if (is_select)
      SelectObject(hDC, GetStockObject(DC_BRUSH));
    else
      SelectObject(hDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(255, 0, 0));
 
    Ellipse(hDC, 0, 0, W, H);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
 
    SetDCPenColor(hDC, RGB(0, 255, 0));
    SetDCBrushColor(hDC, RGB(0, 255, 0));

    Ellipse(hDC, pt.x - 5, pt.y - 5, pt.x + 5, pt.y + 5);
 
    EndPaint(hWnd, &ps);
    return 0;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_CLOSE:
    break;
  case WM_DESTROY:
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}