Installation
============

#. Download the software, either as a zip/tar file, or by cloning the latest version from github.
#. The makefile is currently set up to use the gcc compiler, edit Makefile if you wish to change this, otherwise:
#. Run make in the same folder as `Makefile`.
#. Wait for the code to compile, when finished it will have produced an executable called `HAIL-CAESAR.exe`.
#. You are now ready to run the model!

Dependencies
------------

.. NOTE:: *Your compiler must support the C++11 standard.* Almost all compilers do as of 2016, as long as you are running a fairly recent version. (GCC version 4.7 onwards should be fine, preferrably GCC 4.8+) Currently, support is not usually enabled by default. (UPDATE 2016: It is in GCC 6.1) The C++11 flag to turn on support for the standard is included in the makefile. The code uses some features of the C++11 language standard not supported by older standards. You don't need to know anything about it further to use the model.

The parallel version of the code uses the OpenMP libraries, which are powerful (and somewhat magical...) libraries for compiling the code to run in parallel. These are widely supported libraries and many systems will come with them pre-installed. *But you may need to install the `gcc-devel` package on linux, if using gcc*. Again, the compiler flag is taken care of in the makefile. The code has been tested on the `gcc` (versions 4.8 and above) Cray compiler, and `icc` (Intel) compiler v12. The code has not been extensively tested on Windows, but a recent enough Microsoft C++ compiler (such as comes with Visual Studio), should be sufficient to compile the code. You may need to modify the `Makefile` when using other compilers.

All other libraries are supplied with the source code (The TNT, *Template Numerical Toolkit* library, which provides array/matrix structures for the model data). You do not need to install them separately.
