#ifndef CHOLD_H888
#define CHOLD_H888

#include"CList.h"
#include"CEmnt.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
typedef struct CHold
{
	CEmnt  *Ei;
	CEmnt  *Ej;
	double  dt; //Time to interaction, tti
	double  dT; //saved dT for collision handling
	double *Xi; //saved Xi for collision handling
	double *Xj; //saved Xj for collision handling
} 	CHold;
//--------------------------------------------------------------------
extern CDList   *Tv;//Sorts   list  container 
extern CDItem   *Tx;//list  service  register
extern CDItem   *Tt;//list  service  register
extern CHold    *Ti;//object service register

//--------------------------------------------------------------------
extern void TimeInit(void);//Inits start values in Time container: Tv
//--------------------------------------------------------------------
extern void TimeFree(void);//Releases resources  Tv
//--------------------------------------------------------------------
extern void	TimeGetStp(void);//Gets current time step,		  sets: Tm
//--------------------------------------------------------------------
extern void TimeDecStp(void);//Decreases tti in Tv, using Tm
//--------------------------------------------------------------------
extern void TimeDelStp(void);//Delete ei, ej tti in Tv
//--------------------------------------------------------------------
extern void TimeCalcTT(void);//Calculate ei, ej elements tti
//--------------------------------------------------------------------


#ifdef __cplusplus
}
#endif
#endif//CHOLD_H888