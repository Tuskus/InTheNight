#ifndef WATER_H_
#define WATER_H_

#include "util/shader.h"
#include "util/texture.h"

class Water {
public:
	Water();
	~Water();
	void render(float x, float y, float z, glm::vec3 light, glm::vec3 dark, glm::mat4 viewProjection, glm::mat4 view, Shader* shader);
private:
	GLuint vaoID, vboID, indexID;
};

#endif
