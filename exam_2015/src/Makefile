include ../Makefile.def

SOURCE		= $(wildcard *.cpp)
OBJECTS		= $(SOURCE:%.cpp=%.o)

$(MainExec): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o ../$(MainExec) $(OBJECTS)

$(OBJECTS): $(SOURCE)
	$(CXX) $(CXXFLAGS) -c $?

clean:
	rm *.o ../$(MainExec)
