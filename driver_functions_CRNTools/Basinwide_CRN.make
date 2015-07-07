# make with make -f Basinwide_CRN.make

CC=g++
CFLAGS=-c -Wall -O3
OFLAGS = -Wall -O3
LDFLAGS= -Wall
SOURCES=Basinwide_cosmogenic_analysis.cpp \
        ../LSDMostLikelyPartitionsFinder.cpp \
        ../LSDChiNetwork.cpp \
        ../LSDIndexRaster.cpp \
        ../LSDRaster.cpp \
        ../LSDShapeTools.cpp \
        ../LSDFlowInfo.cpp \
        ../LSDJunctionNetwork.cpp \
        ../LSDIndexChannel.cpp \
        ../LSDChannel.cpp \
        ../LSDIndexChannelTree.cpp \
        ../LSDStatsTools.cpp \
        ../LSDBasin.cpp \
        ../LSDParticle.cpp \
        ../LSDCRNParameters.cpp \
        ../LSDCosmoData.cpp \
        ../LSDRasterInfo.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Basinwide_CRN.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
