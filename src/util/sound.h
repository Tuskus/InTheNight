#ifndef SOUND_H_
#define SOUND_H_

#include <al.h>

class Sound {
public:
	Sound(char* filename);
	~Sound();
	void setPos(float angle, float playerX, float playerY, float playerZ, float sourceX, float sourceY, float sourceZ);
	void play();
	void play(float pitch);
	void loop();
	void loop(float volume);
	void stop();
private:
	unsigned int buffer, source;
};

#endif
