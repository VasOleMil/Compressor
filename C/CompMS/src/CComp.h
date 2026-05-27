#ifndef CCOMP_H888
#define CCOMP_H888

#include"CBase.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern void CompInit(long Rn);//resets containers and data, if changed
//--------------------------------------------------------------------
extern void SortAdd(long id, long Bn, double Rc, double Mc);// id > 0
//--------------------------------------------------------------- + --
extern void SortSet(long id, long Bn, double Rc, double Mc);// Bn > 0
//--------------------------------------------------------------------
extern void SortDel(long id); //sort manipulators before stepping
//--------------------------------------------------------------------
extern void CompLoad(double KT, double KS, double GC); //init stepping
//--------------------------------------------------------------------
extern void CompTemp(double GC);//init stepping for sizing speed
//--------------------------------------------------------------------
extern void CompStep(void);//main loop, is called until Sc >= Sn
//--------------------------------------------------------------------
extern void CompFree(void);//Destruction, releases resources
//--------------------------------------------------------------------

extern void GetVolume(void);//output coordinates to console
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CCOMP_H888