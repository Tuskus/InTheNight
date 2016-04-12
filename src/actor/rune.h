#ifndef RUNE_H_
#define RUNE_H_

#include "actor/actor.h"

class Rune : public Actor {
public:
	Rune(float startX, float startZ, Mesh* runeMesh, Mesh* shadowManMesh, int num);
	~Rune();
	void collidingWith(Player* player);
	void update(int delta);
	void updateMatrix();
	void render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::mat4 viewPerspective, glm::mat4 view, Shader* shader);
private:
	int runeNum, timer;
	bool collected;
	Sound* ambientSfx;
	Sound* macAttack;
	Sound* gatheringSound;
	Mesh* shadowMesh;
};

#endif
