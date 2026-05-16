#ifndef CBASE_H888
#define CBASE_H888

#include<math.h>

#include"CData.h"

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
extern double           De;//range (-De;+De) treated as zero, dT = 0.0
extern double           Ds;//range (-Ds;+Ds) treated as  rv - RV = 0.0

extern long             Cx;// Interactions type
extern long             Ce;// Number of e-e interactions 
extern long             Cb;// Number of b-e interactions
//--------------------------------------------------------------------
extern double           RN;// sqrt(Rn)
extern long             LN;// Rn * sizeof(double),coordinates & speeds
extern double           Vb;// Bound volume
extern double           Vg;// Vgamma(Rn)
extern double           GG;// GG = Gc * Gc
extern double           GR;// GR = (1.0 + Gc * Te)
extern double           GM;// GM = GR^Rn, for Gm = Y
//--------------------------------------------------------------------
extern double *Xi,*Xj,*Vi,*Vj;

extern double  RA, RR, RV, VV, rr, rv, vv, rk, vk, Ri, Rj, Mi, Mj;
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
//========================================================== List.h ==
//extern CDItem     *Lv;//CList.h
//extern CDItem     *Lx;
//========================================================== Sort.h ==
//extern CDList     *Sv;
//extern CDItem     *Sx;
//extern CDItem     *St;//Sort.h
//extern CSort      *Si;
//extern CSort      *Sj;
//========================================================== Emnt.h ==
//extern CDList     *Ev;
//extern CDItem     *Ex;
//extern CDItem     *ei;//CEmnt.h
//extern CDItem     *ej;
//extern CEmnt      *Ei;
//extern CEmnt      *Ej;
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CBASE_H888