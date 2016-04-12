#include "actor/actor.h"

#include "glm/gtc/matrix_transform.hpp"

Actor::Actor(float startX, float startZ, Mesh* m) {
	hitbox = new Circle(startX, startZ, 0.5f);
	mesh = m;
	modelMatrix = glm::mat4(0.25f);
}
Actor::Actor(float startX, float startZ, float r, Mesh* m) {
	hitbox = new Circle(startX, startZ, r);
	mesh = m;
	modelMatrix = glm::mat4(1.0f);
}
Actor::~Actor() {
	delete hitbox;
}
float Actor::getX() {
	return hitbox->getX();
}
float Actor::getY() {
	return height;
}
float Actor::getZ() {
	return hitbox->getY();
}
void Actor::collidingWith(Player* player) {
	hitbox->checkCollision(player->getHitbox());
}
void Actor::update(int delta) {
}
void Actor::setY(float newY) {
	height = newY;
}
void Actor::updateMatrix() {
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-getX(), -height - 1.0f, -getZ()));
}
void Actor::render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::mat4 viewPerspective, glm::mat4 view, Shader* shader) {
	mesh->render(playerX, playerY, playerZ, light, viewPerspective * modelMatrix, view * modelMatrix, shader);
}