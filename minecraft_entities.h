#ifndef MINECRAFT_ENTITIES_H
#define MINECRAFT_ENTITIES_H

struct Player {
    char name[64];
    float x, y, z;
    int health;
    struct Player* next;
    struct Player* prev;
};

extern struct Player* world_head;

void spawnPlayer(char* name, float x, float y, float z);
void despawnPLayer(char* name);
void movePlayer(char* name, float newX, float newY, float newZ);
void listPlayers();
struct Player* findPlayer(char* name);
void damagePlayer(char* name, int damage);

#endif