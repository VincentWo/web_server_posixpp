all: server_cpp server_c
server_cpp: tinyweb-cpp.o http.o
	g++ tinyweb-cpp.o  http.o -L ../Socket -lsocxx -o server_cpp
server_c: tinyweb-c.o
	gcc tinyweb-c.o -o server_c -O0

tinyweb-c.o: tinyweb.c
	gcc tinyweb.c -c -o tinyweb-c.o -O0 -g

tinyweb-cpp.o: tinyweb.cpp
	g++ tinyweb.cpp -c -o tinyweb-cpp.o -O0 -std=c++1y -g

http.o: http.cpp
	g++ http.cpp -c -o http.o -O0 -std=c++1y -g

.PHONY: clean
clean:
	rm *.o
