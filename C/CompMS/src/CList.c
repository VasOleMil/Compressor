#include"CList.h"
//--------------------------------------------------------------------
CDList *Lv; //Data container
CDItem *Lx; //Data exchange register
CFList *Wv; //Func container
CFItem *Wx; //Func exchange register
//--------------------------------------------------------------------
CDItem *Li; //local list register
CDItem *Lj; //local list register
CFItem *Wi; //local list register
CFItem *Wj; //local list register
long    Xc; //local list counter
//==================================================================//
//                     Base functions section                       //
//==================================================================//
//                        Core functions:                           //
//==================================================================//
static void                                                         //
InN(void)                                                           //
{                                                                   //
    Lj = Li->n; //Li is caller defined   /*  0 - +-<-+  */          //
    Lx->n = Lj; //Lx is caller defined   /*  |   |   |  */          //
    Lj->p = Lx; //Li assumed not empty   /*--i->-x->-j--*/          //
    Li->n = Lx; //Lx assumed not empty   /*  |   |   |  */          //
    Lx->p = Li; //(Lx == Li),  ensured   /*  +-<-+ - 0  */          //
}//Inserts Lx item after Li item                                    //
//------------------------------------------------------------------//
static void                                                         //
InP(void)                                                           //
{                                                                   //
    Li = Lj->p; //Lj is caller defined                              //
    Lx->p = Li; //Lx is caller defined                              //
    Li->n = Lx; //Li assumed not empty     Upper diagram            //
    Lj->p = Lx; //Lx assumed not empty                              //
    Lx->n = Lj; //(Lx == Lj),  ensured                              //
}//Inserts Lx item before Lj item                                   //
//--------------------------------------------------------------------
static void                                                         //
Det(void)                                                           //                                                    
{                                                                   //
    Li = Lx->p; //Lx is caller defined   /*  +--->---+  */          //
    Lj = Lx->n; //Lx assumed not empty   /*  |       |  */          //
    Lj->p = Li; //connect back over Lx   /*--i (<x>) j--*/          //
    Li->n = Lj; //connect next over Lx   /*  |       |  */          //
    Lx->p = Lx; //Loop for uniform add   /*  +---<---+  */          //
    Lx->n = Lx; //Looping in new items is also implemented          //
}//Detaches Lx item from List, use Li and Lj for direction          //
//--------------------------------------------------------------------
static void                                                         //
ListMrg(void)                                                       //
{	                                                                //  
                              /*** V(A < B) & F(a < b) ***/         //
                              /*   +----<-------<----+   */         //
                              /*   |       {V}       |   */         //
    //Li = (Lv->Vc);//[A]     /*   +->--A   *   B-->-+   */         //  
                              /*          \   /          */         //
                              /*   (#->0)   X   (#->n)   */         //
                              /*          /   \          */         //
    //Lj = (Lv->Fc);//[a]     /*   +->--a   *   b-->-+   */         //
                              /*   |       {F}       |   */         //
                              /*   +----<-------<----+   */         //
    //(a <-> B)               /*         (a<->B)         */         //
    Lx = Li->n;    //B(A)     /*    <<<                  */         //
    Lj->n = Lx;    //a->B     /*(Lv->Fc)->n = (Lv->Vc->n)*/         //
    Lx->p = Lj;    //B->a     /*(Lv->Vc->n)->p = (Lv->Fc)*/         //
    //(A <-> b)               /*         (A<->b)         */         //
    Lx = Lj->n;    //b(a)     /*    <<<                  */         //
    Li->n = Lx;    //A->b     /*(Lv->Vc)->n = (Lv->Fc->n)*/         //
    Lx->p = Li;    //b->A     /*(Lv->Fc->n)->p = (Lv->Vc)*/         //
}//List merging                                                     //
//====================================================================
//  Base_function_name_XY: X{N, P} - N step next, Y - container     //
//====================================================================
static void 
DetNF(void)
{
    Lx = Lv->Fc; Lv->Fn--;
    Det(); Lv->Fc = Lj; //Step next 
}//Detach from Free container to Lx  
//--------------------------------------------------------------------
static void 
DetNV(void)
{
    Lx = Lv->Vc; Lv->Vn--;   
    Det(); Lv->Vc = Lj; //Step next 
}//Detach from Value container to Lx  
//--------------------------------------------------------------------
static void 
DetPF(void)
{
    Lx = Lv->Fc; Lv->Fn--;
    Det(); Lv->Fc = Li; //Step back 
}//Detach from Free container to Lx  
//--------------------------------------------------------------------
static void 
DetPV(void)
{
    Lx = Lv->Vc; Lv->Vn--;   
    Det(); Lv->Vc = Li; //Step back 
}//Detach from Value container to Lx  
//--------------------------------------------------------------------
static void 
AttNF(void)
{
    Li = (Lv->Fn == 0)?(Lx):(Lv->Fc); 
    InN(); Lv->Fc = Lx; Lv->Fn++; //forward  
}//Attach Lx to Free container  
//--------------------------------------------------------------------
static void 
AttNV(void)
{
    Li = (Lv->Vn == 0)?(Lx):(Lv->Vc); 
    InN(); Lv->Vc = Lx; Lv->Vn++; //forward 
}//Attach Lx to Values container  
//--------------------------------------------------------------------
static void 
AttPF(void)
{
    Lj = (Lv->Fn == 0)?(Lx):(Lv->Fc); 
    InP(); Lv->Fc = Lx; Lv->Fn++; //back  
}//Attach Lx to Free container  
//--------------------------------------------------------------------
static void 
AttPV(void)
{
    Lj = (Lv->Vn == 0)?(Lx):(Lv->Vc); 
    InP(); Lv->Vc = Lx; Lv->Vn++; //back 
}//Attach Lx to Values container  
//--------------------------------------------------------------------
static void 
ListNewNF(void)
{ 
    if (Lv->Fn == 0) 
	{		           
        Lv->ListItemNew();//Create new list element, with adress in Lx
	}
	else 
	{      
        DetNF(); //Detach Lx from Free container, shift next     
	}   
}//Detaches from Free container, places new list element address to Lx 
//--------------------------------------------------------------------
static void 
ListNewPF(void)
{ 
    if (Lv->Fn == 0) 
	{		           
        Lv->ListItemNew();//Create new list element, with adress in Lx
	}
	else 
	{      
        DetPF(); //Detach Lx from Free container, shift previous     
	}   
}//Detaches from Free container, places new list element address to Lx 
//--------------------------------------------------------------------
static void 
ListDelNF(void)
{	
    DetNF(); Lv->ListItemDel();//Clean item and object
}//Release resources allocated by ListItemNew
//--------------------------------------------------------------------
static void 
ListDelPF(void)
{	
    DetPF(); Lv->ListItemDel();//Clean item and object
}//Release resources allocated by ListItemNew
//==================================================================//
//                         Generic section                          //
//==================================================================//
//  Base_function_name_XY:  X{N, P} - N step next, Y - container    //
//  reminder for construction. This section naming is generic       //
//==================================================================//
void 
ListAdd(void)
{
    //---------------------------------------------------------
    if (Lv->Fn == 0) 
	{		           
        Lv->ListItemNew();//Create new list element,   in   Lx
	}
	else 
	{      
        Lx = Lv->Fc; Lv->Fn--;
    
        Li = Lx->p;  //Lx is caller defined   /*  +--->---+  */          
        Lj = Lx->n;  //Lx assumed not empty   /*  |       |  */          
        Lj->p = Li;  //connect back over Lx   /*--i (<x>) j--*/          
        Li->n = Lj;  //connect next over Lx   /*  |       |  */          
        //Lx->p = Lx;//Loop for uniform add   /*  +---<---+  */          
        //Lx->n = Lx;//Looping in new items is also implemented          
    
        Lv->Fc = Lj; //DetNF()      
	}   
    //---------------------------------------------------------   
    Li = (Lv->Vn == 0)?(Lx):(Lv->Vc);

    Lj = Li->n; //Li is caller defined        /*  0 - +-<-+  */          
    Lx->n = Lj; //Lx is caller defined        /*  |   |   |  */          
    Lj->p = Lx; //Li assumed not empty        /*--i->-x->-j--*/          
    Li->n = Lx; //Lx assumed not empty        /*  |   |   |  */          
    Lx->p = Li; //(Lx == Li),  ensured        /*  +-<-+ - 0  */          

    Lv->Vc = Lx; Lv->Vn++; //AttNV()
    //---------------------------------------------------------
}//Moves Free to Value:                  Fc = Fc->n, Vc->n = Lx  
//--------------------------------------------------------------------
void 
ListDel(void)
{	
    if (Lv->Vn > 0) 
    { 
        //-----------------------------------------------------
        Lx = Lv->Vc; Lv->Vn--;   
    
        Li = Lx->p;  //Lx is caller defined   /*  +--->---+  */          
        Lj = Lx->n;  //Lx assumed not empty   /*  |       |  */          
        Lj->p = Li;  //connect back over Lx   /*--i (<x>) j--*/          
        Li->n = Lj;  //connect next over Lx   /*  |       |  */          
        //Lx->p = Lx;//Loop for uniform add   /*  +---<---+  */          
        //Lx->n = Lx;//Looping in new items is also implemented     
    
        Lv->Vc = Li; //DetPV()
        //-----------------------------------------------------
        Lj = (Lv->Fn == 0)?(Lx):(Lv->Fc); 
    
        Li = Lj->p; //Lj is caller defined    /*  0 - +-<-+  */ 
        Lx->p = Li; //Lx is caller defined    /*  |   |   |  */ 
        Li->n = Lx; //Li assumed not empty    /*--i->-x->-j--*/ 
        Lj->p = Lx; //Lx assumed not empty    /*  |   |   |  */  
        Lx->n = Lj; //(Lx == Lj),  ensured    /*  +-<-+ - 0  */ 
    
		Lv->Fc = Lx; Lv->Fn++; //AttPF() 
        //----------------------------------------------------- 
    }    
}//Moves Value to Free:                  Vc = Vc->p, Fc->p = Lx
//--------------------------------------------------------------------
void 
ListADD(void)
{
    //-----------------------------------------------------
    Lx = Lv->Fc; Lv->Fn--;
    
    Li = Lx->p;  //Lx is caller defined   /*  +--->---+  */          
    Lj = Lx->n;  //Lx assumed not empty   /*  |       |  */          
    Lj->p = Li;  //connect back over Lx   /*--i (<x>) j--*/          
    Li->n = Lj;  //connect next over Lx   /*  |       |  */          
    //Lx->p = Lx;//Loop for uniform add   /*  +---<---+  */          
    //Lx->n = Lx;//Looping in new items is also implemented          
    
    Lv->Fc = Lj;   //DetNF()  
    //-----------------------------------------------------
    Li = Lv->Vc; 

    Lj = Li->n; //Li is caller defined    /*  0 - +-<-+  */          
    Lx->n = Lj; //Lx is caller defined    /*  |   |   |  */          
    Lj->p = Lx; //Li assumed not empty    /*--i->-x->-j--*/          
    Li->n = Lx; //Lx assumed not empty    /*  |   |   |  */          
    Lx->p = Li; //(Lx == Li),  ensured    /*  +-<-+ - 0  */          

    Lv->Vc = Lx; Lv->Vn++; //AttNV()
    //----------------------------------------------------- 
}//Moves Free to Value:              Fc = Fc->n, Vc->n = Lx 
//--------------------------------------------------------------------
void 
ListDEL(void)
{	
    //-----------------------------------------------------
    Lx = Lv->Vc; Lv->Vn--;   
    
    Li = Lx->p;  //Lx is caller defined   /*  +--->---+  */          
    Lj = Lx->n;  //Lx assumed not empty   /*  |       |  */          
    Lj->p = Li;  //connect back over Lx   /*--i (<x>) j--*/          
    Li->n = Lj;  //connect next over Lx   /*  |       |  */          
    //Lx->p = Lx;//Loop for uniform add   /*  +---<---+  */          
    //Lx->n = Lx;//Looping in new items is also implemented     
    
    Lv->Vc = Li; //DetPV()
    //-----------------------------------------------------
    Lj = Lv->Fc; 
    
    Li = Lj->p; //Lj is caller defined    /*  0 - +-<-+  */ 
    Lx->p = Li; //Lx is caller defined    /*  |   |   |  */ 
    Li->n = Lx; //Li assumed not empty    /*--i->-x->-j--*/ 
    Lj->p = Lx; //Lx assumed not empty    /*  |   |   |  */  
    Lx->n = Lj; //(Lx == Lj),  ensured    /*  +-<-+ - 0  */ 
       
    Lv->Fc = Lx; Lv->Fn++; //AttPF() 
    //-----------------------------------------------------    
}//Moves Value to Free:              Vc = Vc->p, Fc->p = Lx
//--------------------------------------------------------------------
void
ListClr(void)
{	
    if (Lv->Vn == 0) return;     //safety

    Xc = Lv->Fn;//assumed controll exists
    Lj = Lv->Vc;//assumed  Vn > 0   //[a]
    Li = (Xc == 0)?(Lj):(Lv->Fc);   //[A]

    Lx = Li->n;           //Merge: a -> A
    Lj->n = Lx;
    Lx->p = Lj;

    Lx = Lj->n;
    Li->n = Lx;
    Lx->p = Li;
     
    Lv->Fn = Xc + Lv->Vn; // add counters
    Lv->Fc = Li;          // link  result
    Lv->Vn = 0;           // reset source
}//Move Value items, to Free container
//--------------------------------------------------------------------
void 
ListSize(void)
{
    while (Lv->Fn)  //ListDelNF()
    {
        Lx = Lv->Fc; Lv->Fn--;

        Li = Lx->p;
        Lj = Lx->n;
        Lj->p = Li;
        Li->n = Lj;

        Lv->Fc = Lj; Lv->ListItemDel();
    }
};//Releases Free container  resources
//--------------------------------------------------------------------
void 
ListFree(void)
{
    ListClr();      //Clear Values
    while (Lv->Fn)  //ListDelNF()
    {
        Lx = Lv->Fc; Lv->Fn--;

        Li = Lx->p;
        Lj = Lx->n;
        Lj->p = Li;
        Li->n = Lj;

        Lv->Fc = Lj; Lv->ListItemDel();
    }
};//Releases List containers resources
//--------------------------------------------------------------------
void
FuncAdd(void)
{
    //---------------------------------------------------------
    if (Wv->Fn == 0)
    {
        Wv->ListItemNew();//Create new List element,   in   Wx
    }
    else
    {
        Wx = Wv->Fc; Wv->Fn--;

        Wi = Wx->p;         
        Wj = Wx->n;    
        Wj->p = Wi;         
        Wi->n = Wj;         
 
        Wv->Fc = Wj;  //DetNF()  
    }
    //---------------------------------------------------------   
    Wi = (Wv->Vn == 0) ? (Wx) : (Wv->Vc);

    Wj = Wi->n;       
    Wx->n = Wj;          
    Wj->p = Wx;          
    Wi->n = Wx;         
    Wx->p = Wi;        

    Wv->Vc = Wx; Wv->Vn++; //AttNV()
    //---------------------------------------------------------
}//Moves Free to Value:                  Fc = Fc->n, Vc->n = Wx  
//--------------------------------------------------------------------
void
FuncDel(void)
{
    if (Wv->Vn > 0)
    {
        //-----------------------------------------------------
        Wx = Wv->Vc; Wv->Vn--;

        Wi = Wx->p;         
        Wj = Wx->n;    
        Wj->p = Wi;         
        Wi->n = Wj;         
      
        Wv->Vc = Wi; //DetPV()
        //-----------------------------------------------------
        Wj = (Wv->Fn == 0) ? (Wx) : (Wv->Fc);

        Wi = Wj->p; 
        Wx->p = Wi;
        Wi->n = Wx;
        Wj->p = Wx; 
        Wx->n = Wj;

        Wv->Fc = Wx; Wv->Fn++; //AttPF() 
        //----------------------------------------------------- 
    }
}//Moves Value to Free:                  Vc = Vc->p, Fc->p = Wx
//--------------------------------------------------------------------
void
FuncClr(void)
{
    if (Wv->Vn == 0) return;     //safety

    Xc = Wv->Fn;//assumed controll exists
    Wj = Wv->Vc;//assumed  Vn > 0   //[a]
    Wi = (Xc == 0)?(Wj):(Wv->Fc);   //[A]  

    Wx = Wi->n;           //Merge: a -> A
    Wj->n = Wx;   
    Wx->p = Wj;   
   
    Wx = Wj->n;   
    Wi->n = Wx;  
    Wx->p = Wi;   

    Wv->Fn = Xc + Wv->Vn; // add counters
    Wv->Fc = Wi;          // Link  result
    Wv->Vn = 0;           // reset source
}//Move Value items, to Free container
//--------------------------------------------------------------------
void
FuncSize(void)
{
    while (Wv->Fn)  //ListDelNF()
    {
        Wx = Wv->Fc; Wv->Fn--;
        
        Wi = Wx->p;
        Wj = Wx->n;
        Wj->p = Wi;
        Wi->n = Wj;
        
        Wv->Fc = Wj; Wv->ListItemDel();
    }
};//Releases Free container  resources
//--------------------------------------------------------------------
void
FuncFree(void)
{
    FuncClr();      //Clear Values
    
	while (Wv->Fn)  //ListDelNF()
    {
        Wx = Wv->Fc; Wv->Fn--;

        Wi = Wx->p;
        Wj = Wx->n;
        Wj->p = Wi;
        Wi->n = Wj;

        Wv->Fc = Wj; Wv->ListItemDel();
    }
};//Releases List containers resources
//--------------------------------------------------------------------