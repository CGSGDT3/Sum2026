#include "anim.h"
#include "rnd/rnd.h"
dt3ANIM DT3_Anim;

VOID  DT3_AnimInit( HWND hWnd )
{      
  memset(&DT3_Anim, 0, sizeof(dt3ANIM)); 
  DT3_RndInit(hWnd);
  DT3_Anim.hWnd = DT3_hRndWnd;
  DT3_Anim.hDC = DT3_hRndDCFrame;
  DT3_Anim.W = DT3_RndFrameW;
  DT3_Anim.H = DT3_RndFrameH;
}
VOID DT3_AnimClose( VOID )
{
  INT i;
  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Close(DT3_Anim.Units[i], &DT3_Anim), free(DT3_Anim.Units[i]);
  DT3_RndClose();
  memset(&DT3_Anim, 0, sizeof(dt3ANIM)); 
}
VOID DT3_AnimResize( INT W, INT H )
{
  DT3_RndResize(W, H); 
  DT3_Anim.W = DT3_RndFrameW;
  DT3_Anim.H = DT3_RndFrameH;   
  DT3_AnimRender();
}
VOID DT3_AnimCopyFrame( HDC hDC )
{
  DT3_RndCopyFrame(hDC);
}
VOID DT3_AnimRender( VOID )
{
  INT i;
  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Response(DT3_Anim.Units[i], &DT3_Anim);
  DT3_RndStart();
  for (i = 0; i < DT3_Anim.NumOfUnits; i++)
    DT3_Anim.Units[i]->Render(DT3_Anim.Units[i], &DT3_Anim);
  DT3_RndEnd();
}
VOID DT3_AnimUnitAdd( dt3UNIT *Uni )
{
  if (DT3_Anim.NumOfUnits < dt3_MAX_UNITS)
    DT3_Anim.Units[DT3_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &DT3_Anim);
}