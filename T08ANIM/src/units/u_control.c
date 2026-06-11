#include "units.h"

#include <string.h> 
#include <mmsystem.h>

#pragma comment(lib, "winmm")

typedef struct tagdt3UNIT_CONTROL dt3UNIT_CONTROL;
struct tagdt3UNIT_CONTROL
{
  UNIT_BASE_FIELDS;

  VEC CamLoc, CamDir, CamUp;
  DBL Speed, AngleSpeed;
};

static VOID DT3_UnitInit( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
  Uni->CamLoc = VecSet1(5), Uni->CamDir = VecSet1(1), Uni->CamUp = VecSet(0, 1, 0), Uni->Speed = 2, Uni->AngleSpeed = 12;
}

static VOID DT3_UnitResponse( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
  POINT pt;
  INT i;

  GetCursorPos(&pt);
  ScreenToClient(Ani->hWnd, &pt);
  Ani->Mdx = pt.x - Ani->Mx;
  Ani->Mdy = pt.y - Ani->My;

  Ani->Mx = pt.x;
  Ani->My = pt.y;

  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->DeltaTime * Uni->AngleSpeed * Ani->Mdx));
  GetKeyboardState(Ani->Keys);

  for (i = 0; i < 256; i++)
  {
    Ani->Keys[i] >>= 7;
    Ani->KeysClick[i] = Ani->Keys[i] && !Ani->KeysOld[i];
  }

  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
    (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));
  memcpy(Ani->KeysOld, Ani->Keys, 256); 


  DT3_RndCamSet(Uni->CamLoc, Uni->CamDir, Uni->CamUp); 

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        for (i = 0; i < 32; i++)
        {
          Ani->JBut[i] = (ji.dwButtons >> i) & 1;
          Ani->JButClick[i] = Ani->JBut[i] && !Ani->JButOld[i];
          Ani->JButOld[i] = Ani->JBut[i];
        }

        Ani->JX = DT3_GET_JOYSTIC_AXIS(X);
        Ani->JY = DT3_GET_JOYSTIC_AXIS(Y);
        Ani->JZ = DT3_GET_JOYSTIC_AXIS(Z);
        Ani->JR = DT3_GET_JOYSTIC_AXIS(R);

        Ani->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }                            
    }
  }
}

static VOID DT3_UnitRender( dt3UNIT_CONTROL *Uni, dt3ANIM *Ani )
{
}

static VOID DT3_UnitClose( dt3UNIT *Uni, dt3ANIM *Ani )
{
}

dt3UNIT * DT3_UnitCreateControl( VOID )
{
  dt3UNIT_CONTROL *Uni;

  if ((Uni = (dt3UNIT_CONTROL *)DT3_AnimUnitCreate(sizeof(dt3UNIT_CONTROL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
}
