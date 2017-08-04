# Using the model on parallel systems

This includes (at present) shared memory systems, e.g. workstation PCs
compute clusters (on a single node), HPC (on a single node)

An MPI (distributed memory) version is not yet available. (i.e. model domain 
spread over multiple nodes).

You will need to compile HAIL-CAESAR with a compiler that supports OpenMP. 
This is usually provided with most modern C++ compilers, e.g. g++/gcc, Intel, etc.

The relevant flags are provided in the Makefile. (`-fopenmp` (for gcc) and
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

## Troubleshooting

__My system definitely has multiple processors/CPU cores, but I am only getting 1 thread reported?__

- You may need to explicitly set the `OMP_NUM_THREADS` environment variable. On Unix systems, this is 
done using either `export OMP_NUM_THREADS=[number]` (e.g. 4 for four cores). or `set OMP_NUM_THREADS=[number]`.
You need to check with your system's documentation to see which one is necessary. 

- Usually, you should set `OMP_NUM_THREADS` to match the number of cores/CPUs your system has, however you
may wish to experiment with a number of threads than cores if you doing a lot of heavy and frequent I/O
(i.e. something crazy like writing DEMs every few seconds...)

__I am confused whether my laptop/computer/server/cluster/hpc has cores/CPUs/processors/nodes or something else__

- If you have a relatively modern computer or laptop, you can almost certainly make use of the parallel options 
in HAIL-CAESAR. (When I'm travelling I test it on a crappy netbook, and even that has multiple cores, so it can
do stuff in parallel)

- _CPU_ is used as a generic term nowadays to mean a processing unit. _Processor_ can mean the physical chip(s)
inside your machine (though sometimes it is used interchangeably with CPU.). _Core_ is a region of the physical
computer chip that can be treated as if it were a single, independent CPU. 

- A laptop, for example, usually has a single chip inside it, which has multiple cores. Each core can execute 
a portion of the HAIL-CAESAR code at the same time -- in parallel -- and so speed up the running of the model.
If you were to look in the system settings, this sometimes shows up as individual CPUs.

- To make matters more confusing, some systems (cluster computers, servers, fancy workstation computers, have 
multiple _physical_ processors with multiple cores. So if you have a server with 4 processor chips, and each has
4 cores on it, you actually have 16 'CPUs' to use.

- _Nodes_ are something else entirely found on cluster machines and HPC/supercomputers. A _node_ is a single unit
of processors that can all access the same memory. Nodes are networked together to form supercomputers or clusters.
You cannot (yet) run _one_ instance of HAIL-CAESAR over multiple nodes, but you can run _multiple_ instances of 
HAIL-CAESAR over multiple nodes, e.g. for ensemble simulations or sensitivity analyses.

