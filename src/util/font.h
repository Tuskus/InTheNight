#ifndef FONT_H_
#define FONT_H_

#include <GL/glew.h>
#include "util/shader.h"

class Font {
public:
	Font();
	~Font();
	void render(char* str, float x, float y, Shader* shader);
	void render(char* str, int maxIndex, float x, float y, Shader* shader);
private:
	Texture* texture;
	GLuint vaoID, vboID, indexID;
};

#endif
