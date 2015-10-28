SOURCES   =$(wildcard *.cpp)$
OBJECTS   =$(SOURCES:.cpp=.o)$
EXOBJECTS =$(shell find '../src/' -name '*.o' | grep -v main.o)$
TARGETS   =$(SOURCES:.cpp=)$
LFLAGS    =-lm

.PHONY: clean $(TARGETS)

all: $(TARGETS)

$(TARGETS): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $@.o $(EXOBJECTS) -o $@ $(LFLAGS)
	./$@ > $@_output.txt
	diff $@_correct.txt $@_output.txt | colordiff

%.o: %.c
	$(CXX) -o $@ $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGETS)
	rm -f *output.txt
