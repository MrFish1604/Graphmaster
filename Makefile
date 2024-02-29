CC=g++
ifdef RELEASE
CFLAGS= -fPIC -Wall -O3
else
CFLAGS= -fPIC -Wall -Wextra -pedantic -fdiagnostics-color=always -O0 -g3 -fsanitize=address 
endif
CFLAGS+=-Iinclude 

bin/libgraphmaster.so: bin/graphmaster.o bin/node.o bin/array.o bin/dict.o
	g++ --shared $^ -o $@

test: bin/test.o bin/graphmaster.o bin/node.o bin/array.o bin/dict.o 
	$(CC) $(CFLAGS) $^ -o $@

bin/graphmaster.o: bin
	$(CC) $(CFLAGS) -c src/graphmaster.cpp -o $@ 

bin:
	mkdir -p bin

bin/node.o: bin 
	$(CC) $(CFLAGS) -c src/node.cpp -o $@

bin/test.o: bin 
	$(CC) -DTEST $(CFLAGS) -c src/test.cpp -o $@

bin/array.o: bin 
	$(CC) $(CFLAGS) -c src/array.cpp -o $@

lib: bin/libgraphmaster.so

install: bin/libgraphmaster.so /usr/include/libgraphmaster
	@cp -v bin/libgraphmaster.so /usr/lib/libgraphmaster.so
	@cp -v include/* /usr/include/libgraphmaster/

/usr/include/libgraphmaster:
	mkdir /usr/include/libgraphmaster

clean:
	rm -rf bin test

bin/dict.o: bin 
	$(CC) $(CFLAGS) -c src/dict.cpp -o $@
