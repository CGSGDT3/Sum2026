/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DT3UNIT *Uni;
 *   - animation context:
 *       DT3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitInit( DT3UNIT *Uni, DT3ANIM *Ani )
{
} /* End of 'DT3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DT3UNIT *Uni;
 *   - animation context:
 *       DT3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitClose( DT3UNIT *Uni, DT3ANIM *Ani )
{
} /* End of 'DT3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DT3UNIT *Uni;
 *   - animation context:
 *       DT3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitResponse( DT3UNIT *Uni, DT3ANIM *Ani );
{
} /* End of 'DT3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DT3UNIT *Uni;
 *   - animation context:
 *       DT3ANIM *Ani;
 * RETURNS: None.
 */
static VOID DT3_UnitRender( DT3UNIT *Uni, DT3ANIM *Ani )
{
} /* End of 'DT3_UnitRender' function */

-- функция создания объекта:

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (DT3UNIT *) pointer to created unit.
 */
DT3UNIT * DT3_AnimUnitCreate( INT Size )
{
  DT3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(DT3UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = DT3_UnitInit;
  Uni->Close = DT3_UnitClose;
  Uni->Response = DT3_UnitResponse;
  Uni->Render = DT3_UnitRender;

  return Uni;
} /* End of 'DT3_AnimUnitCreate' function */