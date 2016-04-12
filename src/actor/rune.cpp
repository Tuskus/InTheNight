#include "actor/rune.h"

#include "glm/gtc/matrix_transform.hpp"
#include "game.h"

Rune::Rune(float startX, float startZ, Mesh* runeMesh, Mesh* shadowManMesh, int num) : Actor(startX, startZ, runeMesh) {
	runeNum = num;
	shadowMesh = shadowManMesh;
	timer = 0;
	collected = false;
	ambientSfx = new Sound("data/sfx/r.wav");
	macAttack = new Sound("data/bgm/0.wav");
	gatheringSound = new Sound("data/sfx/gathering.wav");
	ambientSfx->loop(getSfxVolume());
}
Rune::~Rune() {
	delete ambientSfx;
	delete macAttack;
	delete gatheringSound;
}
void Rune::collidingWith(Player* player) {
	if (collected) {
		Actor::collidingWith(player);
		gatheringSound->setPos(player->getRotation(), player->getX(), player->getY(), player->getZ(), getX(), getY(), getZ());
		if (pow(getX() - player->getX(), 2.0f) + pow(getZ() - player->getZ(), 2.0f) > 2600.0f) {
			player->setMap(-1);
		}
	} else {
		if (pow(getX() - player->getX(), 2.0f) + pow(getZ() - player->getZ(), 2.0f) < 1.0f) {
			player->setRuneCollected(runeNum);
			collected = true;
			mesh = shadowMesh;
			hitbox->setY(getZ() + 2.5f);
			ambientSfx->stop();
			macAttack->play();
			gatheringSound->setPos(player->getRotation(), player->getX(), player->getY(), player->getZ(), getX(), getY(), getZ());
			gatheringSound->loop(getSfxVolume());
		}
	}
}
void Rune::update(int delta) {
	timer += delta;
}
void Rune::updateMatrix() {
	Actor::updateMatrix();
	if (!collected) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, sin(timer / 500.0f) / 2.0f + 0.5f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, timer / 350.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}
void Rune::render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::mat4 viewPerspective, glm::mat4 view, Shader* shader) {
	Actor::render(playerX, playerY, playerZ, light, viewPerspective, view, shader);
}