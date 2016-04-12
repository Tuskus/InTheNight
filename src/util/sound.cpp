#include "util/sound.h"
#define __MINGW32__
#include <AL/alut.h>
#include "game.h"

Sound::Sound(char* filename) {
	alGenBuffers(1, &buffer);
	ALenum format;
	ALsizei size;
	ALsizei freq;
	ALboolean loop;
	ALvoid* data;
	alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
}
Sound::~Sound() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}
void Sound::setPos(float angle, float playerX, float playerY, float playerZ, float sourceX, float sourceY, float sourceZ) {
	float s, c;
	float soundX = sourceX - playerX;
	float soundZ = sourceZ - playerZ;
	if ((soundX > 0.0f && soundZ > 0.0f) || (soundX < 0.0f && soundZ < 0.0f)) {
		s = sin(-angle - 3.14f);
		c = cos(-angle - 3.14f);
	} else {
		s = sin(angle - 1.57f);
		c = cos(angle - 1.57f);
	}
	alSource3f(source, AL_POSITION, soundX * c - soundZ * s, sourceY - playerY, soundX * s + soundZ * c);
}
void Sound::play() {
	alSourcef(source, AL_GAIN, getSfxVolume());
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcePlay(source);
}
void Sound::play(float pitch) {
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, getSfxVolume());
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcePlay(source);
}
void Sound::loop() {
	alSourcef(source, AL_GAIN, getBgmVolume() * 0.32f);
	alSourcei(source, AL_LOOPING, AL_TRUE);
	alSourcePlay(source);
}
void Sound::loop(float volume) {
	alSourcef(source, AL_GAIN, volume);
	alSourcei(source, AL_LOOPING, AL_TRUE);
	alSourcePlay(source);
}
void Sound::stop() {
	alSourceStop(source);
}
