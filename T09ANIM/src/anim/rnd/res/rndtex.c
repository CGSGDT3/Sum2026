/* FILE NAME: rndtex.c
 * PROGRAMMER: DT3
 * DATE: 17.06.2026
 * PURPOSE: texture functional file in animation program.
 */

#include <stdio.h>    

#include "rndres.h"
#include "anim/rnd/rnd.h"

dt3TEXTURE DT3_RndTextures[DT3_MAX_TEXTURES]; /* Array of textures */
INT DT3_RndTexturesSize;                      /* Textures array store size */
      
/* Texture array initialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndTexInit( VOID )
{
  DT3_RndTexturesSize = 0;
} /* End of 'DT3_RndTexInit' function */

/* Texture array deinitialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndTexClose( VOID )
{
  memset(DT3_RndTextures, 0, sizeof(dt3TEXTURE) * DT3_RndTexturesSize);
  DT3_RndTexturesSize = 0;
} /* End of 'DT3_RndTexClose' function */

/* Add texture to stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size in pixels:
 *       INT W, H;
 *   - number of color components:
 *       INT C;
 *   - texture pixels data:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) texture stock number (-1 if error is occured).
 */
INT DT3_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;

  if (DT3_RndTexturesSize >= DT3_MAX_TEXTURES)
    return -1;

  /* Setup OpenGL texture */
  glGenTextures(1, &DT3_RndTextures[DT3_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, DT3_RndTextures[DT3_RndTexturesSize].TexId);


  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, W, H);

  if (Bits != NULL)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
                    C == 4 ? GL_BGRA : C == 3 ? GL_BGR : GL_RED, GL_UNSIGNED_BYTE, Bits);
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return DT3_RndTexturesSize++;
} /* End of 'DT3_RndTexAddImg' function */

/* Add texture from file to stock function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) texture stock number (-1 if error is occured).
 */
INT DT3_RndTexAddFromFile( CHAR *FileName )
{
  INT ret = -1;
  HBITMAP hBm;
  FILE *F;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP Bm;

    GetObject(hBm, sizeof(BITMAP), &Bm);
    if (Bm.bmBitsPixel == 24 || Bm.bmBitsPixel == 32 || Bm.bmBitsPixel == 8)
      ret = DT3_RndTexAddImg(FileName, Bm.bmWidth, Bm.bmHeight, Bm.bmBitsPixel >> 3, Bm.bmBits); 
    DeleteObject(hBm);
    return ret;
  }
  if ((F = fopen(FileName, "rb")) != NULL)
  {
    INT w = 0, h = 0, flen, components = -1;
    VOID *mem;

    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);

    fseek(F, 0, SEEK_END);
    flen = ftell(F);
    fseek(F, 4, SEEK_SET);

    if (w * h * 4 + 4 == flen)
      components = 4;
    else if (w * h * 3 + 4 == flen)
      components = 3;
    else if (w * h * 1 + 4 == flen)
      components = 1;

    if (components != -1)
      if ((mem = malloc(w * h * components)) != NULL)
      {
        fread(mem, components, w * h, F);
        ret = DT3_RndTexAddImg(FileName, w, h, components, mem); 
        free(mem);
      }
    fclose(F);
  }
  return ret;
} /* End of 'DT3_RndTexAddFromFile' function */     

/* END OF 'rndtex.c' FILE */