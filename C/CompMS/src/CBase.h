#ifndef CBASE_H888
#define CBASE_H888

#ifndef CCOMP_H888
#include"CData.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern long             Rn;//Space dimension
extern long             Bn;//Number of elements
extern double           Rb;//Bound radius

extern double           Gc;//Sizing speed, Rt = Rc*(1.0 + Gc * Te)

extern double           kT;//Element doubled energy per dimension
     
extern double           dT;//Current eventual time step, minimal tti
extern double           Te;//Eventual time

extern long             Sn;//Max simultanious seria
extern long             Sc;//Number of simultanious events,   dT = 0.0
extern long             Tn;//Max initial random placing seria
extern long             An;//threeshold steps for filters, 400

extern double           De;//range (-De;+De) treated as zero, dT = 0.0
extern double           Ds;//range (-Ds;+Ds) treated as  rv - RV = 0.0
extern double           dA;//linear dt range dA = 2 * sqrt(2 * 10^-Fm)
extern double           Ta;// mean free time
extern double           Ls;// Ls += Ar * (Lc - Ls); mean squared path
extern double           Ar;// Ar = 1.0 / 64.0; 64 steps for 15% accuracy
extern long 		    Fm;// float mantisse  1.0 + 10^-Fm == 1.0
extern long 		    Fe;// float exponent  0.0 + 10^-Fe == 0.0 
extern long 		    Fr;// RAND_MAX bits 

extern long             Cx;// Interactions type
extern long             Ce;// Number of e-e interactions 
extern long             Cb;// Number of b-e interactions

extern double           Qe;//system energy, Bn * Rn * kT / 2 
extern double           Qg;//geomenty deviation, mean squared
//--------------------------------------------------------------------
extern double           RN;// sqrt(Rn)
extern long             LN;// Rn * sizeof(double),coordinates & speeds
extern double           Vb;// Bound volume
extern double           Ve;// Elements volume, based on Rc
extern double           Me;// Elements mass, based on Mc
extern double           Vg;// Vgamma(Rn)
extern double           GG;// GG = Gc * Gc
extern double           GR;// GR = (1.0 + Gc * Te)
extern double           GM;// GM = GR^Rn
//--------------------------------------------------------------------
//extern long             Gv;//(Y) Toggles position verifier
//extern long             Gm;//(N) Toggles mass change while sizing
//extern long             Gr;//(N) Toggles element rejection by center
//========================================================== Data.h ==
//extern long 		UT;// 1, unit memory allocation and UD   start
//extern long 		UD;// 2, dedicated destructor,      UD  finish
//extern long 		UF;// 0, memory allocation for regular free
//--------------------------------------------------------------------
//extern void*      Malloc(size_t LN,long CN,long UD);//malloc wrapper
//---------------------------------------------------------- List.h --
//extern CDItem     *Lv;   //CList.h
//extern CDItem     *Lx;
//---------------------------------------------------------- Sort.h --
//extern CDList     *Sv;
//extern CDItem     *Sx;
//extern CDItem     *St;   //CSort.h
//extern CSort      *Si;
//extern CSort      *Sj;
//---------------------------------------------------------- Emnt.h --
//extern CDList     *Ev;
//extern CDItem     *Ex;
//extern CDItem     *Et;
//extern CDItem     *Es;   //CEmnt.h 
//extern CDItem     *ei;
//extern CDItem     *ej;
//extern CEmnt      *Ei;
//extern CEmnt      *Ej;
//---------------------------------------------------------- Hold.h --
//extern CDList     *Tv;
//extern CDItem     *Tx;
//extern CDItem     *Tt;   //CHold.h 
//extern CDItem     *Tm;
//extern CHold      *Ti;
//---------------------------------------------------------- Mode.h --
//extern CDList     *Mv;
//extern double     *Xc;    //CMode.h
//extern double     *Pc;
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CBASE_H888
