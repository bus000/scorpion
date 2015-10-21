# The OpenCV package config file must be in the PKG_CONFIG_PATH, e.g.
#   export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/local/lib/pkgconfig


# Compile and link flags
CXX         =  g++ -g -g
CXXFLAGS    =
OPENCVFLAGS = `pkg-config opencv playerc++ --cflags`
CXXFLAGS    = $(OPENCVFLAGS)
OPENCVLIBS  = `pkg-config opencv playerc++ --libs`

# Main executable
EXECUTABLE = exercise5

# Files
SOURCES = particles.cc random_numbers.cc camera.cc particleFilter.cpp DriveCtl.cpp stateMachine.cpp
CCFILES = $(EXECUTABLE).cc $(SOURCES)
OBJECTS = $(patsubst %.cc, %.o, $(CCFILES))
OBJECTS := $(patsubst %.cpp, %.o, $(OBJECTS))

## Compile everything ##
all: $(EXECUTABLE)

# Linking
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OPENCVLIBS) -lpthread $(OBJECTS) -o $(EXECUTABLE)

# Compilation.
%.o : %.cc %.cpp
	$(CXX) $(CXXFLAGS) -c $^ 

# Cleaning up
clean :
	rm -f *.o *.lo *.la *.so *~ $(EXECUTABLE)
