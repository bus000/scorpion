# The OpenCV package config file must be in the PKG_CONFIG_PATH, e.g.
#   export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/local/lib/pkgconfig


# Compile and link flags
CXX         =  g++
CXXFLAGS    = -g
OPENCVFLAGS = `pkg-config opencv playerc++ --cflags`
CXXFLAGS    = $(OPENCVFLAGS)
OPENCVLIBS  = `pkg-config opencv playerc++ --libs`

# Main executable
MainExec = robot_runner

# Files
SOURCES = $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

## Compile everything ##
all: $(MainExec)

# Linking
$(MainExec): $(OBJECTS)
	$(CXX) $(OPENCVLIBS) $(OBJECTS) -o $(MainExec)

# Compilation.
$(OBJECTS) : $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $^ 

# Cleaning up
clean :
	rm -f *.o *.lo *.la *.so *~ $(EXECUTABLE)
