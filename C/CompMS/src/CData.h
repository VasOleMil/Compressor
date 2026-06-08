#ifndef CDATA_H888
#define CDATA_H888

#ifdef  _MSC_VER
#define _CRT_SECURE_DEPRECATE_MEMORY
#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#else
#include <string.h>
#endif

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<math.h>

#include"CList.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
//				UT == 0, thread start indicator value, RTL default
//--------------------------------------------------------------------
extern long 	UT;// 1, unit memory allocation and UD   start
extern long 	UD;// 2, dedicated destructor,      UD  finish
extern long 	UF;// 0, memory allocation for regular free

extern double	Pi;// Pi number 3.1415...
extern double   rd;// 2.0 / RAND_MAX, for random double
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern CDList  *Dv;//Data list items container 
extern long		k ;//index, used in dimension 
extern double  *Xi,*Xj,*xe,*Vi,*Vj,*ve; //global registers 
extern double	RA, RR, RV, VV, rr, rv, vv, rk, vk, Ri, Rj, Mi, Mj, me;
//--------------------------------------------------------------------
extern void*    Malloc(size_t LN,long CN,long UD);   // malloc wrapper
//--------------------------------------------------------------------
extern void		DataInit(void); //Construction
//--------------------------------------------------------------------
extern void     DataSize(void); //Releases free data  resources
//--------------------------------------------------------------------
extern void		DataFree(void); //Destruction
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CDATA_H888