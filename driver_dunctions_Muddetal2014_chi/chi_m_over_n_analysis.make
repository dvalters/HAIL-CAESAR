# make with make -f chi_m_over_n_analysis.make

CC=g++
CFLAGS=-c -Wall -O3
OFLAGS = -Wall -O3
LDFLAGS= -Wall
SOURCES=chi_m_over_n_analysis_driver.cpp ../LSDMostLikelyPartitionsFinder.cpp ../LSDChiNetwork.cpp ../LSDStatsTools.cpp ../LSDShapeTools.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=chi_m_over_n_analysis.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
