#include "actor/silent-one.h"

#include "glm/gtc/matrix_transform.hpp"
#include "game.h"

SilentOne::SilentOne(float startX, float startZ, Mesh* m) : Actor(startX, startZ, m) {
	timer = 0;
	angle = 0.0f;
	hoverOffset = randomFloat() * 6.28f;
}
SilentOne::~SilentOne() {
}
void SilentOne::collidingWith(Player* player) {
	diff = glm::vec2(getX() - player->getX(), getZ() - player->getZ());
	float distSquared = pow(diff.x, 2.0f) + pow(diff.y, 2.0f);
	if (distSquared < 32.0f) {
		setThreshold(distSquared / 32.0f);
	}
	if (distSquared < 0.5f) {
		player->setMap(-1);
	} else {
		if (diff.y == 0.0f) {
			angle = 0.0f;
		} else if (diff.y > 0.0f) {
			angle = atan(diff.x / diff.y) + 3.14f;
		} else {
			angle = atan(diff.x / diff.y);
		}
	}
}
void SilentOne::update(int delta) {
	timer += delta;
	diff = -glm::normalize(diff);
	diff *= 0.00008f * delta;
	hitbox->addVelocity(diff.x, diff.y);
	hitbox->move(64.0f, 64.0f, delta);
}
void SilentOne::updateMatrix() {
	Actor::updateMatrix();
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, sin(timer / 1500.0f + hoverOffset) / 8.0f + 0.25f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}