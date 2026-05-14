#ifndef CLIST_H888
#define CLIST_H888

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
typedef struct CLItem
{
    struct CLItem * p; //Previous item
    struct CLItem * n; //Next item
    void          * v; //Object link
}   CLItem;
//--------------------------------------------------------------------
typedef struct CSList
{
    CLItem          *Vc;//Values container,       current element
	long             Vn;//Values counter

	CLItem          *Fc;//Free for use container, current element
	long	         Fn;//Free items counter

    void           (*ListItemNew)(void); //Invoked in ListNew(N^P)F()
    void           (*ListItemDel)(void); //Invoked in ListDel(N^P)F()
}   CSList;
//--------------------------------------------------------------------
extern CSList *Lv; //Lists container
extern CLItem *Lx; //List  exchange register
//==================================================================//
//                      Project functions:                          //
//==================================================================//
//  Y - container  |   Lx == Lv->Yc         |                       //
//--------------------------------------------------------------------
extern void ListAdd(void);//Move Free to Value, one             Vn, Lx
//--------------------------------------------------------------------
extern void ListDel(void);//Move Value to Free, one                 Vn
//--------------------------------------------------------------------
extern void ListClr(void);//Move Value to Free, all                 Vn 
//--------------------------------------------------------------------
extern void ListADD(void);//Move Free to Value, unsafe:         Vn, Lx
//--------------------------------------------------------------------
extern void ListDEL(void);//Move Value to Free, unsafe:             Vn
//--------------------------------------------------------------------
extern void ListSize(void); //Releases Free container resources
//--------------------------------------------------------------------
extern void ListFree(void); //Releases List container resources
//--------------------------------------------------------------------


//==================================================================//
//      Compose necessarry ordering composition in file: CList.c    //
//==================================================================//
//                      Naming convention:                          //
//==================================================================//
//  Base_function_name_XY: X{N, P} - N step next, Y - container     //
//==================================================================//

#ifdef __cplusplus
}
#endif
#endif//CLIST_H888
