CXX := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
GITREV = -D'GIT_REVISION="$(shell git log --pretty=format:'%h' -n 1)"'
SRCDIR := src
BUILDDIR := build
TARGET := bin/HAIL-CAESAR.exe

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++11 -Wall -DOMP_COMPILE_FOR_PARALLEL -fopenmp $(GITREV)# -Wall #-DDEBUG
LIB := -fopenmp
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIB)"; $(CXX) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p bin
	@mkdir -p $(BUILDDIR)/topotools
	@mkdir -p $(BUILDDIR)/catchmentmodel
	@echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)/main.o $(BUILDDIR)/catchmentmodel/ $(BUILDDIR)/topotools/ $(TARGET)"; $(RM) -r $(BUILDDIR)/main.o $(BUILDDIR)/catchmentmodel/ $(BUILDDIR)/topotools/ $(TARGET)

# Tests
tester:
	$(CXX) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CXX) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
