#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "state/state.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/font.h"
#include "util/shader.h"

class OptionsState : public State {
public:
	OptionsState();
	~OptionsState();
	void checkKeyInput(int key, int action);
	void render(int stateTime);
private:
	int selectedOption, currentMode;
	Shader* fontShader;
	Font* font;
	const GLFWvidmode* modes;
	bool isRGB888(GLFWvidmode m);
	void saveOptions();
};

#endif
