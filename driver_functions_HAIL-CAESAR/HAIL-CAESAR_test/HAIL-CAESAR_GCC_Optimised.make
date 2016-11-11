CC = g++
CFLAGS= -c -Wall -O3  -march=native -fopenmp \
		-DOMP_COMPILE_FOR_PARALLEL 
OFLAGS = -Wall -O3 -fopenmp -march=native -DOMP_COMPILE_FOR_PARALLEL 
LDFLAGS= -Wall
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
EXEC = HAIL-CAESAR_OpenMP_GCC_Optimised_O3_marchnative.out

all: $(SOURCES) $(SCRIPTS) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f ../../$(OBJ) ../HAIL-CAESAR_driver.o HAIL-CAESAR_OpenMP_GCC_Optimised.out
