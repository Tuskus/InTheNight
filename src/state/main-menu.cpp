#include "state/main-menu.h"

#include <GLFW/glfw3.h>
#include "game.h"

MainMenuState::MainMenuState(bool start) {
	fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
	font = new Font();
	pressStart = start;
	showPrompt = false;
	wantsExit = false;
	selectedOption = 0;
	switchSfx = new Sound("data/sfx/switch.wav");
	selectSfx = new Sound("data/sfx/select.wav");
	noiseSfx = new Sound("data/sfx/noise.wav");
	noiseSfx->loop(getSfxVolume());
	resetThreshold();
}
MainMenuState::~MainMenuState() {
	delete fontShader;
	delete font;
	delete switchSfx;
	delete selectSfx;
	delete noiseSfx;
}
void MainMenuState::checkKeyInput(int key, int action) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ENTER:
			selectSfx->play();
			if (pressStart) {
				pressStart = false;
				selectedOption = 0;
			} else if (showPrompt) {
				if (wantsExit) {
					quitGame();
				} else {
					showPrompt = false;
				}
			} else {
				switch (selectedOption) {
				case 0:
					setState(GAMEPLAY);
					break;
				case 1:
					setState(OPTIONS);
					break;
				case 2:
					showPrompt = true;
					wantsExit = false;
					break;
				default:
					break;
				}
			}
			break;
		case GLFW_KEY_ESCAPE:
			selectSfx->play();
			if (pressStart) {
				setState(INTRO);
			} else if (showPrompt) {
				showPrompt = false;
			} else {
				pressStart = true;
			}
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			selectedOption--;
			if (selectedOption < 0) {
				selectedOption = 2;
			}
			switchSfx->play();
			break;
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			selectedOption++;
			if (selectedOption > 2) {
				selectedOption = 0;
			}
			switchSfx->play();
			break;
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			if (showPrompt) {
				wantsExit = !wantsExit;
				switchSfx->play();
			}
			break;
		default:
			break;
		}
	}
}
void MainMenuState::render(int stateTime) {
	fontShader->use();
	if (!showPrompt) {
		font->render("IN THE NIGHT", -0.5f, 0.4f, fontShader);
	}
	if (pressStart) {
		if (stateTime % 1500 > 150) {
			font->render("PRESS ENTER", -0.458333f, -0.7f, fontShader);
		}
	} else {
		if (showPrompt) {
			font->render("   ARE YOU SURE\nYOU WISH TO LEAVE?", -0.75f, 0.4f, fontShader);
			font->render("YES", -0.374666f, -0.1f, fontShader);
			font->render("NO", 0.291333f, -0.1f, fontShader);
			if (stateTime % 1500 > 150) {
				font->render(">", wantsExit ? -0.499666f : 0.166333f, -0.1f, fontShader);
			}
		} else {
			if (stateTime % 1500 > 150) {
				font->render(">", -0.75f, selectedOption * -0.25f - 0.15f, fontShader);
			}
			font->render("START THE DREAM", -0.625f, -0.15f, fontShader);
			font->render("OPTIONS", -0.625f, -0.4f, fontShader);
			font->render("LEAVE", -0.625f, -0.65f, fontShader);
		}
	}
}