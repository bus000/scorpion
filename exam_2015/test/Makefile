include ../Makefile.def

SOURCES   =$(wildcard *.cpp)$
OBJECTS   =$(SOURCES:.cpp=.o)$
EXOBJECTS =$(shell find '../src/' -name '*.o' | grep -v main.o)$
TARGETS   =$(SOURCES:.cpp=)$

.PHONY: clean $(TARGETS)

all: $(TARGETS)

camera: camera-test.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) -o camera -I../src/ 

$(TARGETS): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $@.o $(EXOBJECTS) -o $@ $(LIBS)
	./$@ > $@_output.txt
	diff $@_correct.txt $@_output.txt | colordiff

%.o: %.c
	$(CXX) -o $@ $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGETS)
	rm -f *output.txt
