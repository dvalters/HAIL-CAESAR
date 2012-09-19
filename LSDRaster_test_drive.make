# make with make -f LSDRaster_test_drive.make

CC=g++
CFLAGS=-c -Wall -O3 -pg
OFLAGS = -Wall -O3 -pg
LDFLAGS= -Wall
SOURCES=LSDRaster_test_drive.cpp LSDIndexRaster.cpp LSDRaster.cpp LSDFlowInfo.cpp LSDChannelNetwork.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=LSDRaster_test_drive.exe
# EXECUTABLE=LSDRaster_test_drive.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
