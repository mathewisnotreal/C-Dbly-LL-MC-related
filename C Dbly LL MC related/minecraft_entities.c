#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

struct Player* findPlayerByName(char* name) {
	struct Player* temp =  world_head;
	while (temp != NULL) {
		if (strcmp(temp->name, name) == 0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

// Game functions (called by the game engine)

// Spawn Player (called when a player joins the world)

void spawnPlayer(char* name, float x, float y, float z) {
	struct Player* newPlayer = (struct Player*)malloc(sizeof(struct Player));

	strcpy(newPlayer->name, name);
	newPlayer->x = x;
	newPlayer->y = y;
	newPlayer->z = z;
	newPlayer->health = 100;
	newPlayer->next = NULL;
	newPlayer->prev = NULL;

	// Insert at the end of the list
	if (world_head == NULL) {
		world_head = newPlayer;
		printf("> %s spawned in the world.\n", name);
		return;
	}
	 struct Player* temp = world_head;
	 while (temp->next != NULL) {
		 temp = temp->next;   
	}
	 temp->next = newPlayer;
	 newPlayer->prev = temp;
	 printf("Player spawned : % s at(% .1f, % .1f, % .1f)\n" , name, x, y, z);
}

// Despawn a player (called when a player dies or leaves)

void despawnPlayer(char* name) {
	struct Player* temp = world_head;

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

	while (temp != NULL && strcmp(temp->name, name) != 0) {
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("[GAME] %s not found.\n", name);
		return;
	}

	temp->x = newX;
	temp->y = newY;
	temp->z = newZ;
}

// List All Players

void listPlayers() {
	struct Player* temp = world_head;
	printf("\n--- Players in World ---\n");
	if (temp == NULL) {
		printf("No players alive in the world.\n");
	}
	while (temp != NULL) {
		printf("> %s | HP: %d | Pos: (%.1f, %.1f, %.1f)\n",        // continued below 
			temp->name, temp->health, temp->x, temp->y, temp->z);
		temp = temp->next;
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
