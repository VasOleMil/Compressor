#ifndef COMP_H888
#define COMP_H888

#include"CBase.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern void CompInit(void);//Construction, inits containers
//--------------------------------------------------------------------
extern void CompStep(void);//main loop, is called until Sc >= Sn
//--------------------------------------------------------------------
extern void CompFree(void);//Destruction, releases resources
//--------------------------------------------------------------------


#ifdef __cplusplus
}
#endif
#endif//COMP_H888