CC=g++
CFLAGS=-std=c++11

.PHONY: run
run: main.o linear_algebra.hpp prog tests
	./tests
	./prog

prog: main.o linear_algebra.hpp tests
	$(CC) $(CFLAGS) -g -o $@ main.o linear_algebra.hpp

tests: tests.o linear_algebra.hpp
	$(CC) $(CFLAGS) -g -o $@ $^

main.o:
	$(CC) $(CFLAGS) -c main.cpp

tests.o:
	$(CC) $(CFLAGS) -c tests.cpp

.PHONY: clean
clean:
	rm -f *.o *.gch prog tests