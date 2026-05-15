#ifndef CLIST_H888
#define CLIST_H888

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
typedef struct CDItem
{
    struct CDItem *p; //Previous item
    struct CDItem *n; //Next item
    void          *v; //Object link

}   CDItem; //Data type list item
//--------------------------------------------------------------------
typedef struct CFItem
{
    struct CFItem *p;           //Previous item
    struct CFItem *n;           //Next item
    void         (*w)(void);    //function link

}   CFItem; //Func type list item
//--------------------------------------------------------------------
typedef struct CDList
{
    CDItem          *Vc;//Values container,       current element
	long             Vn;//Values counter

	CDItem          *Fc;//Free for use container, current element
	long	         Fn;//Free items counter

    void           (*ListItemNew)(void); //Invoked in ListNew(N^P)F()
    void           (*ListItemDel)(void); //Invoked in ListDel(N^P)F()

}   CDList; //Data type list container
//--------------------------------------------------------------------
typedef struct CFList
{
    CFItem         *Vc;//Values container,       current element
    long            Vn;//Values counter

    CFItem         *Fc;//Free for use container, current element
    long	        Fn;//Free items counter

    void           (*ListItemNew)(void); //Invoked in ListNew(N^P)F()
    void           (*ListItemDel)(void); //Invoked in ListDel(N^P)F()

}   CFList; //Func type list container
//--------------------------------------------------------------------
CDList *Lv; //Data container
CDItem *Lx; //Data exchange register
//--------------------------------------------------------------------
CFList *Wv; //Func container
CFItem *Wx; //Func exchange register
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
extern void FuncAdd(void);//Move Free to Value, one             Vn, Wx
//--------------------------------------------------------------------
extern void FuncDel(void);//Move Value to Free, one                 Vn
//--------------------------------------------------------------------
extern void FuncClr(void);//Move Value to Free, all                 Vn 
//--------------------------------------------------------------------
extern void FuncSize(void); //Releases Free container resources
//--------------------------------------------------------------------
extern void FuncFree(void); //Releases List container resources
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
