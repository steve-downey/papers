client : client.o math.o
	clang++ -std=c++2a -stdlib=libc++ -fmodules -fprebuilt-module-path=. -fimplicit-modules *.o -o client

client.o : client.cpp math.pcm
	clang++ -std=c++2a -stdlib=libc++ -fmodules -fprebuilt-module-path=. -fimplicit-modules -c  client.cpp -o client.o

math.o: math.cpp
	clang++ -std=c++2a -stdlib=libc++ -fmodules -fprebuilt-module-path=. -fimplicit-modules -c  math.cpp -o math.o

math.pcm : math.cpp
	clang++ -std=c++2a -stdlib=libc++ -c math.cpp -fmodules   -Xclang -emit-module-interface -o math.pcm

clean:
	rm math.o client.o math.pcm

clean-gcm:
	rm math.pcm

test:
	./client

all: client
