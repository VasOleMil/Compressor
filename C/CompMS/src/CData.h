#ifndef DATA_H888
#define DATA_H888

#include<stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern long 		    UT;// 1, unit memory allocation and UD   start
extern long 		    UD;// 2, dedicated destructor,      UD  finish
extern long 		    UF;// 0, memory allocation for regular free
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
#endif//DATA_H888