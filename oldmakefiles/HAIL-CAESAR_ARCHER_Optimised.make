CC = CC 
# Note: OpenMP and MPI are enabled by default on the ARCHER system
# The compiler is set by loading the PrgEnv-cray or PrgEnv-gcc environments
CFLAGS= -c -O2 -hstd=c++11 -DOMP_COMPILE_FOR_PARALLEL
OFLAGS = -o -O2 -hstd=c++11 
LDFLAGS= 
SOURCES = ../HAIL-CAESAR_driver.cpp \
			../../LSDCatchmentModel.cpp \
			../../LSDRaster.cpp \
			../../LSDIndexRaster.cpp \
			../../LSDStatsTools.cpp \
			../../LSDShapeTools.cpp \
			../../LSDGrainMatrix.cpp \
			../../LSDRainfallRunoff.cpp
SCRIPTS = 
OBJ = $(SOURCES:.cpp=.o)
#LIBS = -lfftw3 -lpython2.7 -g -O0 -D_GLIBCXX_DEBUG
#LIBS = -lfftw3 -lpython2.7 -Wwrite-strings
LIBS = 
EXEC = HAIL-CAESAR_ARCHER_Optimised.out

all: $(SOURCES) $(SCRIPTS) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f ../../$(OBJ) ../HAIL-CAESAR_driver.o HAIL-CAESAR_ARCHER_Optimised.out
