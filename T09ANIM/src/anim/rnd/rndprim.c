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
VOID dt3_RndPrimFree( dt3PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(dt3PRIM));   
} /* End of 'dt3_RndPrimFree' function */


/* Creating primitive (memory allocation for vertices and indices) function.
 * ARGUMENTS:
 *   - pointer to primitive to be memory allocated:
 *       dt3PRIM *Pr;
 *   - number of vertices and indiced:
 *       INT NoofV, NoofI;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL dt3_RndPrimCreate( dt3PRIM *Pr, INT NoofV, INT NoofI )
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
} /* End of 'dt3_RndPrimCreate' function */

/* Drawing primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to be draw:
 *       dt3PRIM *Pr;
 *   - world matrix:
 *       MATR World;
 * RETURNS:
 *   None.
 */
VOID dt3_RndPrimDraw( dt3PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, DT3_RndMatrVP);

  glLoadMatrixf(wvp.A[0]);

  /* Draw triangles by edges */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {           
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'dt3_RndPrimDraw' function */

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
BOOL dt3_RndPrimCreateSphere( dt3PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;

  if (!dt3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
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
} /* End of 'dt3_RndPrimCreateSphere' function */

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
BOOL dt3_RndPrimCreateCyll( dt3PRIM *Pr, DBL R, DBL Z, INT W, INT H )
{
  INT i, j, k = 0;
  DBL phi, m;

  if (!dt3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
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
} /* End of 'dt3_RndPrimCreateCyll' function */

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
BOOL dt3_RndPrimCreateTh( dt3PRIM *Pr, DBL Ri, DBL Ro, INT W, INT H )
{
  INT i, j, k = 0;
  DBL phi, th = 0, x, y;

  if (!dt3_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
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
} /* End of 'dt3_RndPrimCreateTh' function */

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       dt3PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL dt3_RndPrimLoad( dt3PRIM *Pr, CHAR *FileName )
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

  if (!dt3_RndPrimCreate(Pr, nv, nf * 3))
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
} /* End of 'dt3_RndPrimLoad' function */

/* Tri-mesh geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - vertex array:
 *       vg4VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *Ind;
 *   - index array size:
 *       INT NumOfI;
 */
VOID DT3_RndPrimTriMeshAutoNormals( dt3VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;

 for (i = 0; i < NumOfV; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < NumOfI; i += 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

    V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
    V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
    V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }  

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'DT3_RndPrimTriMeshAutoNormals' function */

/* END OF 'rndprim.c FILE */