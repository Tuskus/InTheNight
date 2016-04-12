#ifndef BUILDER_H_
#define BUILDER_H_

#include "actor/actors.h"

#define NUM_ACTORS 4

vector<Mesh*> mesh;
int meshID[NUM_ACTORS];

void initBuilder() {
	for (int i = 0; i < NUM_ACTORS; i++) {
		meshID[i] = -1;
	}
}
void destroyBuilder() {
	for (vector<Mesh*>::iterator it = mesh.begin(); it != mesh.end(); it++) {
		delete (*it);
	}
	mesh.clear();
}
Actor* buildActor(int actorID, float x, float y) {
	Actor* actor;
	switch (actorID) {
	case 0:
		if (meshID[0] == -1) {
			mesh.push_back(new Mesh("data/mesh/s.obj"));
			meshID[0] = mesh.size() - 1;
		}
		actor = new Rune(x * -4.0f, y * -4.0f, mesh.at(meshID[0]), mesh.at(meshID[1]), 5);
		break;
	case 1:
		if (meshID[1] == -1) {
			mesh.push_back(new Mesh("data/mesh/s.obj"));
			meshID[1] = mesh.size() - 1;
		}
		actor = new Rune(x * -4.0f, y * -4.0f, mesh.at(meshID[1]), mesh.at(meshID[1]), 5);
		break;
	case 2:
		if (meshID[2] == -1) {
			mesh.push_back(new Mesh("data/mesh/rp.obj"));
			meshID[2] = mesh.size() - 1;
		}
		if (meshID[3] == -1) {
			mesh.push_back(new Mesh("data/mesh/s.obj"));
			meshID[3] = mesh.size() - 1;
		}
		actor = new Rune(x * -4.0f, y * -4.0f, mesh.at(meshID[2]), mesh.at(meshID[3]), 0);
		break;
	default:
		actor = NULL;
		break;
	}
	return actor;
}

#endif
