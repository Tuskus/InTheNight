#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "gameplay/map.h"
#include "util/circle.h"
#include "util/sound.h"
using namespace std;

class Actor;

class Player {
public:
	Player(float startX, float startY, float angle, int map);
	~Player();
	void setY(float newY);
	float getX();
	float getY();
	float getZ();
	float getRotation();
	Circle* getHitbox();
	int getMap();
	void setMap(int num);
	void setRuneCollected(int runeNum);
	int getNumRunes();
	glm::mat4 getViewMatrix();
	glm::mat4 getViewProjectionMatrix();
	void stopMovements();
	void checkKeyInput(int key, int action);
	void checkMouseInput(float deltaX, float deltaY);
	void update(Map* map, vector<Actor*> actors, int delta);
	void updateMatrix(int delta);
private:
	bool moveForward, moveBack, moveLeft, moveRight;
	float xRotation, yRotation, height, bobOffset;
	int bobTime, mapNum, numRunes;
	bool runes[5];
	Sound* footstepSound;
	Sound* wetstepSound;
	Circle* hitbox;
	glm::mat4 viewMatrix, projectionMatrix;
	void move(int w, int h, vector<Actor*> actors, int delta);
};

#endif
