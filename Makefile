CC=g++
CFLAGS=-Wall -Wextra -pedantic -fdiagnostics-color=always -O0 -g3 -fsanitize=address 
CFLAGS+=-Iinclude 

test: bin/test.o bin/graphmaster.o bin/node.o bin/array.o bin/dict.o
	$(CC) $(CFLAGS) $^ -o $@

bin/graphmaster.o: bin
	$(CC) $(CFLAGS) -c src/graphmaster.cpp -o $@ 

bin:
	mkdir -p bin

bin/node.o: bin 
	$(CC) $(CFLAGS) -c src/node.cpp -o $@

bin/test.o: bin 
	$(CC) $(CFLAGS) -c src/test.cpp -o $@

bin/array.o: bin 
	$(CC) $(CFLAGS) -c src/array.cpp -o $@

clean:
	rm -rf bin test

bin/dict.o: bin 
	$(CC) $(CFLAGS) -c src/dict.cpp -o $@
