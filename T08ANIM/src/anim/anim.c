DT3_AnimInit( HWND hWnd );
DT3_AnimClose( VOID );
DT3_AnimResize( INT W, INT H );
DT3_AnimCopyFrame( HDC hDC );
DT3_AnimRender( VOID );
VOID AnimUnitAdd( UNIT *Uni )
{
  if (Anim.NumOfUnits < MAX_UNITS)
    Anim.Units[Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &Anim);
}
DT3_AnimFlipFullScreen( VOID );
DT3_AnimExit( VOID );

DT3ANIM DT3_Anim;
