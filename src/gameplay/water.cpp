#include "gameplay/water.h"

#include "glm/gtc/matrix_transform.hpp"

Water::Water() {
	GLuint indexBuffer[6] = {
		0, 1, 2, 3, 4, 5
	};
	GLfloat yInterval = 1.0f / 3.0f;
	GLfloat vertBuffer[30] = {
		-1.0f, 0.5f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 1.0f, 32.0f, 32.0f,
		1.0f, 0.5f, -1.0f, 32.0f, 0.0f,
		1.0f, 0.5f, 1.0f, 32.0f, 32.0f,
		-1.0f, 0.5f, -1.0f, 0.0f, 0.0f,
		-1.0f, 0.5f, 1.0f, 0.0f, 32.0f
	};
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), vertBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
Water::~Water() {
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &indexID);
}
void Water::render(float x, float y, float z, glm::vec3 light, glm::vec3 dark, glm::mat4 viewProjection, glm::mat4 view, Shader* shader) {
	viewProjection = glm::translate(viewProjection, glm::vec3(-x, 0.0f, -z));
	viewProjection = glm::scale(viewProjection, glm::vec3(64.0f, 1.0f, 64.0f));
	view = glm::translate(view, glm::vec3(-x, 0.0f, -z));
	view = glm::scale(view, glm::vec3(64.0f, 1.0f, 64.0f));
	shader->setMatrix4f("modelViewPerspective", viewProjection);
	shader->setMatrix4f("modelView", view);
	shader->setUniform3f("light", light.x, light.y / 4.0f, light.z / 4.0f);
	shader->setUniform3f("dark", dark.x, dark.y, dark.z);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}