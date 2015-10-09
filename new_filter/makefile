CC = g++ -g
flags = `pkg-config --cflags opencv`
libs = `pkg-config --libs opencv`
sources = $(wildcard *.cpp)
output = opencv

openvc: $(sources)
	$(CC) -o $(output) $(flags) $(sources) $(libs)

clean:
	rm -f *.o $(output)

