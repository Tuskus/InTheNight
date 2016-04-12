#include "shader.h"

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
using namespace std;

Shader::Shader(char* vertFile, char* fragFile) {
	char buffer[256];
	FILE* file = fopen(vertFile, "r");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	char* vertCode = new char[size + 1];
	vertCode[0] = '\0';
	fseek(file, 0, SEEK_SET);
	while (feof(file) == 0) {
		fgets(buffer, 256, file);
		strcat(vertCode, buffer);
	}
	fclose(file);
	file = fopen(fragFile, "r");
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	char* fragCode = new char[size + 1];
	fragCode[0] = '\0';
	fseek(file, 0, SEEK_SET);
	while (feof(file) == 0) {
		fgets(buffer, 256, file);
		strcat(fragCode, buffer);
	}
	fclose(file);

	GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
	size = strlen(vertCode);
	glShaderSource(vertID, 1, (const GLchar**) &vertCode, &size);
	glCompileShader(vertID);
	size = strlen(fragCode);
	glShaderSource(fragID, 1, (const GLchar**) &fragCode, &size);
	glCompileShader(fragID);
	programID = glCreateProgram();
	glAttachShader(programID, vertID);
	glAttachShader(programID, fragID);
	glLinkProgram(programID);
	glDeleteShader(vertID);
	glDeleteShader(fragID);
	delete[] vertCode;
	delete[] fragCode;
}
Shader::~Shader() {
	glDeleteProgram(programID);
}
void Shader::use() {
	glUseProgram(programID);
}
void Shader::setMatrix4f(char* name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &matrix[0][0]);
}
void Shader::setUniform1f(char* name, float f) {
	glUniform1f(glGetUniformLocation(programID, name), f);
}
void Shader::setUniform2f(char* name, float f1, float f2) {
	glUniform2f(glGetUniformLocation(programID, name), f1, f2);
}
void Shader::setUniform3f(char* name, float f1, float f2, float f3) {
	glUniform3f(glGetUniformLocation(programID, name), f1, f2, f3);
}
void Shader::setTexture(Texture* tex, char* name, int texNum) {
	glActiveTexture(GL_TEXTURE0 + texNum);
	glBindTexture(GL_TEXTURE_2D, tex->getID());
	glUniform1i(glGetUniformLocation(programID, name), texNum);
}
void Shader::setTextureID(GLuint texID, char* name, int texNum) {
	glActiveTexture(GL_TEXTURE0 + texNum);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(programID, name), texNum);
}