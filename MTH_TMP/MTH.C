#include "MTH.H"
#include <conio.h>

VOID main( VOID )
{
  MATR a =
  {
    {
      {2.4, 23.4, 45.412, 43.23},
      {4.4, 5.5, 3.3, 7.45},
      {12.11, 3.3, 54.8, 856.45},
      {32.3, 34.2, 8.9, 74.423},
    }
  };
  MATR b = MatrInverse(a);
  VEC A = VecSet(1, 30, 2);
  PrintMatrix(MatrMulMatr(a, b));
  printf("%f", MatrDeterm(a));
  PrintVector(VecNeg(A)); 
  printf("\n%f", VecDotVec(A, VecNeg(A)));
  _getch();
}