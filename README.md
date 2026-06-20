# Compressor

Compressor based on hard sphere model. Asynchronous Event‑Driven Solver, based on Independent Free‑Path Principle. Spherical elements of arbitrary size packing task. 

Mathematica/Compression V01.nb, asynchronous optimization using linked lists for times storage, reduces calculation complexity from square to near linear on number of elements, with complete physisity. Since mathematica does not support linked lists directly, indices are just a separate additional array. Without the verification with preserved final position, simple simulation will show smaller energy drift, validation is more accurate and faster than simple eventual. Scattering functions are classic or near elastic, simplicity and small probability of back scatter makes them usefull in modeling, as well as fast not rotated implulse preserving procedure in case of only compression task, function body presents as reference but not used. The content is mainly about the basis of algorithms and fucntion tests. 

Besides personally checked asynchronous effectiveness for more then decade mainly on compression tasks, it is still not widely used in public, and therefore have problems with verification. Project includes simple eventual set of functions for statistical tasks that demands verified methods for processing. 

### Implementation Peculiarities: Cache-Driven Event Processing

This asynchronous implementation peculiarity lies in using sequentially traversed lists, instead of the bisectional EDMD binary tree or more advanced direct PDF predictors (exponential mean free time) for processing event storage, - since the size remains near-linear and complexity is comparable to the next-event computation itself. The major advantage/restriction is bound to the list size; it could be further reduced by using aligned buffers and reduced pointer sizes, though the latter would require platform-specific functions. Therefore, the main speed-up compared to implementations well-[known](https://arxiv.org/pdf/1004.3501) since 1980 and earlier is CPU cache utilization, achieved via index minimization. While hardware-dependent under the hood, it is covered by widespread CPU cache profiling architectures, making the program widely portable and suitable for applications like scaled database materials creation.

More detailed description of procedures and functions here in [Wiki](../../wiki).

_The base repo illustrates morphing but does not implement polymorph. Polymorph is determined by the project, since wide bounding by task provide consisntency. The repo contains programmer languages; the project defines module tasking._
