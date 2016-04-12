#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "state/state.h"

#include "util/font.h"
#include "util/shader.h"
#include "util/sound.h"

class MainMenuState : public State {
public:
	MainMenuState(bool start);
	~MainMenuState();
	void checkKeyInput(int key, int action);
	void render(int stateTime);
private:
	bool pressStart, showPrompt, wantsExit;
	int selectedOption;
	Shader* fontShader;
	Font* font;
	Sound* switchSfx;
	Sound* selectSfx;
	Sound* noiseSfx;
};

#endif
