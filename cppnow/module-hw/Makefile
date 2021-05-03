main : main.o hello.o
	g++ -o main main.o hello.o

main.o : main.cpp gcm.cache/smd.hello.gcm
	g++ -fPIC -fmodules-ts -std=c++20 -o main.o -c main.cpp

hello.o: hello.cpp
	g++ -fPIC -fmodules-ts -std=c++20 -o hello.o -c hello.cpp

gcm.cache/smd.hello.gcm:  hello.o
	@test -f $@ || rm -f $<
	@test -f $@ || $(MAKE) $<

clean:
	rm hello.o main.o gcm.cache/smd.hello.gcm

clean-gcm:
	rm gcm.cache/smd.hello.gcm

test:
	./main

all: main
