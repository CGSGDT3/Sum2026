/* Topunov Denis, 10-3, 01.06.2026 */
#include <windows.h>
#include <string.h>

void main( void )
{ 
  char s[100], info[] = "CGSG PML #30 FOREVER!!!", tmp[100];
  int a = 0, b = 100, m, ans, res, flag = 0;

  wsprintf(tmp, "Hello dear friend! "
    "choose a numb from %i to %i "
    "and remember it", a, b);
  if (MessageBox(NULL, tmp, info, MB_OKCANCEL) == IDOK)
  {
    while (1)
    {
      m = (a + b) / 2;
      wsprintf(s, "Is your number more than %i", m);
      ans = MessageBox(NULL, s, info, MB_YESNOCANCEL);
      if (ans == IDYES)
      {
        a = m + 1;
        if (a > b) 
        {
          flag = 1;
          break;
        }
      }
      else if (ans == IDNO)
      {
        b = m - 1;
        if (a > b)
        {
          flag = 1;
          break;
        }
      }
      else
      {
        res = m;
        break;
      }
    }
    if (flag != 1)
      wsprintf(s, "Your number is %i", m);
    else
      wsprintf(s, "You are bad liar");
    MessageBox(NULL, s, info, MB_OK);
  }
  else
    MessageBox(NULL, "Bye-bye", info, MB_OK);
} 