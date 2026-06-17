/* FILE NAME: rndres.h
 * PROGRAMMER: DT3
 * DATE: 13.06.2026
 * PURPOSE: resource header file in animation program.
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

/***
 * Resources functions
***/

/* Resource initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndResInit( VOID );

/* Resource deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndResClose( VOID );

/***
 * Shaders handle
***/

/* Shader constants */
#define DT3_MAX_SHADERS 20
#define DT3_STR_MAX 1000

/* Shader program store type */
typedef struct tagdt3SHADER
{
  CHAR Name[DT3_STR_MAX]; /* Shader filename prefix */
  UINT ProgId;            /* Shader program Id */
} dt3SHADER;

extern dt3SHADER DT3_RndShaders[DT3_MAX_SHADERS];
extern INT DT3_RndShadersSize;

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load:
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT DT3_RndShdAdd( CHAR *ShaderFileNamePrefix );

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndShdUpdate( VOID );

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndShdInit( VOID );

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DT3_RndShdClose( VOID );

/***
 * Texture handle
***/      

/* Textures stock */
#define DT3_MAX_TEXTURES 3000

/* Texture representation type */
typedef struct tagdt3TEXTURE
{
  CHAR Name[DT3_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} dt3TEXTURE;

extern dt3TEXTURE DT3_RndTextures[DT3_MAX_TEXTURES]; /* Array of textures */
extern INT DT3_RndTexturesSize;                      /* Textures array store size */

/* Texture array initialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndTexInit( VOID );

/* Texture array deinitialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndTexClose( VOID );

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
INT DT3_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

/* Add texture from file to stock function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) texture stock number (-1 if error is occured).
 */
INT DT3_RndTexAddFromFile( CHAR *FileName );

/***
 * Material handle
***/

/* Material store type */
typedef struct tagdt3MATERIAL
{
  CHAR Name[DT3_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */

  FLT Trans;                /* Transparency factor */

  INT Tex[8];               /* Texture references from texture table (or -1) */
                  
  INT ShdNo;                /* Shader number in shader table */
} dt3MATERIAL;

/* Material stock */
#define DT3_MAX_MATERIALS 300

extern dt3MATERIAL DT3_RndMaterials[DT3_MAX_MATERIALS]; /* Array of materials */
extern INT DT3_RndMaterialsSize;                        /* Materials array */

/* Material array add function.
 * ARGUMENTS:
 *   - Pointer to material to be added:
 *       dt3MATERIAL *Mtl;
 * RETUNS:
 *   (INT) number of added material in array.
 */
INT DT3_RndMtlAdd( dt3MATERIAL *Mtl );

/* Default material function.
 * ARGUMENTS:
 *   None.
 * RETURNS:
 *   (dt3MATERIAL) default material.
 */
dt3MATERIAL DT3_RndMtlGetDef( VOID );

/* Material array initialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndMtlInit( VOID );

/* Material array deinitialization function.
 * ARGUMENTS:
 *   None.
 * RETUNS:
 *   None.
 */
VOID DT3_RndMtlClose( VOID );

/* Using material function.
 * ARGUMENTS:
 *     - Material number in material stock:
 *       INT MtlNo;
 * RETUNS:
 *     (INT) progid from shader.
 */
INT DT3_RndMtlApply( INT MtlNo );

#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */