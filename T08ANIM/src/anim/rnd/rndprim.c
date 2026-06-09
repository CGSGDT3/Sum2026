#include "def.h"
#include "rnd.h"

static dt3PRIM Prim;
VOID dt3_RndPrimFree( dt3PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(dt3VERTEX));   /* <-- <string.h>, WinAPI, ZeroMemory */
}


BOOL dt3_RndPrimCreate( dt3PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  Pr->V = malloc(NoofV * sizeof(dt3VERTEX) + NoofI * sizeof(INT));
  Pr->I = (INT *)(Pr->V + NoofV);

  memset(Pr, 0, sizeof(dt3PRIM));
  size = sizeof(dt3VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);
  return TRUE;
}

VOID dt3_RndPrimDraw( dt3PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, DT3_RndMatrVP);
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  /* Build vertex projects */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(PrimV[i], wvp);


    pnts[i].x = (INT)((P.X + 1) * dt3_RndFrameW / 2);
    pnts[i].y = (INT)((-P.Y + 1) * dt3_RndFrameH / 2);
  }

  for (i = 0; i < NumOfI; i += 3)
  {
    MoveToEx(DT3_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL); 
    LineTo(DT3_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(DT3_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(DT3_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}
