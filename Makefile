CXX := g++-7 # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
GITREV = -D'GIT_REVISION="$(shell git log --pretty=format:'%h' -n 1)"'
SRCDIR := src
BUILDDIR := build
TARGET := bin/HAIL-CAESAR 

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++11 -DOMP_COMPILE_FOR_PARALLEL -fopenmp $(GITREV)# -Wall
GEODECOMP_DIR := /Users/aproeme/libgeodecomp/0.4.0
INC := -I ./include -I $(GEODECOMP_DIR)/include
LDFLAGS := -fopenmp -L $(GEODECOMP_DIR)/lib
LIBS := -lgeodecomp


$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CXX) $(LDFLAGS) $(LIBS) $^ -o $(TARGET)"; $(CXX) $(LDFLAGS) $(LIBS) $^ -o $(TARGET) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p bin
	@mkdir -p $(BUILDDIR)/topotools
	@mkdir -p $(BUILDDIR)/catchmentmodel
	@echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)/catchmentmodel/ $(BUILDDIR)/topotools/ $(TARGET)"; $(RM) -r $(BUILDDIR)/catchmentmodel/ $(BUILDDIR)/topotools/ $(TARGET)

# Tests
tester:
	$(CXX) $(CFLAGS) test/tester.cpp $(INC) $(LIBS) -o bin/tester

# Spikes
ticket:
	$(CXX) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIBS) -o bin/ticket

.PHONY: clean
