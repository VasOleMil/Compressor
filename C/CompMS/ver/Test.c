#include"ListTest.h"
#include"CompTest.h"

//--------------------------------------------------------------------
int 
main()
{
    //Direct list structure testing, using malloc free
    ListTestBase();
    ListTestPair();
    ListTestRand();
    
    //Stepping test, using Comp interface
    //CompTestStepHold();
    CompTestStepScan();

    return 0;
}
//--------------------------------------------------------------------
