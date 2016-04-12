#ifndef SILENTONE_H_
#define SILENTONE_H_

#include "actor/actor.h"

class SilentOne : public Actor {
public:
	SilentOne(float startX, float startZ, Mesh* m);
	~SilentOne();
	void collidingWith(Player* player);
	void update(int delta);
	void updateMatrix();
private:
	int timer;
	float angle, hoverOffset;
	glm::vec2 diff;
};

#endif
