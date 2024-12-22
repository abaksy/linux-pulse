CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)

all: filecache fileread

filecache: filecache_size.o
	$(CXX) -o ../bin/filecache_size filecache_size.o

filecache_size.o: filecache_size.cpp
	head -c 16G </dev/urandom >../bin/temp_file
	$(CXX) -c filecache_size.cpp

fileread: fileread.o
	$(CXX) -o ../bin/fileread fileread.o

fileread.o: fileread.cpp
	$(CXX) -c fileread.cpp

remote_fileread: remote_fileread.o
	$(CXX) -o ../bin/remote_fileread remote_fileread.o

remote_fileread.o: remote_fileread.cpp
	$(CXX) -c remote_fileread.cpp

contention: contention.o
	$(CXX) -o ../bin/contention contention.o

contention.o: contention.cpp
	$(CXX) -c contention.cpp

clean:
	$(RM) *.o *.gch
	$(RM) ../bin/*
