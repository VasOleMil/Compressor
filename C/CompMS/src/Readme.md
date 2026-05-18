Source files, currently - sketch for prototypes. Contains simple internal memory optimizer, kind of reduced to explicit constructor-destructor objects calling template. Not optimal for use in direct memory allocation. 

Sorts, Elements, Times, Data - processing with linked list. 

To increase speed and reduce memory usage, program can be reshaped withiout position verifier module, but it required base structure changes, so places with feature support is just highlited by cooments for simplicity of branch modification.


Notes:

* CComp.h is the entry point for library users (CompInit, CompStep, CompFree).
* CBase.h holds global simulation parameters and constants.
* CData.h centralizes standard library includes, memory wrappers, core data exchange. Header is excluded by macro when used by CComp.h to avoid redundant export linkage.

Core stepping:

* CSort.h - Sort structure, container and service
* CEmnt.h - Element structure, container and service
* CHold.h - Time-holder structure, container and service
* CComp.h - Core stepping functions
* CMode.h - entry for core extensioos, service, tools.
