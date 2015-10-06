CC = g++ -g
flags = `pkg-config --cflags playerc++`
libs = `pkg-config --libs playerc++`
sources = $(wildcard *.cpp)
output = rotation

rotation: $(sources)
	$(CC) -o $(output) $(flags) $(sources) $(libs)

clean:
	rm -f *.o $(output)
