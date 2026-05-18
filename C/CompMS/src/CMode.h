#ifndef CMODE_H888
#define CMODE_H888

#include"CList.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// no structures defined yet
//--------------------------------------------------------------------
CDList	*Mv;//Modes resources container, only starting indicator
//--------------------------------------------------------------------
extern void ModeInit(void);
//--------------------------------------------------------------------
extern void ModeFree(void);
//--------------------------------------------------------------------
extern double Vgamma(void);//Multiplier Vb = Vg * Rb^Rn, Vg=Vgamma(Rn)
//--------------------------------------------------------------------
extern void SetGBound(void);//Size for even interaction/move probality
//--------------------------------------------------------------------
extern void SetVolume(void);//Summary elements volume Ve based on Rc
//--------------------------------------------------------------------
extern void SetRanges(void);//Zero drift ranges, for reporting only 
//--------------------------------------------------------------------
extern void EngPhases(void);//Engage phase space, random values {X,V}
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CMODE_H888
