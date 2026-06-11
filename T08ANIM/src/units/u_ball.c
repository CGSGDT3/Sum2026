#include "units.h"

#include <time.h>

typedef struct tagdt3UNIT_BALL dt3UNIT_BALL;
struct tagdt3UNIT_BALL
{
  UNIT_BASE_FIELDS;
  dt3PRIM Ball;
  VEC Pos;
  DBL Size, Speed;
};

static VOID DT3_UnitInit( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(3 * rand1(), 3 * rand1(), rand1());
  Uni->Size = (DBL)rand() / RAND_MAX;
  if (Uni->Size < 0.08)
    Uni->Size = 0.08;
  Uni->Speed = 5 * (DBL)rand() / RAND_MAX;
  dt3_RndPrimCreateSphere(&Uni->Ball, Uni->Size, Gr_W, Gr_H);
}

static VOID DT3_UnitResponse( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  Uni->Pos = VecSet(Uni->Pos.X, sin(Uni->Speed * 3 * clock() / 1000.0), Uni->Pos.Z);
}

static VOID DT3_UnitRender( dt3UNIT_BALL *Uni, dt3ANIM *Ani )
{
  dt3_RndPrimDraw(&Uni->Ball, MatrTranslate(Uni->Pos));
}

static VOID DT3_UnitClose( dt3UNIT *Uni, dt3ANIM *Ani )
{
}

dt3UNIT * DT3_UnitCreateBall( VOID )
{
  dt3UNIT_BALL *Uni;

  if ((Uni = (dt3UNIT_BALL *)DT3_AnimUnitCreate(sizeof(dt3UNIT_BALL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DT3_UnitInit;
  Uni->Response = (VOID *)DT3_UnitResponse;
  Uni->Render = (VOID *)DT3_UnitRender;
  Uni->Close = (VOID *)DT3_UnitClose;
  return (dt3UNIT *)Uni;
}
