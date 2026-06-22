 /* FILE NAME: rndprim.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: primitive drawing file in animation program.
 */

#include "def.h"
#include "rnd.h"

#include <stdio.h>

/* Primitive free function.
 * ARGUMENTS:
 *   - pointer to primitive to be free:
 *       dt3PRIM *Pr;
 * RETURNS:
 *   None.
 */
VOID DT3_RndPrimFree( dt3PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(dt3PRIM));   
} /* End of 'DT3_RndPrimFree' function */


/* Creating primitive (memory allocation for vertices and indices) function.
 * ARGUMENTS:
 *   - pointer to primitive to be memory allocated:
 *       dt3PRIM *Pr;
 *   - number of vertices and indiced:
 *       INT NoofV, NoofI;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndPrimCreate( dt3PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

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
} /* End of 'DT3_RndPrimCreate' function */

/* Drawing primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to be draw:
 *       dt3PRIM *Pr;
 *   - world matrix:
 *       MATR World;
 * RETURNS:
 *   None.
 */
VOID DT3_RndPrimDraw( dt3PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, DT3_RndMatrVP);
  POINT *pnts;
  HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0)); 
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
} /* End of 'DT3_RndPrimDraw' function */

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - sphere radius:
 *       DBL R;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndPrimCreateSphere( dt3PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;

  if (!DT3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;

  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));

  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
} /* End of 'DT3_RndPrimCreateSphere' function */

/* Create cyllinder primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - cyllinder radius:
 *       DBL R;
  *   - cyllinder height:
 *       DBL Z;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndPrimCreateCyll( dt3PRIM *Pr, DBL R, DBL Z, INT W, INT H )
{
  INT i, j, k = 0;
  DBL phi, m;

  if (!DT3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;

  /* Fill vertex array */
  for (i = 0; i < H; i++)
  {
    m = (DBL)i / (H - 1) * Z; 

    for (j = 0; j < W; j++)
    {
      phi = j * 2 * PI / (W - 1);
      Pr->V[k++].P = VecSet(R * cos(phi), m,
                            R * sin(phi));
    }
  }

  /* Fill index array */
  k = 0;
  for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left triangle */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right triangle */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
    
  return TRUE;
} /* End of 'DT3_RndPrimCreateCyll' function */

/* Create thorus primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - inner radius:
 *       DBL Ri;
  *   - outer radius:
 *       DBL Ro;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndPrimCreateTh( dt3PRIM *Pr, DBL Ri, DBL Ro, INT W, INT H )
{
  INT i, j, k = 0;
  DBL phi, th = 0, x, y;

  if (!DT3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE; 

  /* Fill vertex array */
    for (i = 0; i < H; i++)
  {
    th = (DBL)i / (H - 1) * 2 * PI; 
  for (j = 0; j < W; j++)
    {         
      phi = (DBL)j / (W - 1) * 2 * PI; 
      y = (Ro + Ri * cos(phi)) * sin(th);
      x = (Ro + Ri * cos(phi)) * cos(th);
      Pr->V[k++].P = VecSet(x, y, sin(phi) * Ri);
    }
  }

  /* Fill index array */
  k = 0;
  for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left triangle */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right triangle */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }                    
  return TRUE;
} /* End of 'DT3_RndPrimCreateTh' function */

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       dt3PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL DT3_RndPrimLoad( dt3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[3000];

  memset(Pr, 0, sizeof(dt3PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices and indices */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';

      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
          n++;
        oldc = *ptr++;
      }

      nf += n - 2;
    }
  }

  if (!DT3_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }

  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n, n1, n2, n3;
      INT fvn = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';

      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
        {
          sscanf(ptr, "%d", &n);
          if (n > 0)
            n--;
          else
            if (n < 0)
              n = nv + n;

          if (fvn == 0)
            n1 = n;
          else if (fvn == 1)
            n2 = n;
          else
          {
            n3 = n;

            Pr->I[nf++] = n1;
            Pr->I[nf++] = n2;
            Pr->I[nf++] = n3;

            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'DT3_RndPrimLoad' function */

/* END OF 'rndprim.c FILE */