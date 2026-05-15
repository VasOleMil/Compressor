#include"CBase.h"
#include"CSort.h"
//--------------------------------------------------------------------
CDList  *Sv;//Sorts  lists  container 
    
CDItem  *Sx;//list  service  register
CDItem  *St;//list  service  register
    
CSort   *Si;//object service register
CSort   *Sj;//object service register
//--------------------------------------------------------------------
long    k;
double  V, r;
//--------------------------------------------------------------------
static void 
SortItemNew(void)
{
    Lx = (CDItem*) Malloc(sizeof(CDItem), UT, UT); //create ListItem
    Si = (CSort*)  Malloc(sizeof(CSort),  UT, UD); //create Object  
    //Loop Lx ListItem and connect to Si Object 
    Lx->p = Lx;
    Lx->n = Lx;    
    Lx->v = Si;
}//ListNew item provider for Sort, sets looped Lx
//--------------------------------------------------------------------
static void 
SortItemDel(void)
{
    free(Lx->v); free(Lx);
}//ListRem item provider for Sort
//--------------------------------------------------------------------
void 
SortInit(void)
{
    if (Sv != NULL) return;

    Sv = (CDList*) Malloc(sizeof(CDList), UT, UD);
	//Set Sorts containers counters and providers
	Sv->Fn = 0; Sv->ListItemDel = SortItemDel;
	Sv->Vn = 0; Sv->ListItemNew = SortItemNew; 
}//Inits start values in Sorts container. Set: Sv
//--------------------------------------------------------------------
void 
SortFree(void)
{
    if (Sv == NULL) return;
    Lv = Sv; ListFree(); 
    free(Sv); Sv = NULL;
}//Releases Free resources  Sv
//--------------------------------------------------------------------
static void
SortTest(long Id, long Bn, double Rc, double Mc)
{
    if (Rc <= 0.00 || Bn <= 0)  
    { 
        Si = (CSort*)-1; 
    }
    
    Si = NULL; //No error
}//Test sort parameters. Set: Si
//--------------------------------------------------------------------
static void
SortInit(long Id, long Bn, double Rc, double Mc)
{     
    Si->id = Id;   
    Si->Bn = Bn;
    Si->Rc = Rc;
    Si->Vc = Rc * Gc;
    Si->Rt = Rc * GR; // GR = (1.0 + Gc * Te)

    r = Rc; V = Vg * r; for (k = 1; k < Rn; k++) V *= r; 

    Si->Mc = (Mc > 0.0)? Mc : V; Si->Mt = Si->Mc * GM; // GM = GT^Rn		        
}//Init sort object, uses: Si
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
}//Get by id (color) Sort item pointer and object.            Set:  Sx
//--------------------------------------------------------------------
void 
SortAdd(long Id, long Bn, double Rc, double Mc)
{   
    SortGet(Id);      if(Sx != NULL) { return; }
    SortTest(Id, Bn, Rc, Mc); if(Si) { return; }
    Lv = Sv; ListAdd();  Sx = Lx; // Add new List item,  Lx == Sv->Vc
    Si = Sx->v; //Localize new object  
    SortInit(Id, Bn, Rc, Mc);  //Set values
}//Add new element to Sorts selector container.    Set: Sv->Vc, Sx, Si
//--------------------------------------------------------------------
void 
SortSet(long Id, long Bn, double Rc, double Mc)
{   
    SortGet(Id);      if(Sx == NULL) { return; }
    SortTest(Id, Bn, Rc, Mc); if(Si) { return; }
    Si = Sx->v; //Localize new object
    SortInit(Id, Bn, Rc, Mc);  //Set values
}//Set values Sort object.                                 Set: Sx, Si
//--------------------------------------------------------------------
void 
SortDel(long Id)
{   
    SortGet(Id); if(Sx == NULL) { return; }
    Lv = Sv; Sv->Vc = Sx; ListDel(); 
}//Moves selected Sort item to Free container          Set: Sv->Vc, Sx
//--------------------------------------------------------------------
void 
SortGrow(void)
{
    Sx = St = Sv->Vc;      
    do
    {      
        Si = St->v; 
        Si->Rt = (Si->Rc) * GR; //change size
        if(Gm)
        Si->Mt = (Si->Mc) * GM; //change mass
    }
    while((St = St->n) != Sx);
}//Sizes Sv->Vc Sorts and mass     
//--------------------------------------------------------------------
