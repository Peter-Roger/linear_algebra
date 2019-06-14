CC=g++
CFLAGS=-std=c++11

prog: main.o tests
	$(CC) -o $@ $(CFLAGS) main.o
	./tests

tests: tests.o
	$(CC) -o $@ $(CFLAGS) $^

main.o:
	$(CC) $(CFLAGS) -c main.cpp

tests.o:
	$(CC) $(CFLAGS) -c tests.cpp

.PHONY: clean
clean:
	rm -f *.o *.gch prog tests