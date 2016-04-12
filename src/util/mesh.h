#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <GL/glew.h>
#include "util/shader.h"
using namespace std;

class Mesh {
public:
	Mesh(char* filename);
	~Mesh();
	void render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::mat4 viewPerspective, glm::mat4 view, Shader* shader);
private:
	int indexCount;
	GLuint vaoID, vboID, indexID;
	vector<int> colorIndex;
	vector<glm::vec3> colors;
};

#endif
