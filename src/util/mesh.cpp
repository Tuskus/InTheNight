#include "util/mesh.h"

#include <stdio.h>
#include <string.h>

Mesh::Mesh(char* filename) {
	char buffer[128];
	FILE* file = fopen(filename, "r");
	vector<glm::vec3> verts;
	vector<glm::vec3> normals;
	vector<unsigned int> vertIndex;
	vector<unsigned int> normalIndex;
	colorIndex.push_back(0);
	while (feof(file) == 0) {
		fscanf(file, "%s", buffer);
		if (buffer[0] == 'v') {
			if (buffer[1] == 'n') {
				glm::vec3 normal;
				fscanf(file, " %f %f %f\n", &normal.x, &normal.y, &normal.z);
				normal = glm::normalize(normal);
				normals.push_back(normal);
			} else {
				glm::vec3 vert;
				fscanf(file, " %f %f %f\n", &vert.x, &vert.y, &vert.z);
				verts.push_back(vert);
			}
		} else if (buffer[0] == 'f') {
			unsigned int vi[3];
			unsigned int ni[3];
			fscanf(file, "%d//%d %d//%d %d//%d\n", &vi[0], &ni[0], &vi[1], &ni[1], &vi[2], &ni[2]);
			vertIndex.push_back(vi[0]);
			vertIndex.push_back(vi[1]);
			vertIndex.push_back(vi[2]);
			normalIndex.push_back(ni[0]);
			normalIndex.push_back(ni[1]);
			normalIndex.push_back(ni[2]);
		} else if (buffer[0] == 'c') {
			glm::vec3 c;
			fscanf(file, "%f %f %f\n", &c.x, &c.y, &c.z);
			colors.push_back(c);
			colorIndex.push_back(vertIndex.size());
		}
	}
	fclose(file);
	indexCount = vertIndex.size();
	float* vertBuffer = new float[indexCount * 6];
	unsigned int* indexBuffer = new unsigned int[indexCount];
	for (unsigned int i = 0; i < indexCount; i++) {
		vertBuffer[i * 6] = verts.at(vertIndex.at(i) - 1).x;
		vertBuffer[i * 6 + 1] = verts.at(vertIndex.at(i) - 1).y;
		vertBuffer[i * 6 + 2] = verts.at(vertIndex.at(i) - 1).z;
		vertBuffer[i * 6 + 3] = normals.at(normalIndex.at(i) - 1).x;
		vertBuffer[i * 6 + 4] = normals.at(normalIndex.at(i) - 1).y;
		vertBuffer[i * 6 + 5] = normals.at(normalIndex.at(i) - 1).z;
		indexBuffer[i] = i;
	}
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, indexCount * 6 * sizeof(GLfloat), vertBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	delete[] vertBuffer;
	delete[] indexBuffer;
}
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &indexID);
}
void Mesh::render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::mat4 modelViewPerspective, glm::mat4 modelView, Shader* shader) {
	shader->setMatrix4f("modelViewPerspective", modelViewPerspective);
	shader->setMatrix4f("modelView", modelView);
	shader->setUniform3f("playerPos", playerX, playerY, playerZ);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	for (int i = 0; i < colorIndex.size() - 1; i++) {
		shader->setUniform3f("light", colors.at(i).x * 2.0f, colors.at(i).y * 2.0f, colors.at(i).z * 2.0f);
		shader->setUniform3f("dark", light.x * colors.at(i).x, light.y * colors.at(i).y, light.z * colors.at(i).z);
		glDrawArrays(GL_TRIANGLES, colorIndex.at(i), colorIndex.at(i + 1) - colorIndex.at(i));
	}
}