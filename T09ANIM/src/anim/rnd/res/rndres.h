/* FILE NAME: rndres.h
 * PROGRAMMER: DT3
 * DATE: 13.06.2026
 * PURPOSE: resource header file in animation program.
 */

#ifndef __rndres_h_
#define __rndres_h

#include "def.h"

/* Shader constants */
#define DT3_MAX_SHADERS 20
#define DT3_STR_MAX 1000

/* Shader program store type */
typedef struct tagdt3SHADER
{
  CHAR Name[DT3_STR_MAX]; /* Shader filename prefix */
  UINT ProgId;            /* Shader program Id */
} dt3SHADER;

dt3SHADER DT3_RndShaders[DT3_MAX_SHADERS];

INT DT3_RndShadersSize;

/* Save log to file function.
 * ARGUMENTS:
 *   - shader prefix:
 *       CHAR *FileNamePrefix;
 *   - shader name:
 *       CHAR *ShaderName;
 *   - error text:
 *       CHAR *Text;
 * RETURNS: None.
 */
static VOID DT3_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text );

/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
static CHAR * DT3_RndLoadTextFromFile( CHAR *FileName );

/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (UINT) load shader program Id.
 */
static UINT DT3_RndShdLoad( CHAR *FileNamePrefix );

/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT ProgId;
 * RETUNS: None.
 */
static VOID DT3_RndShdFree( UINT ProgId );

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

#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */