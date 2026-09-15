#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include <unistd.h>
#include "minecraft_entities.h"

// Calculate distance between two points in 3D space
float getDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    float dz = z1 - z2;
    return sqrtf((dx * dx ) + (dy * dy) + (dz * dz));
}

struct Player* findClosestPlayer(float myX, float myY, float myZ) {
    struct Player* temp = world_head;
    struct Player* closest = NULL;
    float closestDist = -1.0f;

    // Distance condition if a player exists loop
    while (temp != NULL) {
        float dist = getDistance(myX, myY, myZ, temp->x, temp->y, temp->z);

        if (closest == NULL || dist < closestDist) {
            closest = temp;
            closestDist = dist;
        }
        temp = temp->next;
    }
    return closest;

}

void sendPlayerDataOverUDP() {
    int sock;
    struct sockaddr_in server_addr; // sockaddr_in is a struct type defined in the <arpa/inet.h> library, specifically for IPv4 addresses.
                                    // Declares a variable named server_addr
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("SOcket creation failed");
        return;
    }

    server_addr.sin_family = AF_INET; // AF is the address family, INET is the internet protocol (ip)
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

     // Example coordinates for the player
    float myX = 0.0, myY = 1.0, myZ = 0.0;



    struct Player* target = findClosestPlayer(myX, myY, myZ);

    // Seeing if the current target exists, if it DOES, we send the data over UDP to the server, but if NOT, we print a message saying that no players were found to send data

    if (target != NULL) {
        sprintf(message, "%s: %.1f %.1f %.1f", target->name, target->x, target->y, target->z);
        
        sendto(sock, message, strlen(message), 0, // strlen(message) is the length of the message to be sent,
            (struct sockaddr*)&server_addr, sizeof(server_addr));
        
        printf("[CLIENT] Sent: %s\n", message);
    } else {
        printf("[CLIENT] No players found to send data.\n");
    }

    close(sock);
}

int main() {
    // Spawn players (simulated game data)
    spawnPlayer("Steve", 100.0, 64.0, 200.0);
    spawnPlayer("Alex", 120.0, 64.0, 250.0);
    spawnPlayer("Dream", 80.0, 64.0, 180.0);

    listPlayers();

    printf("\n--- Sending closest target to Radar Server ---\n");
    sendPlayerDataOverUDP();

    return 0;
}