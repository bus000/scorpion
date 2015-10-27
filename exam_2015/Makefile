# The OpenCV package config file must be in the PKG_CONFIG_PATH, e.g.
#   export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/local/lib/pkgconfig


# Compile and link flags
CXX         =  g++
CXXFLAGS    = -g
OPENCVFLAGS = `pkg-config opencv --cflags`
OPENCVLIBS  = `pkg-config opencv --libs`

PLAYERFLAGS = `pkg-config playerc++ --cflags`
PLAYERLIBS  = `pkg-config playerc++ --libs`
CXXFLAGS    = $(OPENCVFLAGS)

# Main executable
MainExec = robot_runner

# Files
SOURCES = $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

## Compile everything ##
all: $(MainExec)

camera-test: $(BIN)
	$(CXX) $(OPENCVLIBS) -I src/ src/camera.cpp test/camera-test.cpp -o bin/camera-test

# Linking
$(MainExec): $(OBJECTS)
	$(CXX) $(OPENCVLIBS) $(OBJECTS) -o $(MainExec)

# Compilation.
$(OBJECTS) : $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $^ 

$(BIN):
	mkdir bin

# Cleaning up
clean :
	rm -f *.o *.lo *.la *.so *~ $(EXECUTABLE)
