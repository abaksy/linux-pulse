CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)

all: memaccesstime membandwidth pagefault

pagefault: pagefault_time.o
	$(CXX) -o ../bin/pagefault pagefault_time.o

pagefault_time.o: pagefault_time.cpp
	head -c 4G </dev/urandom >../bin/temp_file
	$(CXX) -c pagefault_time.cpp

membandwidth: mem_bandwidth.o
	$(CXX) -o ../bin/membandwidth mem_bandwidth.o

mem_bandwidth.o: mem_bandwidth.cpp
	$(CXX) -c mem_bandwidth.cpp

memaccesstime: mem_access_time.o 
	$(CXX) -o ../bin/memaccesstime mem_access_time.o

mem_access_time.o: mem_access_time.cpp 
	$(CXX) -c mem_access_time.cpp

clean:
	$(RM) *.o *.gch
	$(RM) ../bin/*
