#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "util/texture.h"

class Shader {
public:
	Shader(char* vertFile, char* fragFile);
	~Shader();
	void use();
	void setMatrix4f(char* name, glm::mat4 matrix);
	void setUniform1f(char* name, float f);
	void setUniform2f(char* name, float f1, float f2);
	void setUniform3f(char* name, float f1, float f2, float f3);
	void setTexture(Texture* tex, char* name, int texNum);
	void setTextureID(GLuint texID, char* name, int texNum);
private:
	int programID;
};

#endif
