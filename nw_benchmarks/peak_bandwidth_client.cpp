#include "../utils.hpp"
#include "netconfig.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


#define BUFFER_SIZE 1<<17
#define TEST_DURATION 20 // Test duration in seconds

struct timediff t;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, 'A', BUFFER_SIZE);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to server, starting bandwidth test...\n");

    uint64_t start_cycles, end_cycles, total_cycles;
    size_t total_bytes_sent = 0;
    size_t total_bytes_received = 0;

    double start_time = time(NULL);
    double end_time = start_time + TEST_DURATION;

    tic(t);
    while (time(NULL) < end_time) {
        ssize_t bytes_sent = send(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_sent < 0) {
            perror("Send error");
            break;
        }
        total_bytes_sent += bytes_sent;

        ssize_t bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            perror("Receive error");
            break;
        }
        total_bytes_received += bytes_received;
    }
    toc(t);
    double elapsed_time = time(NULL) - start_time;
    
    start_cycles = getcycles(t.cycles_high0, t.cycles_low0);
    end_cycles = getcycles(t.cycles_high1, t.cycles_low1);
    total_cycles = end_cycles - start_cycles;

    
    double bandwidth = ((total_bytes_sent + total_bytes_received)*8) / (total_cycles * 0.3858 * 1e-9 * 1e6); // Mbps

    printf("Test completed.\n");
    printf("Total bytes sent: %zu\n", total_bytes_sent);
    printf("Total bytes received: %zu\n", total_bytes_received);
    printf("Elapsed time: %.2f seconds\n", elapsed_time);
    printf("Elapsed CPU cycles: %lu\n", total_cycles);
    printf("Peak bandwidth: %.2f MBps\n", bandwidth);
    close(sock);
    return 0;
}
