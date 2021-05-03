all:

main : main.o fringetree.o
	g++ -o main $<

prepend : prepend.o fringetree.o
	g++ -o prepend $<

fringetree.t : fringetree.t.o fringetree.o
	g++ -o fringetree.t ./googletest/build/lib/libgtest_main.a ./googletest/build/lib/libgtest.a $<

%.o: %.cpp
	g++ -I.. -I./googletest/googletest/include/ -fPIC -fmodules-ts -std=c++20 -o $@ -c $<

main.o: gcm.cache/smd.fringtree.gcm
prepend.o: gcm.cache/smd.fringtree.gcm

gcm.cache/smd.fringtree.gcm:  fringetree.o
	@test -f $@ || rm -f $<
	@test -f $@ || $(MAKE) $<

clean:
	-rm -rf *.o gcm.cache/smd.fingetree.gcm

clean-gcm:
	rm gcm.cache/*

test:
	./main

all: main prepend

.PHONY: all test clean clean-gcm
