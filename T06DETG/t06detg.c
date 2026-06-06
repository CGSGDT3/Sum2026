 /* FILE NAME: t06detg.c
 * PROGRAMMER: DT3
 * DATE: 05.06.2026
 * PURPOSE: Matrix determinate by Gauss method.
 */

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

typedef DOUBLE DBL;

#define MAX 10
static DBL A[MAX][MAX];
static INT N;


VOID Debug( VOID )
{
  INT i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
      printf("%10.3f", A[i][j]);
    printf("\n");
  }
  printf("\n\n");
}

VOID Swap( DOUBLE *a, DOUBLE *b )
{
  DOUBLE tmp = *a;
  
  *a = *b, *b = tmp;
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
  INT i, x, y, max_row, max_col, flag = 0, j, k, m;
  DOUBLE det, coef;

  LoadMatrix("IN.TXT");  
  Debug();

  det = 1;
  for (i = 0; i < N; i++)
  {
    /* look for maximum matrix element */
    if (flag == 0)
      max_row = max_col = i, flag = 1;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_row = y, max_col = x;

    if (A[max_row][max_col] == 0)
    {
      det = 0;
      break;
    }
    /* move max element to [i][i] position */
    if (max_row != i)
    {
      /* Swap max_row and i row (elements/columns: [i..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[max_row][x], &A[i][x]);
      det = -det;
    }
    if (max_col != i)
    {
      /* Swap max_col and i column (elements/columns: [0..N-1]) */
      for (m = i; m < N; m++)
        Swap(&A[m][max_col], &A[m][i]); 
      det = -det;
    }
    printf("Debugging:\n");
    Debug();
      /* Subtrack from every row k:[i+1..N-1] row [i] multipled by (A[k][i] / A[i][i]) */
      for (j = i + 1; j < N; j++)
      {
        coef = A[j][i] / A[i][i];
        A[j][i] = 0;
        for (k = i + 1; k < N; k++)
          A[j][k] -= A[i][k] * coef;
      }
      det *= A[i][i];
      printf("Gaussing:\n");
      Debug();
  }
  printf("Det:%lf", det);

  _getch(); 
}
