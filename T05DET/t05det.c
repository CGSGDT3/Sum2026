#include <windows.h>
#include <stdio.h>
#include <conio.h>

typedef DOUBLE DBL;

#define MAX 10
#define MAXF 3628800
static DBL A[MAX][MAX];
static INT N;
static INT p[MAX];
static BOOL IsParity, Par[MAXF];
static INT Perm[MAXF][MAX];
static INT k;

VOID Swap( INT *a, INT *b )
{
  INT tmp = *a;
  
  *a = *b, *b = tmp;
}

VOID Go( INT Pos )
{
  INT i;

  if (Pos == MAX)
  {
    for (i = 0; i < MAX; i++)
      Perm[k][i] = p[i];
    Par[k] = IsParity;  
    k++;
  }
  else
  {
    if (Pos == 0)
      for (i = 0; i < MAX; i++)
        p[i] = i;
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

BOOL LoadMatrix( char *FileName )
{
  FILE *F;
  INT i, j;
  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &N);
  if (N < 0)
    N = 0;
  else 
    if (N > MAX)
      N = MAX;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
}
VOID main( VOID )
{
  INT i, j;
  DOUBLE prod = 0, det = 0;
  IsParity = TRUE; 
  k = 0;          

  Go(0);           
  LoadMatrix("IN.TXT");  
      

  for (j = 0; j < MAXF; j++)
  {
      prod = 1;
      for (i = 0; i < N; i++)
          prod *= A[i][Perm[j][i]];

      det += (Par[j] ? 1 : -1) * prod;
  }
  printf("Det:%lf", det);
  _getch(); 
}
