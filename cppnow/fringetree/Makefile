main : main.o fringetree.o
	g++-11 -o main $<

prepend : prepend.o fringetree.o
	g++-11 -o prepend $<

%.o : %.cpp
	g++-11 -I.. -fPIC -fmodules-ts -std=c++20 -o $@ -c $<

main.o : gcm.cache/smd.fingetree.gcm
prepend.o : gcm.cache/smd.fingetree.gcm

gcm.cache/smd.fingetree.gcm:  fringetree.o
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
