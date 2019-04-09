CFLAGS=-std=c++11 -f-no-exceptions

all:
	g++ $(CFLAGS) main.cpp jmpexceptions.cpp -o main

clean:
	rm -rf *.o main
