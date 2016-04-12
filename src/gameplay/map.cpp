#include "gameplay/map.h"

#include <stdio.h>

Map::Map(char* filename) {
	unsigned char size[2];
	FILE* file = fopen(filename, "rb");
	fread(size, 2, 1, file);
	width = size[0];
	height = size[1];
	unsigned char* data = new unsigned char[width * height + 1];
	fread(data, width * height + 1, 1, file);
	fclose(file);

	numTriangles = 2 * (width - 1) * (height - 1);
	GLuint* indexBuffer = new GLuint[numTriangles * 3];
	vertBuffer = new GLfloat[numTriangles * 18];
	for (int i = 0; i < numTriangles; i++) {
		indexBuffer[i * 3] = i * 3;
		indexBuffer[i * 3 + 1] = i * 3 + 1;
		indexBuffer[i * 3 + 2] = i * 3 + 2;
		if (i % 2 == 0) {
			vertBuffer[i * 18] = i / 2 % (width - 1) * 4.0f;
			vertBuffer[i * 18 + 1] = (float) data[i / 2 + i / (width - 1) / 2] / 8.0f;
			vertBuffer[i * 18 + 2] = i / 2 / (width - 1) * 4.0f;
			vertBuffer[i * 18 + 6] = i / 2 % (width - 1) * 4.0f;
			vertBuffer[i * 18 + 7] = (float) data[i / 2 + width + i / (width - 1) / 2] / 8.0f;
			vertBuffer[i * 18 + 8] = i / 2 / (width - 1) * 4.0f + 4.0f;
			vertBuffer[i * 18 + 12] = i / 2 % (width - 1) * 4.0f + 4.0f;
			vertBuffer[i * 18 + 13] = (float) data[i / 2 + i / (width - 1) / 2 + 1] / 8.0f;
			vertBuffer[i * 18 + 14] = i / 2 / (width - 1) * 4.0f;
			glm::vec3 v1 = glm::vec3(vertBuffer[i * 18 + 6] - vertBuffer[i * 18], 
				vertBuffer[i * 18 + 7] - vertBuffer[i * 18 + 1], 
				vertBuffer[i * 18 + 8] - vertBuffer[i * 18 + 2]);
			glm::vec3 v2 = glm::vec3(vertBuffer[i * 18 + 12] - vertBuffer[i * 18], 
				vertBuffer[i * 18 + 13] - vertBuffer[i * 18 + 1], 
				vertBuffer[i * 18 + 14] - vertBuffer[i * 18 + 2]);
			glm::vec3 normal = glm::cross(v1, v2);
			vertBuffer[i * 18 + 3] = normal.x;
			vertBuffer[i * 18 + 4] = normal.y;
			vertBuffer[i * 18 + 5] = normal.z;
			vertBuffer[i * 18 + 9] = normal.x;
			vertBuffer[i * 18 + 10] = normal.y;
			vertBuffer[i * 18 + 11] = normal.z;
			vertBuffer[i * 18 + 15] = normal.x;
			vertBuffer[i * 18 + 16] = normal.y;
			vertBuffer[i * 18 + 17] = normal.z;
		} else {
			vertBuffer[i * 18] = i / 2 % (width - 1) * 4.0f + 4.0f;
			vertBuffer[i * 18 + 1] = (float) data[i / 2 + i / (width - 1) / 2 + 1] / 8.0f;
			vertBuffer[i * 18 + 2] = i / 2 / (width - 1) * 4.0f;
			vertBuffer[i * 18 + 6] = i / 2 % (width - 1) * 4.0f;
			vertBuffer[i * 18 + 7] = (float) data[i / 2 + i / (width - 1) / 2 + width] / 8.0f;
			vertBuffer[i * 18 + 8] = i / 2 / (width - 1) * 4.0f + 4.0f;
			vertBuffer[i * 18 + 12] = i / 2 % (width - 1) * 4.0f + 4.0f;
			vertBuffer[i * 18 + 13] = (float) data[i / 2 + i / (width - 1) / 2 + width + 1] / 8.0f;
			vertBuffer[i * 18 + 14] = i / 2 / (width - 1) * 4.0f + 4.0f;
			glm::vec3 v1 = glm::vec3(vertBuffer[i * 18 + 6] - vertBuffer[i * 18],
				vertBuffer[i * 18 + 7] - vertBuffer[i * 18 + 1],
				vertBuffer[i * 18 + 8] - vertBuffer[i * 18 + 2]);
			glm::vec3 v2 = glm::vec3(vertBuffer[i * 18 + 12] - vertBuffer[i * 18],
				vertBuffer[i * 18 + 13] - vertBuffer[i * 18 + 1],
				vertBuffer[i * 18 + 14] - vertBuffer[i * 18 + 2]);
			glm::vec3 normal = glm::cross(v1, v2);
			vertBuffer[i * 18 + 3] = normal.x;
			vertBuffer[i * 18 + 4] = normal.y;
			vertBuffer[i * 18 + 5] = normal.z;
			vertBuffer[i * 18 + 9] = normal.x;
			vertBuffer[i * 18 + 10] = normal.y;
			vertBuffer[i * 18 + 11] = normal.z;
			vertBuffer[i * 18 + 15] = normal.x;
			vertBuffer[i * 18 + 16] = normal.y;
			vertBuffer[i * 18 + 17] = normal.z;
		}
	}
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * 18 * sizeof(GLfloat), vertBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTriangles * 3 * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	delete[] indexBuffer;
	delete[] data;
}
Map::~Map() {
	delete[] vertBuffer;
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &indexID);
}
GLfloat* Map::getVertBuffer() {
	return vertBuffer;
}
unsigned int Map::getWidth() {
	return width;
}
unsigned int Map::getHeight() {
	return height;
}
void Map::render(float playerX, float playerY, float playerZ, glm::vec3 light, glm::vec3 dark, glm::mat4 modelViewPerspective, glm::mat4 modelView, Shader* shader) {
	shader->setMatrix4f("modelViewPerspective", modelViewPerspective);
	shader->setMatrix4f("modelView", modelView);
	shader->setUniform3f("playerPos", playerX, playerY, playerZ);
	shader->setUniform3f("light", light.x, light.y, light.z);
	shader->setUniform3f("dark", dark.x, dark.y, dark.z);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
}