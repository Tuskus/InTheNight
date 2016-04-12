#include "gameplay/player.h"

#include <math.h>
#include "glm/gtc/matrix_transform.hpp"
#include "actor/actor.h"
#include "game.h"

#define PI 3.14159265f

Player::Player(float startX, float startY, float angle, int map) {
	mapNum = map;
	moveForward = false;
	moveBack = false;
	moveLeft = false;
	moveRight = false;
	xRotation = 0.0f;
	yRotation = angle;
	height = 0.0f;
	bobOffset = 0.0f;
	bobTime = 0;
	for (int i = 0; i < 5; i++) {
		runes[i] = false;
	}
	numRunes = 0;
	footstepSound = new Sound("data/sfx/footstep.wav");
	wetstepSound = new Sound("data/sfx/wetstep.wav");
	hitbox = new Circle(startX, startY, 0.125f);
	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspectiveFov(PI / 3.0f, (float) getWidth() / getHeight(), 1.0f, 0.1f, 180.0f);
}
Player::~Player() {
	delete footstepSound;
	delete hitbox;
}
void Player::setY(float newY) {
	height = newY;
}
float Player::getX() {
	return hitbox->getX();
}
float Player::getY() {
	return height;
}
float Player::getZ() {
	return hitbox->getY();
}
float Player::getRotation() {
	return yRotation;
}
Circle* Player::getHitbox() {
	return hitbox;
}
glm::mat4 Player::getViewMatrix() {
	return viewMatrix;
}
int Player::getMap() {
	return mapNum;
}
void Player::setMap(int num) {
	mapNum = num;
}
void Player::setRuneCollected(int runeNum) {
	if (!runes[runeNum]) {
		numRunes++;
	}
	runes[runeNum] = true;
}
int Player::getNumRunes() {
	return numRunes;
}
glm::mat4 Player::getViewProjectionMatrix() {
	return projectionMatrix * viewMatrix;
}
void Player::stopMovements() {
	moveForward = false;
	moveBack = false;
	moveLeft = false;
	moveRight = false;
}
void Player::checkKeyInput(int key, int action) {
	switch (key) {
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS) {
			moveForward = true;
		} else if (action == GLFW_RELEASE) {
			moveForward = false;
		}
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS) {
			moveBack = true;
		} else if (action == GLFW_RELEASE) {
			moveBack = false;
		}
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS) {
			moveLeft = true;
		} else if (action == GLFW_RELEASE) {
			moveLeft = false;
		}
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS) {
			moveRight = true;
		} else if (action == GLFW_RELEASE) {
			moveRight = false;
		}
		break;
	default:
		break;
	}
}
void Player::checkMouseInput(float deltaX, float deltaY) {
	xRotation += deltaY * 0.0006f;
	if (xRotation > PI / 2.0f) {
		xRotation = PI / 2.0f;
	} else if (xRotation < -PI / 2.0f) {
		xRotation = -PI / 2.0f;
	}
	yRotation += deltaX * 0.0006f;
	if (yRotation >= 2.0f * PI) {
		yRotation -= 2.0f * PI;
	} else if (yRotation < 0.0f) {
		yRotation += 2.0f * PI;
	}
}
void Player::update(Map* map, vector<Actor*> actors, int delta) {
	move(map->getWidth(), map->getHeight(), actors, delta);
}
void Player::move(int w, int h, vector<Actor*> actors, int delta) {
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	float angleOffset = 0.0f;
	bool moving = false;
	if (moveForward) {
		moving = true;
		if (moveRight) {
			angleOffset = PI * 0.75f;
		} else if (moveLeft) {
			angleOffset = PI / 4.0f;
		} else {
			angleOffset = PI / 2.0f;
		}
	} else if (moveBack) {
		moving = true;
		if (moveRight) {
			angleOffset = -PI * 0.75f;
		} else if (moveLeft) {
			angleOffset = -PI / 4.0f;
		} else {
			angleOffset = -PI / 2.0f;
		}
	} else if (moveRight) {
		moving = true;
		angleOffset = PI;
	} else if (moveLeft) {
		moving = true;
	}
	if (moving) {
		deltaX = (float) cos(yRotation + angleOffset);
		deltaY = (float) sin(yRotation + angleOffset);
		deltaX *= 0.00016f * delta;
		deltaY *= 0.00016f * delta;
		hitbox->addVelocity(deltaX, deltaY);
	}
	hitbox->move(w, h, delta);
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		(*it)->collidingWith(this);
	}
}
void Player::updateMatrix(int delta) {
	viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::rotate(viewMatrix, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	if (fabs(hitbox->getVelocityX()) > 0.0064f || fabs(hitbox->getVelocityY()) > 0.0064f) {
		int prevBob = bobTime;
		bobTime += delta;
		if (prevBob / 1000 < bobTime / 1000 || prevBob == 0) {
			if (height > -1.25f) {
				wetstepSound->play(randomFloat() * 0.5f + 0.85f);
			} else {
				footstepSound->play(randomFloat() * 0.5f + 0.85f);
			}
		}
		bobOffset = fabs(sin(bobTime / 318.0f)) * -0.25f;
	} else {
		bobOffset += delta / 1000.0f;
		if (bobOffset > 0.0f) {
			bobOffset = 0.0f;
			bobTime = 0;
		}
	}
	viewMatrix = glm::translate(viewMatrix, glm::vec3(hitbox->getX(), height + bobOffset, hitbox->getY()));
}