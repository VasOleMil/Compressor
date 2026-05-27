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
long             Tn;//Max initial random placing seria

double           De;//range (-De;+De) treated as zero, dT = 0.0
double           Ds;//range (-Ds;+Ds) treated as  rv - RV = 0.0
long 		     Fm;// float mantisse  1.0 + 10^-Fm == 1.0
long 		     Fe;// float exponent  0.0 + 10^-Fe == 0.0 

double           Qe;//system energy, Bn * Rn * kT / 2
double           Qg;//geomenty deviation, mean squared

long             Ce;// Number of e-e interactions 
long             Cb;// Number of b-e interactions
long             Cx;// interactions type
//--------------------------------------------------------------------
double           RN;// sqrt(Rn)
long             LN;// Rn * sizeof(double), coordinates or speeds size
double           Vb;// Bound volume
double           Ve;// Elements volume, based on Rc
double           Me;// Elements mass, based on Mc
double           Vg;// Vgamma(Rn)
double           GG;// GG = Gc * Gc
double           GR;// GR = (1.0 + Gc * Te)
double           GM;// GM = GR^Rn
//--------------------------------------------------------------------
//long             Gv;//Toggles position verifier
//long             Gm;//Toggles mass change while sizing
//long             Gr;//Toggles element rejection by center
//--------------------------------------------------------------------

