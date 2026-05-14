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
	CEmnt	*i;
	CEmnt	*j;
	double  dt;
	double  dT; //saved dT for collision handling
	double* Xi; //saved Xi for collision handling
	double* Xj; //saved Xj for collision handling
} 	CHold;
//--------------------------------------------------------------------
extern CSList   *Tv;//Sorts   list  container 
extern CLItem   *Tx;//list  service  register
extern CLItem   *Tt;//list  service  register
extern CHold    *Ti;//object service register

//--------------------------------------------------------------------
extern void TimeInit(void);//Inits start values in Time container: Tv
//--------------------------------------------------------------------
extern void TimeFree(void);//Releases Free resources  Sv
//--------------------------------------------------------------------
//extern void TimeAdd(void);
//--------------------------------------------------------------------


#ifdef __cplusplus
}
#endif
#endif//CHOLD_H888