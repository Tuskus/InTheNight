#ifndef ACTOR_H_
#define ACTOR_H_

#include "glm/glm.hpp"
#include "gameplay/player.h"
#include "util/mesh.h"

class Actor {
public:
	Actor(float startX, float startZ, Mesh* m);
	Actor(float startX, float startZ, float r, Mesh* m);
	virtual ~Actor();
	float getX();
	float getY();
	float getZ();
	virtual void collidingWith(Player* player);
	virtual void update(int delta);
	virtual void setY(float newY);
	virtual void updateMatrix();
	virtual void render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::mat4 viewPerspective, glm::mat4 view, Shader* shader);
protected:
	float height;
	glm::mat4 modelMatrix;
	Mesh* mesh;
	Circle* hitbox;
};

#endif
