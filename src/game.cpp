// In the Night

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <AL/alut.h>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
using namespace rapidxml;

GLFWwindow* window;
int width, height, numVidModes;
float threshold, sfxVolume, bgmVolume, oldMouseX, oldMouseY;
bool fullscreen, justIntro;
State* currentState;
GLuint fbo, fboTex, rbo, vao, vbo;
Shader* postShader;

long getTime() {
	return GetTickCount();
}
unsigned int seed = getTime();
void errorCallback(int error, const char* description) {
	fputs(description, stderr);
	_fgetchar();
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	currentState->checkKeyInput(key, action);
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	currentState->checkMouseInput((float) xpos - oldMouseX, (float) ypos - oldMouseY);
	oldMouseX = (float) xpos;
	oldMouseY = (float) ypos;
}
int randomInt(int max) {
	seed = (unsigned int) (seed * 0x5DEECE66D + 0xB + getTime());
	int result = seed % max;
	return result;
}
float randomFloat() {
	int rand = randomInt(0xFFFF);
	return (float) rand / 0xFFFF;
}
void initGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, width, height);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_DITHER);
	glClearColor(0.04f, 0.01f, 0.09f, 0.0f);
}
int getWidth() {
	return width;
}
int getHeight() {
	return height;
}
void createFramebuffer() {
	postShader = new Shader("data/shader/post.vert", "data/shader/post.frag");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &fboTex);
	glBindTexture(GL_TEXTURE_2D, fboTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, getWidth(), getHeight());

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLfloat verts[] = { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void setSize(int w, int h) {
	delete postShader;
	glDeleteTextures(1, &fboTex);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteBuffers(1, &vbo);

	width = w;
	height = h;
	glfwSetWindowSize(window, width, height);
	initGL();

	createFramebuffer();
}
bool isFullscreen() {
	return fullscreen;
}
void swapFullscreen() {
	delete postShader;
	glDeleteTextures(1, &fboTex);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteBuffers(1, &vbo);

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_REFRESH_RATE, 30);
	fullscreen = !fullscreen;
	GLFWwindow* newWindow;
	if (fullscreen) {
		newWindow = glfwCreateWindow(width, height, "In the Night", glfwGetPrimaryMonitor(), NULL);
	} else {
		newWindow = glfwCreateWindow(width, height, "In the Night", NULL, NULL);
	}
	glfwMakeContextCurrent(NULL);
	glfwDestroyWindow(window);
	window = newWindow;
	if (!window) {
		printf("New window could not be created!\n");
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSwapInterval(1);
	glfwShowWindow(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	initGL();
	createFramebuffer();
}
float getSfxVolume() {
	return sfxVolume;
}
void setSfxVolume(float v) {
	sfxVolume = v;
}
float getBgmVolume() {
	return bgmVolume;
}
void setBgmVolume(float v) {
	bgmVolume = v;
}
int getNumVidModes() {
	return numVidModes;
}
const GLFWvidmode* getVidModes() {
	int i = 1;
	return glfwGetVideoModes(glfwGetMonitors(&i)[0], &numVidModes);
}
void resetThreshold() {
	threshold = 1.0f;
}
void setThreshold(float n) {
	if (n < threshold) {
		threshold = n;
	}
}
bool isLeftMouseDown() {
	return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == 1;
}
bool isRightMouseDown() {
	return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == 1;
}
void setState(StateEnum s) {
	switch (s) {
	case INTRO:
		delete currentState;
		currentState = new IntroState();
		justIntro = true;
		break;
	case MAIN_MENU:
		delete currentState;
		currentState = new MainMenuState(justIntro);
		justIntro = false;
		break;
	case OPTIONS:
		delete currentState;
		currentState = new OptionsState();
		justIntro = false;
		break;
	case GAMEPLAY:
		delete currentState;
		currentState = new GameplayState();
		justIntro = false;
		break;
	default:
		break;
	}
}
void quitGame() {
	glfwSetWindowShouldClose(window, GL_TRUE);
}
void loadSettings() {
	file<> xmlFile("data/settings.itn");
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("settings");
	if (atoi(rootNode->first_node("fullscreen")->first_attribute("val")->value()) == 0) {
		fullscreen = false;
	} else {
		fullscreen = true;
	}
	width = atoi(rootNode->first_node("width")->first_attribute("val")->value());
	height = atoi(rootNode->first_node("height")->first_attribute("val")->value());
	sfxVolume = atof(rootNode->first_node("sfx")->first_attribute("val")->value());
	bgmVolume = atof(rootNode->first_node("bgm")->first_attribute("val")->value());
}
void init() {
	glfwSetErrorCallback(errorCallback);
	try {
		loadSettings();
	} catch (const std::exception& ex) {
		width = 800;
		height = 600;
		fullscreen = false;
		sfxVolume = 1.0f;
		bgmVolume = 1.0f;
	}
	oldMouseX = width / 2;
	oldMouseY = height / 2;
	if (!glfwInit()) {
		printf("GLFW could not be initialized!");
		exit(-1);
	}
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_REFRESH_RATE, 30);
	if (fullscreen) {
		window = glfwCreateWindow(width, height, "In the Night", glfwGetPrimaryMonitor(), NULL);
	} else {
		window = glfwCreateWindow(width, height, "In the Night", NULL, NULL);
	}
	if (!window) {
		glfwTerminate();
		printf("Window could not be created!");
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSwapInterval(1);
	glfwShowWindow(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("GLEW could not be initialized! %s", glewGetErrorString(err));
		exit(-1);
	}
	initGL();
	createFramebuffer();
	alutInit(0, NULL);
}
void render(int stateTime) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentState->render(stateTime);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	postShader->use();
	postShader->setTextureID(fboTex, "tex", 0);
	postShader->setUniform1f("threshold", threshold);
	postShader->setUniform1f("time", (float) stateTime);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
void loop() {
	threshold = 1.0f;
	justIntro = true;
	currentState = new IntroState();
	int stateTime = 0;
	long currentTime = getTime();
	long lastTime = getTime();
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		currentTime = getTime();
		int delta = (int) (currentTime - lastTime);
		stateTime += delta;
		currentState->update(delta);
		render(stateTime);
		glfwSwapBuffers(window);
		lastTime = currentTime;
	}
	delete currentState;
}
void dispose() {
	delete postShader;
	glDeleteTextures(1, &fboTex);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteBuffers(1, &vbo);
	glfwDestroyWindow(window);
	alutExit();
	glfwTerminate();
}
int main() {
	init();
	loop();
	dispose();
	return 0;
}
