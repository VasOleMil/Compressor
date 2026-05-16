Source files, currently - sketch for prototypes. Contains simple internal memory optimizer, kind of reduced to explicit constructor-destructor objects calling template. Not optimal for use in direct memory allocation. 

Sorts, Elements, Times, Data - processing with linked list. 

To increase speed and reduce memory usage, program can be reshaped withiout position verifier module, but it required base structure changes, so places with feature support is just highlited by cooments for simplicity of branch modification.


Header usage:

CComp.h        → Public library interface
   |
   └── CBase.h   → Global simulation parameters & constants
          |
          └── CData.h   → Collector of standard libs + memory discipline
          

CData.h is excluded by macro while use in Comp.h
