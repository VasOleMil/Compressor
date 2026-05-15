#ifndef CEMNT_H888
#define CEMNT_H888

#include"CList.h"
#include"CSort.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
typedef struct CEmnt
{
	double *X;  //Coordinares current
    double *V;  //Speeds current
    CSort  *S;  //Sort
	CDItem *v;  //List item link

} 	CEmnt;
//--------------------------------------------------------------------
extern CDList  *Ev;//Elements list container
extern CDItem  *Ex;//list  service  register
extern CDItem  *Et;//list  service  register
extern CEmnt   *ei;//saved element for collision handling
extern CEmnt   *ej;//saved element for collision handling
extern CEmnt   *Ei;//object service register
extern CEmnt   *Ej;//object service register
//--------------------------------------------------------------------
extern void EmntInit(void);//Inits start values in Emnt container:  Ev
//--------------------------------------------------------------------
extern void EmntFree(void);//Releases Free resources  Ev
//--------------------------------------------------------------------
extern void EmntSize(void);//Si sort, align number to Si->Bn
//--------------------------------------------------------------------
inline void EmntMove(void);//Moves all elements
//--------------------------------------------------------------------
extern void EmntColl(void);//ei, ej  interaction
//--------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif//CEMNT_H888