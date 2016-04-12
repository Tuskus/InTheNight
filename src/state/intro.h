#ifndef INTRO_H_
#define INTRO_H_

#include "state/state.h"

#include "util/font.h"
#include "util/shader.h"
#include "util/sound.h"

class IntroState : public State {
public:
	IntroState();
	~IntroState();
	void checkKeyInput(int key, int action);
	void update(int delta);
	void render(int stateTime);
private:
	int timer, nextStep;
	Shader* fontShader;
	Font* font;
	Sound* typeSound;
};

#endif
