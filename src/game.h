#ifndef GAME_H
#define GAME_H
#define GLEW_STATIC

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "state/states.h"

int getWidth();
int getHeight();
void setSize(int w, int h);
bool isFullscreen();
void swapFullscreen();
float getSfxVolume();
void setSfxVolume(float v);
float getBgmVolume();
void setBgmVolume(float v);
int getNumVidModes();
const GLFWvidmode* getVidModes();
bool isLeftMouseDown();
bool isRightMouseDown();
void setState(StateEnum s);
void quitGame();
int randomInt(int max);
float randomFloat();
void resetThreshold();
void setThreshold(float n);

#endif
