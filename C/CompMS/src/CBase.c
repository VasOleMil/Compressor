#include"CBase.h"
//--------------------------------------------------------------------
long             Rn;//Space dimension
long             Bn;//Number of elements
double           Rb;//Bound radius

double           Gc;//Sizing speed, Rt = Rc*(1.0 + Gc * Te)

double           kT;//Initial element doubled energy per dimension
     
double           dT;//Current eventual time step
double           Te;//Eventual time

long             Sn;//Max simultanious seria
long             Sc;//Number of simultanious events,   dT = 0.0
double           De;//range (-De;+De) treated as zero, dT = 0.0
double           Ds;//range (-Ds;+Ds) treated as  rv - RV = 0.0

long             Ce;// Number of e-e interactions 
long             Cb;// Number of b-e interactions
long             Cx;// interactions type
//--------------------------------------------------------------------
double           RN;// sqrt(Rn)
long             LN;// Rn * sizeof(double), coordinates or speeds size
double           Vb;// Bound volume
double           Vg;// Vgamma(Rn)
double           GR;// GR = (1.0 + Gc * Te)
double           GM;// GM = GR^Rn
//--------------------------------------------------------------------
//long             Gv;//Toggles position verifier
//long             Gm;//Toggles mass change while sizing
//long             Gr;//Toggles element rejection by center
//--------------------------------------------------------------------

