#include"CBase.h"
//--------------------------------------------------------------------
long             Rn;//Space dimension
long             Bn;//Number of elements
double           Rb;//Bound radius
double           Vb;//Bound volume
double           Vg;//Vgamma(Rn)

long             Gm;//Toggles mass change while sizing
double           Gc;//Sizing speed
double           GR;//GR = (1.0 + Gc * Te)
double           GM;//GM = GR^Rn

double           kT;//Initial element doubled energy per dimension
     
double           dT;//Current eventual time step
double           Te;//Eventual time


long             Sn;//Max simultanious seria
long             Sc;//Numer simultanious events, dT = 0.0
double           De;//range (-De;+De) treated as dT = 0.0
double           Ds;//range (-Ds;+Ds) treated as rv - RV = 0.0
//--------------------------------------------------------------------
long 		     UT;// 1, unit for memory allocation.
long 		     UD;// 1, dedicated destructor, 0 - no, 1 - yes
long 		     UF;// 0, dedicated destructor, 0 - no, 1 - yes
double           RN;//sqrt(Rn)
long             Ce;//Number of e-e interactions 
long             Cb;//Number of b-e interactions
long             Cx;//Interactions type
//--------------------------------------------------------------------

