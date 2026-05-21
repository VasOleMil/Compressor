#ifndef CCOMP_H888
#define CCOMP_H888

#include"CBase.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern void CompInit(long Rn);//resets containers and data, if changed
//--------------------------------------------------------------------
extern void SortAdd(long id, long Bn, double Rc, double Mc);// 
//--------------------------------------------------------------------
extern void SortSet(long id, long Bn, double Rc, double Mc);// 
//--------------------------------------------------------------------
extern void SortDel(long id);//
//--------------------------------------------------------------------
extern void CompLoaD(double GC);//init stepping for sizing speed
//--------------------------------------------------------------------
extern void CompLoad(double KT, double KS, double GC);//init stepping
//--------------------------------------------------------------------
extern void CompStep(void);//main loop, is called until Sc >= Sn
//--------------------------------------------------------------------
extern void CompFree(void);//Destruction, releases resources
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CCOMP_H888