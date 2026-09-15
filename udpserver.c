#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr; // sockaddr_in is a struct 
    char buffer[1024];
    socklen_t addrlen = sizeof(client_addr);

    // create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed.")
        return 1;
    }

    // Configure the server address
    server_addr.sin_family = AF_INET; // sin_family sets the address type to IPv4
    server_addr.sin_port = htons(8080) // sin_port holds the port number, and htons converts 8080 to network byte order
                                       // htons -> h = Host , to = to , n = Network (big-endian byte order, stores the most significant byte at the lowest memory address)
                                       // s = Short (16-bit number, like a port)
    server_addr.sin_addr.s_addr = INADDR_ANY; // sin_addr.s_addr -> configures which IP addresses the server is allowed to listen on,
                                              // = INADDR_ANY listens on ALL available network interfaces (anyone on the internet, localhost, wifi, ethernet, anything.)

    // Bind the socket to port 8080. This allows the server to listen for data that is sent to port 8080

    if (bind(sock,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed.");
        close(sock);
        return 1;
    }

    printf("[SERVER] Listening for UDP packets on port 8080...\n");
    
    // Keep running and print anything that arrives on the socket

    while (1) {
        int len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, 
        (struct sockaddr*)&client_addr, &addr_len);
        // & is for 2 purposes: To act as the address of operator to find a variable's memory address,
        // Also serves as a bitwise AND operator, which can manipulate individuals bits to do things like set, clear, or toggle specific bits in a value.
        
        if (len > 0) {
            buffer[len] = '\0'; // backslash \ for null terminator in order to know where the string ends
            printf("[RADAR] Received: %s\n", buffer);
        }
    }
    close(sock);
    return 0;
}

