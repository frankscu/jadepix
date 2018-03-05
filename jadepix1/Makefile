#
#  Xin Shi <shixin@ihep.ac.cn>
#  2018.03.01 
#

BIN=./bin
SRC=./src

CC = g++
GCCFLAGS  = -Wall -g  -std=c++0x

ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))


FLAGS=$(GCCFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) 


PROG=genApx convert
LIST=$(addprefix $(BIN)/, $(PROG))


all: $(LIST)
	@echo "Build successful."

$(LIST): | $(BIN)


$(BIN): 
	mkdir -p $(BIN)

$(BIN)/genApx: $(SRC)/genApx.cc 
	$(CC) $< $(FLAGS) -L/opt/allpix/v1.1.0/lib -lAllpixObjects -o $@

$(BIN)/convert: $(SRC)/convert.cc 
	$(CC) $< $(FLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(BIN)/genApx
	rm -f $(BIN)/convert
