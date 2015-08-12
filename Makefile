CXX=g++
CC=gcc
CXXFLAGS= -std=c++1y -Werror -pedantic -Wall -Wextra -Wmissing-include-dirs -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wfloat-equal -Wshadow -Wcast-qual -Wcast-align -Wconversion -Wzero-as-null-pointer-constant -Wuseless-cast -Wsign-conversion -Wsized-deallocation -Wlogical-op -Wno-aggressive-loop-optimizations -Wredundant-decls -Winline -Winvalid-pch -Wdisabled-optimization -Wfatal-errors
CPPFLAGS= -I /home/vincent/coding  -I /home/vincent/coding/web_server_posixpp

all: server_cpp server_c
server_cpp: main.o http.o handle_connection.o
	g++ main.o  http.o handle_connection.o -L ../posixpp -lsocxx -o server_cpp
server_c: tinyweb.o
	gcc tinyweb.o -o server_c -O0

.PHONY: clean
clean:
	rm *.o
