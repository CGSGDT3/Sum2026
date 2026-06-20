/* FILE NAME  : rndfnt.c
 * PROGRAMMER : DT3
 * LAST UPDATE: 19.06.2026
 * PURPOSE    : 3D animation project.
 *              Rendering resources implementation module.
 *              Resource implemetation module.
 *              Font handle module.
 */

#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/* Current font description */
/* Font data */
static dt3FONT DT3_RndFntFont;

/* Font characters primitives */
static dt3PRIM DT3_RndFntChars[256];

/* Font material */
static INT DT3_RndFntMtlNo;

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DT3_RndFntLoad( CHAR *FileName )
{
  DWORD Sign, W, H, *Tex;
  dt3VERTEX Chars[256][4];
  dt3MATERIAL mtl;
  FILE *F;
  INT i;

  DT3_RndFntClose();

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }

  fread(&DT3_RndFntFont, sizeof(dt3FONT), 1, F);
  fread(Chars, sizeof(dt3VERTEX), 4 * 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
  if ((Tex = malloc(W * H * 4)) == NULL)
  {
    memset(&DT3_RndFntFont, 0, sizeof(DT3_RndFntFont));
    memset(&DT3_RndFntFont, 0, sizeof(DT3_RndFntFont));
    fclose(F);
    return FALSE;
  }
  fread(Tex, 4, W * H, F);
  fclose(F);

  /* Material */
  mtl = DT3_RndMtlGetDef();
  strncpy(mtl.Name, FileName, DT3_STR_MAX - 1);
  mtl.Tex[0] = DT3_RndTexAddImg(FileName, W, H, 4, Tex);
  mtl.ShdNo = DT3_RndShdAdd("font");
  DT3_RndFntMtlNo = DT3_RndMtlAdd(&mtl);

  for (i = 0; i < 256; i++)
  {
    DT3_RndPrimCreate(&DT3_RndFntChars[i], DT3_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    DT3_RndFntChars[i].MtlNo = DT3_RndFntMtlNo;
  }
  free(Tex);
  return TRUE;
} /* End of 'DT3_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndFntInit( VOID )
{
  DT3_RndFntLoad("bin/fonts/ConsolasBold.g3df");
} /* End of 'DT3_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    DT3_RndPrimFree(&DT3_RndFntChars[i]);
  memset(DT3_RndFntChars, 0, sizeof(DT3_RndFntChars));
  memset(&DT3_RndFntFont, 0, sizeof(DT3_RndFntFont));
} /* End of 'DT3_RndFntInit' function */

/* Draw screen space string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   - font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID DT3_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  VEC Start = Pos;

  if (Size <= 0)
    Size = DT3_RndFntFont.LineH;

  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = Start.X, Pos.Y -= Size;
    else if (DT3_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
    {
      DT3_RndPrimDraw(&DT3_RndFntChars[(UCHAR)*Str],
        MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
      Pos.X += DT3_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
    }
    Str++;
  }
} /* End of 'DT3_RndFntDraw' function */

/* END OF 'rndfnt.c' FILE */