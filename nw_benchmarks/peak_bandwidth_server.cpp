#include "../utils.hpp"
#include "netconfig.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

/*
Remote
Connected to server, starting bandwidth test...
Test completed.
Total bytes sent: 16252928
Total bytes received: 14182399
Elapsed time: 20.00 seconds
Elapsed CPU cycles: 50892296896
Peak bandwidth: 12.40 Mbps
*/

/*
Localhost
Connected to server, starting bandwidth test...
Test completed.
Total bytes sent: 52984807424
Total bytes received: 52974387147
Elapsed time: 20.00 seconds
Elapsed CPU cycles: 49398584024
Peak bandwidth: 44478.68 Mbps
*/

#define BUFFER_SIZE 1<<17

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted, echoing data...\n");

    while (1) {
        ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            break; // Connection closed
        }
        // Echo back data
        write(new_socket, buffer, bytes_read);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
