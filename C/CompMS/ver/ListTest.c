#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include"..\src\CList.h"
#include"..\src\CMode.h"
#include"ListTest.h"

//--------------------------------------------------------------------
static void
TestItemNew(void)
{
    Lx = (CDItem*)malloc(sizeof(CDItem)); if (Lx == NULL) exit(1); 
    Lx->p = Lx; //Loop Lx ListItem 
    Lx->n = Lx;
    Lx->v = NULL;
}//ListNew item provider for Test, sets looped Lx
//--------------------------------------------------------------------
static void
TestItemDel(void)
{
    free(Lx);
}//ListDel item provider for Test
//--------------------------------------------------------------------
static void
ListINIT(void)
{
    if (Lv != NULL) return;
    //Dedicated list testing, init list with malloc
    Lv = (CDList*)malloc(sizeof(CDList));   if (Lv == NULL) exit(1);
    //Set Test containers counters and providers
    Lv->Fn = 0; Lv->ListItemDel = TestItemDel;
    Lv->Vn = 0; Lv->ListItemNew = TestItemNew;
}
//--------------------------------------------------------------------
static void
ListFREE(void)
{
    if (Lv == NULL) return;
    //Dedicated list testing, init list with malloc
    ListFree(); free(Lv); Lv = NULL;
}
//--------------------------------------------------------------------
void 
ListTestBase(void)
{
    //Use direct memory allocation to dedicated list core test
    Lv = NULL; int a, i, j; a = 0;

    //Allocate functions array to process pairing
    size_t Fn = 4;             //number to test
    typedef void (*FAU)(void); //funtions array
    FAU *FA = (FAU*)malloc(sizeof(FAU) * Fn); if (FA == NULL) exit(1);

    //First stage, possible pairs of ListAdd, ListDel
    FA[0] = ListAdd; FA[1] = ListVoid;
    FA[2] = ListDel; FA[3] = ListVoid;
    printf("\n\nFirst stage: pairs of ListAdd, ListDel, ListVoid\n\n");
    for (i = 0; i < Fn; i++)for (j = 0; j < Fn; j++)
    {
        ListINIT();

        FA[i](); FA[j]();
        
        if (TestStruct(Lv)) { i = j = Fn; a = 1; };
        
        ListFREE();  //free list resources
    }

    if (a == 0) printf("\n\tTest passed\n"); free(FA); return;
}
//--------------------------------------------------------------------
void
ListTestPair(void)
{
    //Use direct memory allocation to dedicated list core test
    Lv = NULL; int a, i, j; a = 0;

    //Allocate functions array to process pairing
    size_t Fn = 6;             //number to test
    typedef void (*FAU)(void); //funtions array
    FAU* FA = (FAU*)malloc(sizeof(FAU) * Fn); if (FA == NULL) exit(1);
    //Second stage, possible pairs of All+Void
    FA[0] = ListAdd;  FA[1] = ListDel;
    FA[2] = ListClrV; FA[3] = ListSize;
    FA[4] = ListFree; FA[5] = ListVoid;
    printf("\n\nSecond stage: ListFunc + ListVoid pairs\n\n");
    for (i = 0; i < Fn; i++)for (j = 0; j < Fn; j++)
    {
        ListINIT();

        FA[i](); FA[j]();

        if (TestStruct(Lv)) { i = j = Fn; a = 1; };

        ListFREE();  //free list resources
    }

    if (a == 0) printf("\n\tTest passed\n"); free(FA); return;
}
//--------------------------------------------------------------------
void
ListTestRand(void)
{
    //Use direct memory allocation to dedicated list core test
    Lv = NULL; int a, c, s, f11, f12, f21, f22;

    ListINIT(); ListFREE(); if (Lv!=NULL) exit(1); //base test
    
    //Allocate functions array to process pairing
    size_t Fn = 6;             //number to test
    typedef void (*FAU)(void); //funtions array
    FAU* FA = (FAU*)malloc(sizeof(FAU) * Fn); if (FA == NULL) exit(1);

    FA[0] = ListAdd;  FA[1] = ListDel;  //FA[0] = ListAdd;  FA[1] = ListDel;
    FA[2] = ListClrV; FA[3] = ListSize; //FA[2] = ListClrV; FA[3] = ListSize;
    FA[4] = ListFree; FA[5] = ListVoid; //FA[4] = ListFree; FA[5] = ListVoid;
    //string functions names for report
    const char* fN[] = 
    {   
        "ListAdd ","ListDel ",
        "ListClrV","ListSize",
        "ListFree","ListVoid" 
    };
    s = Fn * Fn * Fn * Fn * 100; a = 0; srand(time(NULL));
    printf("\n\nTest stage: ListFunc + ListVoid, double pairs\n");
    printf("\n\t%ld random calls of double pair", s);
    printf("\n\t1%% to miss 4 calls of any same function\n\n");
    //Stress list
    for (c = 0; c < s; c++)
    {
        ListINIT(); 

        FA[f11 = rand() % Fn](); FA[f12 = rand() % Fn]();
        FA[f21 = rand() % Fn](); FA[f22 = rand() % Fn]();
       
        if (TestStruct(Lv))
        {
            printf("\nError on step: %ld\n",
                c);
            printf("\tItem counters: Fn=%ld, Vn=%ld\n",
                Lv->Fn, Lv->Vn);
            printf("\t%s\t%s\n\t%s\t%s\n", 
                fN[f11], fN[f12], fN[f21], fN[f22]);
            c = s; a = 1;
        }

        ListFREE();  //free list resources
    }
    
    if (a == 0) printf("\n\tTest passed\n"); free(FA); return;
}
//--------------------------------------------------------------------
