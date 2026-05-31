#ifndef CMODE_H888
#define CMODE_H888

#include"CList.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// no structures defined yet
//--------------------------------------------------------------------
extern CDList *Mv;//Modes resources container, only starting indicator
extern double *Xc;//Center of mass vector
extern double *Pc;//Summary impulse vector
//--------------------------------------------------------------------
extern void ModeInit(void); //CData.c
//--------------------------------------------------------------------
extern void ModeFree(void); //CData.c
//--------------------------------------------------------------------
extern double Vgamma(void);//Multiplier Vb = Vg * Rb^Rn, Vg=Vgamma(Rn)
//--------------------------------------------------------------------
extern void SetPBound(void);//Size for even interaction/move probality
//--------------------------------------------------------------------
extern void SetVolume(void);//Summary elements volume Ve & Me, uses Rc
//--------------------------------------------------------------------
extern void SetRanges(void);//Zero drift ranges, for reporting only 
//--------------------------------------------------------------------
extern void EngPhases(void);//Engage phase space, random values {X,V}
//--------------------------------------------------------------------
extern void NormMassCenter(void);//Try shift Xc to the center of bound
//--------------------------------------------------------------------
extern void NormImpulse(void);// supress system drift
//--------------------------------------------------------------------
extern void NormMomenta(void);// supress system rotation
//--------------------------------------------------------------------
extern void NormEnergy(void);//Normalize energy to given kT, saves Qe 
//--------------------------------------------------------------------
extern void TestMassCenter(void);//get current mass center vector *Xc
//--------------------------------------------------------------------
extern void TestImpulse(void);// get current summary impulse  *Pc
//--------------------------------------------------------------------
extern void TestEnergy(void);// get system energy value Qe 
//--------------------------------------------------------------------
extern void TestGeometry(void);// average geometry deviation,  Sc, Qg
//--------------------------------------------------------------------
// debug
//--------------------------------------------------------------------
extern void GetVolume(void);//output coordinates to consol
//--------------------------------------------------------------------
extern int	TestStruct(CDList* LV);//List test
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CMODE_H888
