 /* FILE NAME: globe.h
 * PROGRAMMER: DT3
 * DATE: 06.06.2026
 * PURPOSE: Header file in drawing globe program.
 */
#ifndef __GLOBE_H_
#define __GLOBE_H_

#define DBL DOUBLE

#include <windows.h>
#include "MTH.H"

VOID GLB_Init( DBL R );
VOID GLB_Draw( HDC hDC );
VOID GLB_Resize( INT W, INT H );
VEC RotateX( VEC P, DBL Angle );
VEC RotateY( VEC P, DBL Angle );
VEC RotateZ( VEC P, DBL Angle );
#endif /*__GLOBE_H_*/
/* End of 'globe.h file */