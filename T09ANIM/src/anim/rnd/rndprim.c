 /* FILE NAME: rndprim.c
 * PROGRAMMER: DT3
 * DATE: 09.06.2026
 * PURPOSE: primitive drawing file in animation program.
 */

#include "def.h"
#include "rnd.h"

#include <stdio.h>
  
/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dt3PRIM *Pr;
 *   - primitive type:
 *       dt3PRIM_TYPE Type;
 *   - vertex attributes array:
 *       dt3VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID DT3_RndPrimCreate( dt3PRIM *Pr, dt3PRIM_TYPE Type,
                        dt3VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  memset(Pr, 0, sizeof(dt3PRIM));
  Pr->Trans = MatrIdentity();
  Pr->Type = Type;

  glGenVertexArrays(1, &Pr->VA);

  /* Vertex data */
  if (V != NULL && NoofV != 0)
  {
    glBindVertexArray(Pr->VA);
    glGenBuffers(1, &Pr->VBuf);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dt3VERTEX) * NoofV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(dt3VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(dt3VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(dt3VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(dt3VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    /* Obtain BB */
    if (NoofV > 0)
    {
      INT i;

      Pr->MinBB = Pr->MaxBB = V[0].P;
      for (i = 1; i < NoofV; i++)
      {
        Pr->MinBB = VecMinVec(Pr->MinBB, V[i].P);
        Pr->MaxBB = VecMaxVec(Pr->MaxBB, V[i].P);
      }
    }
  }

  /* Index data */
  if (Ind != NULL && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Ind, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;
} /* End of 'DT3_RndPrimCreate' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive to be draw:
 *       dt3PRIM *Pr;
 *   - transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DT3_RndPrimDraw( dt3PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr3(Pr->Trans, World, DT3_RndMatrVP);
  INT prim_type =
    Pr->Type == DT3_RND_PRIM_LINES ? GL_LINES :
    Pr->Type == DT3_RND_PRIM_TRIMESH ? GL_TRIANGLES :
    GL_POINTS;

  glLoadMatrixf(wvp.A[0]);

  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(prim_type, 0, Pr->NumOfElements);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
} /* End of 'DT3_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be free:
 *       dt3PRIM *Pr;
 * RETURNS: None.
 */
VOID DT3_RndPrimFree( dt3PRIM *Pr )
{
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->VBuf);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(dt3PRIM));
} /* End of 'DT3_RndPrimFree' function */  

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
  INT i, j, k, nv =  W * H, nf = (H - 1) * (W - 1) * 6, size;
  DBL theta, phi;
  dt3VERTEX *V;
  INT *Ind;

  size = sizeof(dt3VERTEX) * nv + sizeof(INT) * nf * 3;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);

  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));
  k = 0;
  for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      Ind[k++] = i * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j + 1;
    }      
  DT3_RndPrimTriMeshAutoNormals(V, nv, Ind, nf);

  for (i = 0; i < nv; i++) 
  {
    VEC L = VecNormalize(VecSet1(1)); 
    FLT nl = VecDotVec(L, V[i].N);
    V[i].C = VecSet4(nl * 0.8, nl * 0.47, nl * 0.30, 1);
  }

  DT3_RndPrimCreate(Pr, DT3_RND_PRIM_TRIMESH, V, nv, Ind, nf);   

  free(V);
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
  INT i, m, j, k, nv =  W * H, nf = (H - 1) * (W - 1) * 6, size;
  DBL phi;
  dt3VERTEX *V;
  INT *Ind;

  size = sizeof(dt3VERTEX) * nv + sizeof(INT) * nf * 3;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);

  for (k = 0, i = 0; i < H; i++)
  {
    m = (DBL)i / (H - 1) * Z; 

    for (j = 0; j < W; j++)
    {
      phi = j * 2 * PI / (W - 1);
     V[k++].P = VecSet(R * cos(phi), m,
                            R * sin(phi));
    }
  }
  k = 0;
  for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      Ind[k++] = i * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j + 1;
    }      
  DT3_RndPrimTriMeshAutoNormals(V, nv, Ind, nf);

  DT3_RndPrimCreate(Pr, DT3_RND_PRIM_TRIMESH, V, nv, Ind, nf);   

  free(V);
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
  INT i, x, y, th, j, k, nv =  W * H, nf = (H - 1) * (W - 1) * 6, size;
  DBL phi;
  dt3VERTEX *V;
  INT *Ind;

  size = sizeof(dt3VERTEX) * nv + sizeof(INT) * nf * 3;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);

  for (k = 0,i = 0; i < H; i++)
  {
    th = (DBL)i / (H - 1) * 2 * PI; 
    for (j = 0; j < W; j++)
    {         
      phi = (DBL)j / (W - 1) * 2 * PI; 
      y = (Ro + Ri * cos(phi)) * sin(th);
      x = (Ro + Ri * cos(phi)) * cos(th);
      V[k++].P = VecSet(x, y, sin(phi) * Ri);
    }
  }

  k = 0;
  for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      Ind[k++] = i * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j + 1;
    }      
  DT3_RndPrimTriMeshAutoNormals(V, nv, Ind, nf);

  DT3_RndPrimCreate(Pr, DT3_RND_PRIM_TRIMESH, V, nv, Ind, nf);   

  free(V);
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
  INT i, nv = 0, nf = 0;
  static CHAR Buf[3000];
  dt3VERTEX *V;
  INT *Ind; 
  DBL size;

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
  size = sizeof(dt3VERTEX) * nv + sizeof(INT) * nf * 6;
  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Ind = (INT *)(V + nv);
  memset(V, 0, size);

  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
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

            Ind[nf++] = n1;
            Ind[nf++] = n2;
            Ind[nf++] = n3;

            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
  DT3_RndPrimTriMeshAutoNormals(V, nv, Ind, nf);

  for (i = 0; i < nv; i++) 
  {
    VEC L = VecNormalize(VecSet1(1)); 
    FLT nl = VecDotVec(L, V[i].N);
    V[i].C = VecSet4(nl * 0.8, nl * 0.47, nl * 0.30, 1);
  }
  DT3_RndPrimCreate(Pr, DT3_RND_PRIM_TRIMESH, V, nv, Ind, nf);
  free(V);   
  return TRUE;
} /* End of 'DT3_RndPrimLoad' function */

/* Tri-mesh geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - vertex array:
 *       dt3VERTEX *V;
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