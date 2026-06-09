#include "def.h"
#include "rnd.h"

static dt3PRIM Prim;
VOID dt3_RndPrimFree( dt3PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(dt3PRIM));   
}


BOOL dt3_RndPrimCreate( dt3PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;
  memset(Pr, 0, sizeof(dt3PRIM));
  Pr->V = malloc(NoofV * sizeof(dt3VERTEX) + NoofI * sizeof(INT));
  Pr->I = (INT *)(Pr->V + NoofV);


  size = sizeof(dt3VERTEX) * NoofV + sizeof(INT) * NoofI;

  if (Pr->V == NULL)
    return FALSE;
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
  HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); 
  SelectObject(DT3_hRndDCFrame, hPen);

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  /* Build vertex projects */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);


    pnts[i].x = (INT)((p.X + 1) * DT3_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * DT3_RndFrameH / 2);
  }

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(DT3_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL); 
    LineTo(DT3_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(DT3_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(DT3_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);  
  DeleteObject(hPen);
}
