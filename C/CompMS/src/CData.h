#ifndef CDATA_H888
#define CDATA_H888

#include<stdlib.h>
#include<math.h>

#ifdef  _MSC_VER
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>
#else
#include <string.h>
#endif

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
extern double	s ;// s = 1.0 / sqrt(2.0)
//--------------------------------------------------------------------
extern double  *Xi,*Xj,*Vi,*Vj; //global registers 
extern double	RA, RR, RV, VV, rr, rv, vv, rk, vk, Ri, Rj, Mi, Mj;
//--------------------------------------------------------------------
extern void*    Malloc(size_t LN,long CN,long UD);   // malloc wrapper
//--------------------------------------------------------------------
extern void		DataInit(void); //Construction
//--------------------------------------------------------------------
extern void		DataFree(void); //Destruction
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CDATA_H888