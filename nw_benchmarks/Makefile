CC=gcc
CXX=g++
RM=rm -f

all: tcptest peak_bw_client peak_bw_server

peak_bw_client: peak_bandwidth_client.o
	$(CXX) -o ../bin/client_peakbw peak_bandwidth_client.o

peak_bandwidth_client.o: peak_bandwidth_client.cpp 
	$(CXX) -c peak_bandwidth_client.cpp 

peak_bw_server: peak_bandwidth_server.o
	$(CXX) -o ../bin/server_peakbw peak_bandwidth_server.o

peak_bandwidth_server.o: peak_bandwidth_server.cpp 
	$(CXX) -c peak_bandwidth_server.cpp

tcptest: tcp_conn_params.o
	$(CXX) -o ../bin/tcptest tcp_conn_params.o

tcp_conn_params.o: tcp_conn_params.cpp
	$(CXX) -c tcp_conn_params.cpp

clean:
	$(RM) *.o *.gch
	$(RM) ../bin/*
