CC = clang++ -g
flags = `pkg-config --cflags opencv playerc++`
libs = `pkg-config --libs opencv playerc++`
sources = $(wildcard *.cpp)
output = opencv

openvc: $(sources)
	$(CC) -o $(output) $(flags) $(sources) $(libs)

clean:
	rm -f *.o $(output)

