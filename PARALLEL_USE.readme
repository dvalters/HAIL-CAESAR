# Using the model on parallel systems

This includes (at present) shared memory systems, e.g. workstation PCs
compute clusters (on a single node), HPC (on a single node)

An MPI (distribute memory) version is not yet available.

You will need to compile HAIL-CAESAR with a compiler that supports OpenMP.

This is usually provided with most modern C++ compilers, e.g. g++/gcc, Intel, etc.

The relevant flags are provided in the Makefile. (-fopenmp (for gcc) and
`-DOMP_COMPILE_FOR_PARALLEL`.

If the model can see a multithreaded environment, the output from STDOUT will 
look something like this when you run the `hailcaesar` executable:

```
##################################
#  CATCHMENT HYDROGEOMORPHOLOGY  #
#        MODEL version 1.0       #
#          (HAIL-CAESAR)         #
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Hello! My name is OpenMP. I like to do LSD (Land Surface Dynamics) in parallel!
Your system has: 4 PROCESSORS available and 4 THREADS to use!
(Note: On some systems, threads are reported the same as processors.)
Yo, whaddup! From thread number...0
Yo, whaddup! From thread number...1
Yo, whaddup! From thread number...2
Yo, whaddup! From thread number...3
Goodbye, parallel region!
```
