#include "util/font.h"

#include <string.h>

Font::Font() {
	texture = new Texture("data/tex/menu/font.png");
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	int j = 0;
	float texCoordX = 1.0f / 12.0f;
	float texCoordY = 0.125f;
	GLfloat vertBuffer[24] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.07f, 0.05f, texCoordX, texCoordY,
		0.0f, 0.05f, 0.0f, texCoordY,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.07f, 0.0f, texCoordX, 0.0,
		0.07f, 0.05f, texCoordX, texCoordY
	};
	GLuint indexBuffer[6] = {
		0, 1, 2, 3, 4, 5
	};
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, vertBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*) (sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indexBuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
Font::~Font() {
	delete texture;
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &indexID);
}
void Font::render(char* str, float x, float y, Shader* shader) {
	render(str, strlen(str), x, y, shader);
}
void Font::render(char* str, int maxIndex, float x, float y, Shader* shader) {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);

	shader->setTexture(texture, "diffuseTex", 0);
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	for (int i = 0; i < maxIndex; i++) {
		if (str[i] == '\n') {
			xOffset = i * -0.084f - 0.084f;
			yOffset -= 0.15f;
		} else if (str[i] != ' ') {
			shader->setUniform2f("offset", x + i * 0.084f + xOffset, y + yOffset);
			shader->setUniform2f("texOffset", (str[i] - '!') % 12 / 12.0f, (str[i] - '!') / 12 / -8.0f - 0.125f);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}