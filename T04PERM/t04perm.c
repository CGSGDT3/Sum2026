#define MAX 4

#include <windows.h>
#include <stdio.h>

static INT p[MAX];
static BOOL IsParity;

VOID Store( VOID )
{
  INT i;
  FILE *F;

  F = fopen("PERM", "a");
  if (F == NULL)
    return;
  for (i = 0; i < MAX - 1; i++)
    fprintf(F, "%i->", p[i]);
  fprintf(F, "%i - parity: %s\n", p[MAX - 1], IsParity ? "even" : "odd");
  fclose(F);
}


VOID Swap( INT *a, INT *b )
{
  INT tmp = *a;

  *a = *b, *b = tmp;
}

VOID Go( INT Pos )
{
  INT i;

  if (Pos == MAX)
    Store();
  else
  {
    if (Pos == 0)
      for (i = 0; i < MAX; i++)
        p[i] = i + 1;
    for (i = Pos; i < MAX; i++)
    {
      Swap(&p[Pos], &p[i]);
      if (Pos != i)
        IsParity = !IsParity;
      Go(Pos + 1);
      Swap(&p[Pos], &p[i]);
      if (Pos != i)
        IsParity = !IsParity;
    }
  }
}

VOID main( VOID )
{
  IsParity = TRUE;
  Go(0);
}

