#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>

class Texture {
public:
	Texture(char* filename);
	~Texture();
	GLuint getID();
private:
	GLuint texID;
};

#endif
