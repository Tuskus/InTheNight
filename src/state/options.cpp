#include "state/options.h"

#include <stdio.h>
#include <string.h>
#include "game.h"

OptionsState::OptionsState() {
	fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
	font = new Font();
	selectedOption = 0;
	modes = getVidModes();
	currentMode = 0;
	for (int i = 0; i < getNumVidModes() && currentMode == 0; i++) {
		if (modes[i].width == getWidth() && modes[i].height == getHeight() && modes[i].refreshRate == 60 && isRGB888(modes[i])) {
			currentMode = i;
		}
	}
}
OptionsState::~OptionsState() {
	delete fontShader;
	delete font;
}
bool OptionsState::isRGB888(GLFWvidmode m) {
	return m.redBits == 8 && m.greenBits == 8 && m.blueBits == 8;
}
void OptionsState::saveOptions() {
	FILE* file = fopen("data/settings.itn", "w");
	fwrite("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"), 1, file);
	fwrite("<settings>\n", strlen("<settings>\n"), 1, file);
	char buffer[32];
	fwrite("\t<fullscreen val=\"", strlen("\t<fullscreen val=\""), 1, file);
	if (isFullscreen()) {
		fwrite("1", 1, 1, file);
	} else {
		fwrite("0", 1, 1, file);
	}
	fwrite("\"/>\n", strlen("\"/>\n"), 1, file);
	sprintf(buffer, "\t<width val=\"%d\"/>\n", getWidth());
	fwrite(buffer, strlen(buffer), 1, file);
	sprintf(buffer, "\t<height val=\"%d\"/>\n", getHeight());
	fwrite(buffer, strlen(buffer), 1, file);
	sprintf(buffer, "\t<sfx val=\"%f\"/>\n", getSfxVolume());
	fwrite(buffer, strlen(buffer), 1, file);
	sprintf(buffer, "\t<bgm val=\"%f\"/>\n", getBgmVolume());
	fwrite(buffer, strlen(buffer), 1, file);
	sprintf(buffer, "</settings>", strlen("</settings>"), 1, file);
	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
}
void OptionsState::checkKeyInput(int key, int action) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ENTER:
			if (selectedOption == 0) {
				saveOptions();
				setState(MAIN_MENU);
			} else if (selectedOption == 5) {
				if (!isFullscreen()) {
					delete fontShader;
					delete font;
					swapFullscreen();
					fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
					font = new Font();
				}
				while (modes[currentMode].width != 800 || modes[currentMode].height != 600 ||
					modes[currentMode].refreshRate != 60 || !isRGB888(modes[currentMode])) {
					currentMode++;
					if (currentMode >= getNumVidModes()) {
						currentMode = 0;
					}
				}
				setSize(modes[currentMode].width, modes[currentMode].height);
				setSfxVolume(1.0f);
				setBgmVolume(1.0f);
			}
			break;
		case GLFW_KEY_ESCAPE:
			saveOptions();
			setState(MAIN_MENU);
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			selectedOption--;
			if (selectedOption < 0) {
				selectedOption = 5;
			}
			break;
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			selectedOption++;
			if (selectedOption > 5) {
				selectedOption = 0;
			}
			break;
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			switch (selectedOption) {
			case 1:
				delete fontShader;
				delete font;
				swapFullscreen();
				fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
				font = new Font();
				break;
			case 2:
				do {
					currentMode--;
					if (currentMode < 0) {
						currentMode = getNumVidModes() - 1;
					}
				} while (modes[currentMode].refreshRate != 60 || !isRGB888(modes[currentMode]));
				setSize(modes[currentMode].width, modes[currentMode].height);
				break;
			case 3:
				if (getSfxVolume() > 0.0f) {
					setSfxVolume(getSfxVolume() - 0.1f);
					if (getSfxVolume() < 0.0f) {
						setSfxVolume(0.0f);
					}
				}
				break;
			case 4:
				if (getBgmVolume() > 0.0f) {
					setBgmVolume(getBgmVolume() - 0.1f);
					if (getBgmVolume() < 0.0f) {
						setBgmVolume(0.0f);
					}
				}
				break;
			default:
				break;
			}
			break;
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			switch (selectedOption) {
			case 1:
				delete fontShader;
				delete font;
				swapFullscreen();
				fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
				font = new Font();
				break;
			case 2:
				do {
					currentMode++;
					if (currentMode >= getNumVidModes()) {
						currentMode = 0;
					}
				} while (modes[currentMode].refreshRate != 60 || !isRGB888(modes[currentMode]));
				setSize(modes[currentMode].width, modes[currentMode].height);
				break;
			case 3:
				if (getSfxVolume() < 1.0f) {
					setSfxVolume(getSfxVolume() + 0.1f);
					if (getSfxVolume() > 1.0f) {
						setSfxVolume(1.0f);
					}
				}
				break;
			case 4:
				if (getBgmVolume() < 1.0f) {
					setBgmVolume(getBgmVolume() + 0.1f);
					if (getBgmVolume() > 1.0f) {
						setBgmVolume(1.0f);
					}
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}
void OptionsState::render(int stateTime) {
	fontShader->use();
	char buffer[16];
	if (stateTime % 1500 > 150) {
		font->render(">", -0.9333, 0.855f - selectedOption * 0.285f, fontShader);
	}
	font->render("BACK", -0.8333f, 0.855f, fontShader);
	font->render("FULLSCREEN", -0.8333f, 0.57f, fontShader);
	strcpy(buffer, isFullscreen() ? "ON" : "OFF");
	font->render(buffer, 0.85f - strlen(buffer) * 0.08333f, 0.57f, fontShader);
	font->render("SCREEN SIZE", -0.8333f, 0.285f, fontShader);
	sprintf(buffer, "%dx%d", modes[currentMode].width, modes[currentMode].height);
	font->render(buffer, 0.85f - strlen(buffer) * 0.08333f, 0.285f, fontShader);
	font->render("SOUND EFFECTS VOLUME", -0.8333f, 0.0f, fontShader);
	font->render("_____________", -0.5f, -0.1f, fontShader);
	font->render("v", -0.5f + getSfxVolume(), -0.075f, fontShader);
	font->render("^", -0.5f + getSfxVolume(), -0.15f, fontShader);
	font->render("MUSIC VOLUME", -0.8333f, -0.285f, fontShader);
	font->render("_____________", -0.5f, -0.385f, fontShader);
	font->render("v", -0.5f + getBgmVolume(), -0.36f, fontShader);
	font->render("^", -0.5f + getBgmVolume(), -0.435f, fontShader);
	font->render("RETURN TO DEFAULTS", -0.8333f, -0.57f, fontShader);
}
