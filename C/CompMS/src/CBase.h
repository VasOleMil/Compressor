#ifndef CBASE_H888
#define CBASE_H888

#include<stdlib.h>
#include<math.h>

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
extern long             Rn;//Space dimension
extern long             Bn;//Number of elements
extern double           Rb;//Bound radius
extern double           Vb;//Bound volume
extern double           Vg;//Vgamma(Rn)

extern long             Gm;//Toggles mass change while sizing
extern double           Gc;//Sizing speed
extern double           GR;//GR = (1.0 + Gc * Te)
extern double           GM;//GM = GR^Rn

extern double           kT;//Element doubled energy per dimension
     
extern double           dT;//Current eventual time step
extern double           Te;//Eventual time

extern long             Sn;//Max simultanious seria
extern long             Sc;//Numer simultanious events, dT = 0.0
extern double           De;//range (-De;+De) treated as dT = 0.0
extern double           Ds;//range (-Ds;+Ds) treated as rv - RV = 0.0
//--------------------------------------------------------------------
extern long 		    UT;// 1, unit memory allocation and UD   start
extern long 		    UD;// 2, dedicated destructor,      UD  finish
extern long 		    UF;// 0, memory allocation for regular free
extern double           RN;// sqrt(Rn)
extern long             Ce;// Number of e-e interactions 
extern long             Cb;// Number of b-e interactions
extern long             Cx;// Interactions type
//--------------------------------------------------------------------
extern void*        Malloc(size_t LN,long CN,long UD);//malloc wrapper
//--------------------------------------------------------------------
//extern CLItem     *Lv;//List.h
//extern CLItem     *Lx;
//--------------------------------------------------------------------
//extern CSList     *Sv;//Sort.h
//extern CLItem     *Sx;
//extern CLItem     *St;
//extern CSort      *Si;
//extern CSort      *Sj;
//--------------------------------------------------------------------
//extern CSList     *Ev;//CEmnt.h
//extern CLItem     *Ex;
//extern CLItem     *Em;
//extern CLItem     *Et;
//extern CEmnt      *Ei;
//extern CEmnt      *Ej;
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CBASE_H888