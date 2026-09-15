#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "minecraft_entities.h"

//Player Entity Struct (Doubly Linked List)

struct Player {
	char name[64];		//Player name
	float x, y, z;			//Coordinates of player
	int health;			//Health
	struct Player* next;	//Forward link
	struct Player* prev;	//Backward link
};

struct Player* world_head = NULL;

//Find player by name (helper)

struct Player* findPlayerByName(char* name) { // helper func that helps search LL for a player name, if found, return the "Player" struct (node), else return NULL
	struct Player* temp =  world_head;
	while (temp != NULL) { // means when temp has something in it, we want to keep iterating thru the LL until we find the desired name or reach of end of the LL 
		if (strcmp(temp->name, name) == 0) { //strcmp is used to compare strings, returns 0 if equal
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

// Game functions (called by the game engine)

// Spawn Player (called when a player joins the world)

void spawnPlayer(char* name, float x, float y, float z) { // Search for the player in the linked list by name, if found, return, else create a new a new player and add it to the end of the LL
	struct Player* newPlayer = (struct Player*)malloc(sizeof(struct Player));
	// malloc() allocates memory for a new player,
	// sizeof is your measuring tape for memory,
	// telling the compiler how much memory to allocate for a datatype/variable,
	// (in this case, the "Player" struct)
	// We need sizeof to be accurate about how much memory to allocate for the new player,
	// because the Player struct contains multiple data types (char array, floats, int, and pointers).

	strcpy(newPlayer->name, name); // strcpy copies the string from the previous name to the new player name
	newPlayer->x = x;              // we copy the string name to the new name because want to store the new name
	newPlayer->y = y;
	newPlayer->z = z;
	newPlayer->health = 100;
	newPlayer->next = NULL; // sets the value of the NEXT pointer to nothing (NULL) because we're adding a new player to the end of the list
	newPlayer->prev = NULL; // sets the value of the PREV pointer to nothing (NULL) because we're adding a new player to the end of the list

	// Insert at the end of the list
	if (world_head == NULL) { // basic check if the world_head is NULL so we don't run into a segmentation fault (segfault), 
		world_head = newPlayer; // which is a way of saying "you tried to access memory that doesn't exist" (in this case, the world_head is NULL, so we can't access it)"
		printf("> %s spawned in the world.\n", name);
		return;
	}
	 struct Player* temp = world_head;
	 while (temp->next != NULL) {
		 temp = temp->next;   
	}
	 temp->next = newPlayer;
	 newPlayer->prev = temp;
	 printf("Player spawned : %s at(%.1f, %.1f, %.1f)\n" , name, x, y, z);
}

// Despawn a player (called when a player dies or leaves)

void despawnPlayer(char* name) { // Search for the player by name in the linked list and remove them from the world
	struct Player* temp = world_head; // 

	while (temp != NULL && strcmp(temp->name, name) != 0) {
		temp = temp->next;
	}
	if (temp == NULL) {
		printf("Player %s not found in the world.\n", name);
		return;
	}

	if (temp->prev != NULL) {
		temp->prev->next = temp->next;
	}
	else {
		world_head = temp->next; // Update head if first player is removed
	}

	if (temp->next != NULL) {
		temp->next->prev = temp->prev;
	}
	free(temp);
	printf("Player %s has been despawned from the world.\n", name);
}

// Damage Player (called when a player takes damage, lowers health and triggers death->new node and temp)

void damagePlayer(char* name, int damage) {
	struct Player* target = findPlayerByName(name);

	if (target == NULL) {
		printf("! Player %s not found in the world.\n", name);
		return;
	}

	target->health -= damage;
	printf("> Player %s took %d damage. Health is now %d.\n", target->name, damage, target->health);

	if (target->health <= 0) {
		printf("> %s has died!\n", target->name);
		despawnPlayer(name);
	}
}
//Update Player Position (called when a player moves)

void movePlayer(char* name, float newX, float newY, float newZ) {
	struct Player* temp = world_head;

	while (temp != NULL && strcmp(temp->name, name) != 0) { // string comparison to check if the player names match, if not, we move to the next player in the LL-> makes sure no duplicate names exist in the LL.
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("[GAME] %s not found.\n", name);
		return;
	}

	temp->x = newX; // This updates the player's world position to new coordinates (newX, newY, newZ) when they move in the world.
	temp->y = newY; // This updates the player's world position to new coordinates (newX, newY, newZ) when they move in the world.
	temp->z = newZ; // This updates the player's world position to new coordinates (newX, newY, newZ) when they move in the world.
}

// List All Players

void listPlayers() {
	struct Player* temp = world_head; // This is a temp pointer used to iterate thru the LL of players,
	printf("\n--- Players in World ---\n"); // starting from the head of the list (world_head) and moving to the next node until the end of the list. 
	if (temp == NULL) {                     // This is important because we want to print all players in the world, starting from the VERY top of the list (world_head) 
		printf("No players alive in the world.\n");
	}
	while (temp != NULL) {
		printf("> %s | HP: %d | Pos: (%.1f, %.1f, %.1f)\n",        // continued below 
			temp->name, temp->health, temp->x, temp->y, temp->z); 
		temp = temp->next; // moving the memory address over to the next node in the LL (temp->next) until all players have been printed out of the list (temp == NULL)
	}
	printf("End of List!\n");
}

// Game loop simulation (for testing purposes)

int main() {
	spawnPlayer("Top Steve", -3.90, 1.00, -2.65);
	spawnPlayer("Middle Steve", -2.50, 0.00, -1.45);
	spawnPlayer("Bottom Steve", -2.20, -1.00, -1.64);

	listPlayers();

	movePlayer("Top Steve", -0.50, 1.50, -0.50); // \n adds a blank space for readability
	printf("[GAME] Top Steve moved.\n\n"); 

	despawnPlayer("Bottom Steve");

	listPlayers();

	return 0;
}
