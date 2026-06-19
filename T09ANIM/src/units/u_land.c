/* FILE NAME: u_Land.c
 * PROGRAMMER: DT3
 * DATE: 11.06.2026
 * PURPOSE: unit Land file in animation program.
 */ 

#include "units.h"

/* Structure of Land unit */
typedef struct tagdt3UNIT_LAND
{
  UNIT_BASE_FIELDS; /* Basic unit functions */ 
  dt3PRIM Land;     /* Primitives to drawing */
  VEC Pos;          /* Vector of position */
  DBL Size;         /* Size of Land */
  dt3GRID G;
} dt3UNIT_LAND;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_LAND *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( dt3UNIT_LAND *Uni, dt3ANIM *Ani )
{
  HBITMAP hBm;
  BITMAP bm;
  INT w, h, x, y;

  Uni->Size = 1000;

  if ((hBm = LoadImage(NULL, "bin/heights/hf.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if (bm.bmBitsPixel == 8 && DT3_RndGridCreate(&Uni->G, w, h))
    {
      BYTE *Bits = bm.bmBits;

      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x];

          Uni->G.V[y * w + x].P = VecSet(x / (w - 1.0),
                                    hgt / 255.0,
                                    1 - y / (h - 1.0));
        }
      DT3_RndGridAutoNormals(&Uni->G);
      DT3_RndPrimFromGrid(&Uni->Land, &Uni->G);
    }                                  
  }
  Uni->Pos = VecSet1(0);
} /* End of 'DT3_UnitInit' function */ 


/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dt3UNIT_LAND *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( dt3UNIT_LAND *Uni, dt3ANIM *Ani )
{
  DT3_RndPrimDraw(&Uni->Land, MatrMulMatr(MatrTranslate(Uni->Pos),
    MatrScale(VecSet1(Uni->Size))));
} /* End of 'DT3_UnitRender' function */ 

/* Unit deinitialization function.
 * ARGUMENTS:                            
 *   - self-pointer to unit object:
 *       dt3UNIT_LAND *Uni;
 *   - animation context:
 *       dt3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( dt3UNIT_LAND *Uni, dt3ANIM *Ani )
{
  DT3_RndGridFree(&Uni->G);
  DT3_RndPrimFree(&Uni->Land);
} /* End of 'DT3_UnitClose' function */ 

/* Unit Land creation function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3UNIT *) pointer to created unit.
 */
dt3UNIT * DT3_UnitCreateLand( VOID )
{
  dt3UNIT_LAND *Uni;

  if ((Uni = (dt3UNIT_LAND *)DT3_AnimUnitCreate(sizeof(dt3UNIT_LAND))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
} /* End of 'DT3_UnitCreateLand' function */ 

/* END OF 'u_land.c' FILE */
