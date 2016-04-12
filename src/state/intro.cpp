#include "state/intro.h"

#include <GLFW/glfw3.h>
#include "game.h"
#include "state/state-enum.h"

#define LOGO_END 5000
#define GANBARE_STR "GANBARE"
#define GAMES_STR "GAMES"

IntroState::IntroState() {
	timer = 0;
	nextStep = 250;
	fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
	font = new Font();
	typeSound = new Sound("data/sfx/menu/type.wav");
}
IntroState::~IntroState() {
	delete fontShader;
	delete font;
	delete typeSound;
}
void IntroState::checkKeyInput(int key, int action) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE || key == GLFW_KEY_ENTER) {
			if (timer < 3250) {
				timer = 3250;
			} else {
				timer = LOGO_END;
			}
		}
	}
}
void IntroState::update(int delta) {
	timer += delta;
	if (timer < 3250) {
		if (nextStep <= timer) {
			typeSound->play();
			nextStep += 250;
		}
	}
	if (timer >= LOGO_END) {
		setState(MAIN_MENU);
	}
}
void IntroState::render(int stateTime) {
	fontShader->use();
	if (timer < LOGO_END) {
		if (timer > 1750) {
			if (timer > LOGO_END - 250) {
				font->render(GANBARE_STR, -0.338f, 0.27f, fontShader);
			} else {
				font->render(GANBARE_STR, -0.338f, 0.27f, fontShader);
			}
		} else {
			font->render(GANBARE_STR, timer / 250, -0.338f, 0.27f, fontShader);
		}
		if (timer > 3000) {
			if (timer > LOGO_END - 250) {
				font->render(GAMES_STR, -0.075f, 0.17f, fontShader);
			} else {
				font->render(GAMES_STR, -0.075f, 0.17f, fontShader);
			}
		} else if (timer > 1750) {
			font->render(GAMES_STR, (timer - 1750) / 250, -0.075f, 0.17f, fontShader);
		}
	}
}