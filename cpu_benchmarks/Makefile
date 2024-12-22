CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)

all: timingoverhead loopoverhead syscalloverhead procedurecalloverhead processcreateoverhead threadcreateoverhead threadctxswitch processctxswitch

processctxswitch: process_contextswitch.o 
	$(CXX) -o ../bin/processctxswitch process_contextswitch.o 

process_contextswitch.o: process_contextswitch.cpp 
	$(CXX) -c process_contextswitch.cpp

threadctxswitch: thread_contextswitch.o 
	$(CXX) -o ../bin/threadctxswitch thread_contextswitch.o 

thread_contextswitch.o: thread_contextswitch.cpp 
	$(CXX) -c thread_contextswitch.cpp

threadcreateoverhead: threadcreation.o 
	$(CXX) -o ../bin/threadcreateoverhead threadcreation.o 

threadcreation.o: threadcreation.cpp 
	$(CXX) -c -lpthread threadcreation.cpp

processcreateoverhead: processcreationoverhead.o 
	$(CXX) -o ../bin/processcreateoverhead processcreationoverhead.o 

processcreationoverhead.o: processcreationoverhead.cpp 
	$(CXX) -c processcreationoverhead.cpp

procedurecalloverhead: procedurecalloverhead.o 
	$(CXX) -o ../bin/procedurecalloverhead procedurecalloverhead.o 

procedurecalloverhead.o: procedurecalloverhead.cpp 
	$(CXX) -c procedurecalloverhead.cpp

syscalloverhead: syscalloverhead.o 
	$(CXX) -o ../bin/syscalloverhead syscalloverhead.o 

syscalloverhead.o: syscalloverhead.cpp 
	$(CXX) -c syscalloverhead.cpp

timingoverhead: timingoverhead.o 
	$(CXX) -o ../bin/timingoverhead timingoverhead.o 

timingoverhead.o: timingoverhead.cpp 
	$(CXX) -c timingoverhead.cpp

loopoverhead: loopoverhead.o 
	$(CXX) -o ../bin/loopoverhead loopoverhead.o 

loopoverhead.o: loopoverhead.cpp 
	$(CXX) -c loopoverhead.cpp

clean:
	$(RM) *.o *.gch
	$(RM) ../bin/*