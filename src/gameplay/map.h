#ifndef MAP_H_
#define MAP_H_

#include "util/shader.h"

class Map {
public:
	Map(char* filename);
	~Map();
	GLfloat* getVertBuffer();
	unsigned int getWidth();
	unsigned int getHeight();
	void render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::vec3 dark, glm::mat4 modelViewPerspective, glm::mat4 modelView, Shader* shader);
private:
	GLfloat* vertBuffer;
	GLuint vaoID, vboID, indexID;
	unsigned int numTriangles, width, height;
};

#endif
