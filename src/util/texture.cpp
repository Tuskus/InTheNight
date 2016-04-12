#include "util/texture.h"

#include "util/SOIL/SOIL.h"

Texture::Texture(char* filename) {
	texID = SOIL_load_OGL_texture(filename, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
Texture::~Texture() {
	glDeleteTextures(1, &texID);
}
GLuint Texture::getID() {
	return texID;
}
