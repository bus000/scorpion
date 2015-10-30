include ../Makefile.def

SOURCE		= $(wildcard *.cpp)
OBJECTS		= $(SOURCE:%.cpp=%.o)

$(MainExec): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o ../$(MainExec) $(OBJECTS)

%.o: %.c
	$(CXX) -o $@  $(CXXFLAGS) -c $<

clean:
	rm -f *.o ../$(MainExec)
