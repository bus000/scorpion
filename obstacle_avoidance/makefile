CC = g++ -g
flags = `pkg-config --cflags playerc++`
libs = `pkg-config --libs playerc++`
sources = $(wildcard *.cpp)

obstacle_avoidance: $(sources)
	$(CC) -o obstacle_avoidance $(flags) $(sources) $(libs)
