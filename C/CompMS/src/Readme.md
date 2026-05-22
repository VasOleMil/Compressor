Current source files provide prototype implementations of core modules. They include a simple internal memory optimizer based on explicit constructor/destructor calls. Direct raw memory allocation is not recommended.

Sorts, Elements, Times, Data - processing with ring list. 

Notes:

* CComp.h is the entry point for library users (CompInit, CompStep, CompFree).
* CBase.h holds global simulation parameters and constants.
* CData.h centralizes standard library includes, memory wrappers, core data exchange. Header is excluded by macro when used by CComp.h to avoid redundant export linkage.

Usage:

```c
//--------------------------------------------------------------------
//"CompTest.h" example
#ifndef COMPTEST_HREADME
#define COMPTEST_HREADME

#ifdef __cplusplus
extern "C" {
#endif
//- - - - - - - - - - - - - - -
extern void CompTestStep(void);
//- - - - - - - - - - - - - - -
#ifdef __cplusplus
}
#endif
#endif//COMPTEST_HREADME
//--------------------------------------------------------------------
//"CompTest.c" example
#include"CompTest.h"
#include"СComp.h"
void
CompTestStep()
{
	int i, Steps = 200;

	CompInit(2);

	SortAdd(1, 7, 1.0, 0.0);

	CompLoad(1.0, 0.0, 0.0);

	for (i = 0; i < Steps; i++)
	{      CompStep();        }

	CompFree();
}
//--------------------------------------------------------------------
```

Interrnal types and data:

* CList - responsible for the data structure (ring lists, addition, deletion, merging, clearing).
Works exclusively with pointers to list elements, does not deal with memory directly.

* CData - responsible for memory and internal container. Uses `malloc`/`free` to work with data, does not interfere with the logic of lists.
Functions are autonomous, do not require stack wrappers. 

_Interaction is near type and object. Modules have no dependent functions._

Core stepping:

* CSort.h - Sort structure, container and service
* CEmnt.h - Element structure, container and service
* CHold.h - Time-holder structure, container and service
* CComp.h - Core stepping functions
* CMode.h - entry for core extensioos, service, tools.

To increase speed and reduce memory usage, program can be reshaped withiout position verifier module, but it required base structure changes, so places with feature support is highlited by cooments for simplicity of branch modification.




