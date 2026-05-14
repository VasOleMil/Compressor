#ifndef COMP_H888
#define COMP_H888

// For Lib header 
// move includes in CComp.c
#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern void CompInit(void);
//--------------------------------------------------------------------

//--------------------------------------------------------------------
extern void CompFree(void);
//--------------------------------------------------------------------


#ifdef __cplusplus
}
#endif
#endif//COMP_H888