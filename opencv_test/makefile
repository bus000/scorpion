CC = g++ -g
flags = `pkg-config --cflags opencv`
libs = `pkg-config --libs opencv`
sources = $(wildcard *.cpp)
output = opencv

obstacle_avoidance: $(sources)
	$(CC) -o $(output) $(flags) $(sources) $(libs)

clean:
	rm -f *.o $(output)

