CC = g++ -g
flags = `pkg-config --cflags opencv`
libs = `pkg-config --libs opencv`
sources = calibrate.cpp
output = calibrate

openvc: $(sources)
	$(CC) -o $(output) $(flags) $(sources) $(libs)

clean:
	rm -f *.o $(output)

