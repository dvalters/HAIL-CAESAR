CC = g++
CFLAGS= -c -Wreturn-type -Og -pg -ggdb -std=gnu++11 -fopenmp -D_GLIBCXX_DEBUG \
			-DOMP_COMPILE_FOR_PARALLEL -DDEBUG
OFLAGS = -Wreturn-type -Og -pg -ggdb -std=gnu++11 -fopenmp -D_GLIBCXX_DEBUG -DDEBUG
LDFLAGS= -Wreturn-type
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
EXEC = HAIL-CAESAR_OpenMP_GCC_DEBUG.out

all: $(SOURCES) $(SCRIPTS) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f ../../$(OBJ) ../HAIL-CAESAR_driver.o HAIL-CAESAR_OpenMP_GCC_DEBUG.out
