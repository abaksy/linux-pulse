#include "../utils.hpp"
#include "netconfig.h"
#include <sys/socket.h>
#include <arpa/inet.h>

struct timediff t;

int main() {
    set_priority(-20);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Measure connection establishment time
    tic(t);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    toc(t);
    uint64_t conn_start_time = getcycles(t.cycles_high0, t.cycles_low0);
    uint64_t conn_end_time = getcycles(t.cycles_high1, t.cycles_low1);
    uint64_t conn_total_time = conn_end_time - conn_start_time;

    // Create and send 56-byte packet
    char packet[56];
    memset(packet, 'X', sizeof(packet));

    // Buffer to receive data
    char buffer[56];

    // Measure data transmission time
    tic(t);
    
    send(sock, packet, sizeof(packet), 0);
    
    recv(sock, buffer, sizeof(buffer), 0);
    
    toc(t);
    uint64_t data_start_time = getcycles(t.cycles_high0, t.cycles_low0);
    uint64_t data_end_time = getcycles(t.cycles_high1, t.cycles_low1);
    uint64_t data_total_time = data_end_time - data_start_time;

    // Measure connection termination time
    tic(t);
    
    close(sock);
    
    toc(t);

    uint64_t teardown_start_time = getcycles(t.cycles_high0, t.cycles_low0);
    uint64_t teardown_end_time = getcycles(t.cycles_high1, t.cycles_low1);
    uint64_t teardown_total_time = teardown_end_time - teardown_start_time;

    // Print results

    std::cout << conn_total_time << " " << data_total_time << " "<< teardown_total_time << '\n';

    return 0;
}