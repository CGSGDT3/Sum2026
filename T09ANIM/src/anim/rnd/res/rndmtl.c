/* FILE NAME: rndmtl.c
 * PROGRAMMER: DT3
 * DATE: 17.06.2026
 * PURPOSE: material functional file in animation program.
 */

#include "anim/rnd/rnd.h"

#include "rndres.h"

dt3MATERIAL DT3_RndMaterials[DT3_MAX_MATERIALS]; /* Array of materials */
INT DT3_RndMaterialsSize;                        /* Materials array */

struct
{
  char *Name;
  float amb[3], dif[3], spec[3], shin;
} MatLib[] =
{
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},               32},
  {"Brass",         {0.329412,0.223529,0.027451}, {0.780392,0.568627,0.113725}, {0.992157,0.941176,0.807843}, 27.8974},
  {"Bronze",        {0.2125,0.1275,0.054},       {0.714,0.4284,0.18144},       {0.393548,0.271906,0.166721},  25.6},
  {"Chrome",        {0.25, 0.25, 0.25},          {0.4, 0.4, 0.4},              {0.774597, 0.774597, 0.774597}, 76.8},
  {"Copper",        {0.19125,0.0735,0.0225},     {0.7038,0.27048,0.0828},      {0.256777,0.137622,0.086014},  12.8},
  {"Gold",          {0.24725,0.1995,0.0745},     {0.75164,0.60648,0.22648},    {0.628281,0.555802,0.366065},  51.2},
  {"Peweter",       {0.10588,0.058824,0.113725}, {0.427451,0.470588,0.541176}, {0.3333,0.3333,0.521569},      9.84615},
  {"Silver",        {0.19225,0.19225,0.19225},   {0.50754,0.50754,0.50754},    {0.508273,0.508273,0.508273},  51.2},
  {"Polished Silver", {0.23125,0.23125,0.23125}, {0.2775,0.2775,0.2775},       {0.773911,0.773911,0.773911},  89.6},
  {"Turquoise",     {0.1, 0.18725, 0.1745},      {0.396, 0.74151, 0.69102},    {0.297254, 0.30829, 0.306678}, 12.8},
  {"Ruby",          {0.1745, 0.01175, 0.01175},  {0.61424, 0.04136, 0.04136},  {0.727811, 0.626959, 0.626959}, 76.8},
  {"Polished Gold", {0.24725, 0.2245, 0.0645},   {0.34615, 0.3143, 0.0903},    {0.797357, 0.723991, 0.208006}, 83.2},
  {"Polished Bronze", {0.25, 0.148, 0.06475},    {0.4, 0.2368, 0.1036},        {0.774597, 0.458561, 0.200621}, 76.8},
  {"Polished Copper", {0.2295, 0.08825, 0.0275}, {0.5508, 0.2118, 0.066},      {0.580594, 0.223257, 0.0695701}, 51.2},
  {"Jade",          {0.135, 0.2225, 0.1575},     {0.135, 0.2225, 0.1575},      {0.316228, 0.316228, 0.316228}, 12.8},
  {"Obsidian",      {0.05375, 0.05, 0.06625},    {0.18275, 0.17, 0.22525},     {0.332741, 0.328634, 0.346435}, 38.4},
  {"Pearl",         {0.25, 0.20725, 0.20725},    {1.0, 0.829, 0.829},          {0.296648, 0.296648, 0.296648}, 11.264},
  {"Emerald",       {0.0215, 0.1745, 0.0215},    {0.07568, 0.61424, 0.07568},  {0.633, 0.727811, 0.633},       76.8},
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},                32.0},
  {"Black Rubber",  {0.02, 0.02, 0.02},          {0.01, 0.01, 0.01},           {0.4, 0.4, 0.4},                10.0},
};
#define MAT_N (sizeof(MatLib) / sizeof(MatLib[0]))

/* Default material function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3MATERIAL) default material.
 */
dt3MATERIAL DT3_RndMtlGetDef( VOID )
{
  dt3MATERIAL def_mtl =
  {
    "default",
    {0.1, 0.1, 0.1},
    {0.90, 0.90, 0.90},
    {0.30, 0.30, 0.30},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };

  return def_mtl;
} /* End of 'DT3_RndMtlGetDef' function */

/* Material array initialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndMtlInit( VOID )
{
  INT i;

  dt3MATERIAL def = DT3_RndMtlGetDef();

  DT3_RndMaterialsSize = 0;
  DT3_RndMtlAdd(&def);
  DT3_RndMaterials[0].Tex[0] = DT3_RndTexAddFromFile("bin/textures/rock.bmp");

  for (i = 1; i < 20; i++)
  {
    def.Ka = VecSet(MatLib[i].amb[0], MatLib[i].amb[1], MatLib[i].amb[2]);
    def.Kd = VecSet(MatLib[i].dif[0], MatLib[i].dif[1], MatLib[i].dif[2]);
    def.Ks = VecSet(MatLib[i].spec[0], MatLib[i].spec[1], MatLib[i].spec[2]);
    def.Ph = MatLib[i].shin;
    DT3_RndMtlAdd(&def);
    DT3_RndMaterials[i].Tex[0] = DT3_RndTexAddFromFile("bin/textures/cow.bmp");
  }
} /* End of 'DT3_RndMtlInit' function */

/* Material array deinitialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndMtlClose( VOID )
{
  memset(DT3_RndMaterials, 0, sizeof(dt3MATERIAL) * DT3_RndMaterialsSize);
  DT3_RndMaterialsSize = 0;
} /* End of 'DT3_RndMtlClose' function */

/* Material array add function.
 * ARGUMENTS:
 *   - Pointer to material to be added:
 *       dt3MATERIAL *Mtl;
 * RETUNS:
 *   (INT) number of added material in array.
 */
INT DT3_RndMtlAdd( dt3MATERIAL *Mtl )
{
  if (DT3_RndMaterialsSize >= DT3_MAX_MATERIALS)
    return 0;
  DT3_RndMaterials[DT3_RndMaterialsSize] = *Mtl;
  return DT3_RndMaterialsSize++;
} /* End of 'DT3_RndMtlAdd' function */

/* Using material function.
 * ARGUMENTS:
 *     - Material number in material stock:
 *       INT MtlNo;
 * RETUNS:
 *     (INT) progid from shader.
 */
INT DT3_RndMtlApply( INT MtlNo )
{
  UINT prg;
  dt3MATERIAL *mtl;
  INT loc, i;
  CHAR name[] = "IsTexture0";

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DT3_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DT3_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= (UINT)DT3_RndShadersSize)
    prg = 0;
  prg = DT3_RndShaders[prg].ProgId;

  if (prg == 0)
    return 0;

  glUseProgram(prg);


  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);  
    glBindTexture(GL_TEXTURE_2D, DT3_RndTextures[mtl->Tex[i]].TexId);
    name[9] = '0' + i;
    if ((loc = glGetUniformLocation(prg, name)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
                        
  return prg;
} /* End of 'DT3_RndMtlApply' function */

/* END OF 'rndmtl.c' FILE */

