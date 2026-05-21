#include"CBase.h"
#include"CSort.h"
//--------------------------------------------------------------------
CDList  *Sv;//Sorts  lists  container 
    
CDItem  *Sx;//list  service  register
CDItem  *St;//list  service  register
    
CSort   *Si;//object service register
CSort   *Sj;//object service register
//--------------------------------------------------------------------
//  No local registers, see Base & Data for reference
//--------------------------------------------------------------------
static void 
SortItemNew(void)
{
    Lx = (CDItem*) Malloc(sizeof(CDItem), UT, UT); //create ListItem
    Si = (CSort*)  Malloc(sizeof(CSort),  UT, UD); //create Object  
    //Loop Lx ListItem and connect to Si Object 
    Lx->p = Lx; //Lv = Sv; //restore context
    Lx->n = Lx;            //loop item
    Lx->v = Si;            //connect object
}//ListNew item provider for Sort                               Si, Lx
//--------------------------------------------------------------------
static void 
SortItemDel(void)
{
    Si = Lx->v; free(Si); free(Lx);
}//ListDel item provider for Sort                                   Si
//--------------------------------------------------------------------
void 
SortInit(void)
{
    if (Sv != NULL) return;

    Sv = (CDList*) Malloc(sizeof(CDList), UT, UD);
	//Set Sorts containers counters and providers
	Sv->Fn = 0; Sv->ListItemDel = SortItemDel;
	Sv->Vn = 0; Sv->ListItemNew = SortItemNew; 
}//Inits start values in Sorts container.                           Sv
//--------------------------------------------------------------------
void 
SortFree(void)
{
    if (Sv == NULL) return;
    Lv = Sv; ListFree(); 
    free(Sv); Sv = NULL;
}//Releases Free resources                                  Lx, Si, Sv
//--------------------------------------------------------------------
static void
SortTesT(long Id, long Bn, double Rc, double Mc)
{
    if (Rc <= 0.00 || Bn <= 0)  
    { 
        Si = (CSort*)-1; 
    }
    
    Si = NULL; //No error, fail safe
}//Test sort parameters. Validator connector                        Si
//--------------------------------------------------------------------
static void
SortIniT(long Id, long Bn, double Rc, double Mc)
{     
    Si->id = Id;   
    Si->Bn = Bn;
    Si->Rc = Rc;
    Si->Vc = Rc * Gc;
    Si->Rt = Rc * GR; // GR = (1.0 + Gc * Te)

    Mi = Vg * Rc; for (k = 1; k < Rn; k++) Mi *= Rc; 

    Si->Mc = (Mc > 0.0)? Mc : Mi; Si->Mt = Si->Mc * GM; // GM = GR^Rn		        
}//Init sort object, uses: Si, Gm(N) -> GM = 1.0                Mi, Si
//--------------------------------------------------------------------
void 
SortGet(long Id)
{
    if(Sv->Vn == 0) { Sx = NULL; return; }
    //Search in values container
    Sx = St = Sv->Vc; //Valid Sx is set by if 
    do 
    {
        if ((Si = St->v)->id == Id) { Sx = St; return; }
    } 
    while ((St = St->n) != Sx);  

    Sx = NULL; //check existence by Sx
}//Get by id (color) Sort item pointer and object.              St, Sx
//--------------------------------------------------------------------
void 
SortAdd(long Id, long Bn, double Rc, double Mc)
{   
    SortGet(Id);      if(Sx != NULL) { return; }
    SortTesT(Id, Bn, Rc, Mc); if(Si) { return; }
    Lv = Sv; ListAdd();  Sx = Lx; // Add new List item,  Lx == Sv->Vc
    Si = Sx->v; //Localize new object  
    SortIniT(Id, Bn, Rc, Mc);  //Set values
}//Add new element to Sorts selector container. Lx, Vc, Sx, St, Si, Mi
//--------------------------------------------------------------------
void 
SortSet(long Id, long Bn, double Rc, double Mc)
{   
    SortGet(Id);      if(Sx == NULL) { return; }
    SortTesT(Id, Bn, Rc, Mc); if(Si) { return; }
    Si = Sx->v; //Localize new object
    SortIniT(Id, Bn, Rc, Mc);  //Set values
}//Set values Sort object.                              Sx, St, Si, Mi
//--------------------------------------------------------------------
void 
SortDel(long Id)
{   
    SortGet(Id); if(Sx == NULL) { return; }
    Lv = Sv; Sv->Vc = Sx; ListDel(); 
}//Moves selected Sort item to Free container                   Lx, Sx
//--------------------------------------------------------------------
void 
SortGrow(void)
{
	Sx = St = Sv->Vc; Te += dT; GR = (1.0 + Gc * Te); 
    GM = GR; for (k = 1; k < Rn; k++) GM *= GR;
    do
    {      
        Si = St->v; 
        Si->Rt = (Si->Rc) * GR;     //change size
		//if (Gm)
        //Si->Mt = (Si->Mc) * GM;   //change mass
    }
    while((St = St->n) != Sx);
}//Sizes Sv->Vc Sorts and mass                  Te, Sx, St, Si, GR, GM
//--------------------------------------------------------------------
