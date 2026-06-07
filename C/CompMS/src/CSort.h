#ifndef CSORT_H888
#define CSORT_H888

#include"CList.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
typedef struct CSort
{
	long    id;//Elements id
    long    Bn;//Number of elements

    double  Rc;//Radius
    double  Mc;//Mass
	double  Vc;//Gc * Rc, used Gr = N, reject by element surface
    double  Vs;//Vc * Vc, used Gr = N, reject by element surface
    double  Rt;//Current elements radius, Rc * (1.0 + Gc * Te)
    double  Mt;//Current elements mass,   Mc * (1.0 + Gc * Te)^Rn

} 	CSort;
//--------------------------------------------------------------------
extern CDList   *Sv;//Sorts   list  container 
extern CDItem   *Sx;//list  service  register
extern CDItem   *St;//list  service  register
extern CSort    *Si;//object service register
extern CSort    *Sj;//object service register
//--------------------------------------------------------------------
extern void SortInit(void);//Inits start values in Sorts container: Sv
//--------------------------------------------------------------------
extern void SortFree(void);//Releases Free resources  Sv
//--------------------------------------------------------------------
extern void SortLoad(void);///Init sort object values, uses: Si 
//--------------------------------------------------------------------
extern void SortAdd(long id, long Bn, double Rc, double Mc);// Sx,Si
//--------------------------------------------------------------------
extern void SortSet(long id, long Bn, double Rc, double Mc);// Sx,Si
//--------------------------------------------------------------------
extern void SortGet(long id);//Search in Sv->Vc, sets: Sx
//--------------------------------------------------------------------
extern void SortDel(long id);//Moves selected item to Free container 
//--------------------------------------------------------------------
extern void SortGrow(void);//Sizes Sv Sorts radii Rt, mass Mt
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CSORT_H888