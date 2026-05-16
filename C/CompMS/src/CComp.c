#include"CBase.h"
#include"CList.h"
#include"CSort.h"
#include"CEmnt.h"
#include"CHold.h"
#include"CData.h"
#include"CComp.h"
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void
CompInit()
{
    DataInit();
}
//--------------------------------------------------------------------
void
CompFree()
{
    DataFree();
}
//--------------------------------------------------------------------
void
CompStep()
{
	if (Tv->Vn == 0) return; //safety, no events to process
    
    //Select minimal tti  from holder Tv
	TimeGetStp();// Tm, ei, ej, dT are set
    
    //mean free time and path tests, before interaction
    
    //Change geometry and variables, if not simultanious 
    if (dT > 0.0) //Te += dT in SortGrow
    { Sc = 0; EmntMove(); SortGrow(); }
    else //check impulse loop, stop processing
    { Sc++  ; if (Sc >= Sn) return;   }

    //if Gv(Y), verify position, to prevent time summing errors 
	TimeValStp(); //initiate counters Cx, Ce, Cb
    
    //Elements ei or ej interaction, 
    EmntColl(); //procced to new time calculation

	//mean free time and path tests, after interaction
    
    //Clear interacted elements tti in Tv
    TimeDelStp();//Tm in free container
    
    //Change tti in Tv, Tm not touched
    if (Sc == 0) TimeDecStp();
    
    //Calculate ei, ej elements tti
    TimeCalcTT();// ei, ej, dT are safe
    //Tm is overwritted and reused
}//main loop, is called until Sc >= Sn, or x or t span is covered
//--------------------------------------------------------------------


//--------------------------------------------------------------------
static double
Vgamma(long Dim)
{
    double Pk, Pi; long i, k, d, Rh;

    k = 1; d = 2; Pk = 1.0; Pi = 3.141592653589793;

    if (Dim % 2 == 0)
    {
        for (i = 1, Rh = (Dim - 0) / 2; i <= Rh; i++)
        {
            Pk *= Pi; k *= i;
        }

        return Pk / k;
    }
    else
    {
        for (i = 1, Rh = (Dim - 1) / 2; i <= Rh; i++)
        {
            Pk *= Pi;  d *= 2; k *= 2 * i + 1;
        }

        return Pk * d / k;
    }
}
//--------------------------------------------------------------------
    