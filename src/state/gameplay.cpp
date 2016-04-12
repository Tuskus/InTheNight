#include "state/gameplay.h"

#include <stdio.h>
#include "glm/gtc/matrix_transform.hpp"
#include "game.h"
#include "util/builder.h"

#define PI 3.14159265f

GameplayState::GameplayState() {
	startTimer = 4000;
	surfaceShader = new Shader("data/shader/surface.vert", "data/shader/surface.frag");
	fontShader = new Shader("data/shader/font.vert", "data/shader/font.frag");
	font = new Font();
	water = new Water();
	soMesh = new Mesh("data/mesh/so.obj");
	stakeMesh = new Mesh("data/mesh/stake.obj");
	mapNum = 0;
	loadMap();
}
GameplayState::~GameplayState() {
	destroyBuilder();
	for (vector<SilentOne*>::iterator it = silentOnes.begin(); it != silentOnes.end(); it++) {
		delete (*it);
	}
	silentOnes.clear();
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		delete (*it);
	}
	actors.clear();
	delete surfaceShader;
	delete fontShader;
	delete font;
	delete map;
	delete player;
	delete water;
	delete soMesh;
	delete stakeMesh;
	delete bgm;
}
void GameplayState::loadMap() {
	map = new Map("data/map/5.m");
	float startX = 1.5f * -4.0f;
	float startY = 13.25f * -4.0f;
	float angle = 0.0f;
	player = new Player(startX, startY, angle, mapNum);
	prevRunes = player->getNumRunes();
	genColors(5);
	loadActors();
	silentOnes.push_back(new SilentOne(-2.4f, -2.0f, soMesh));
	silentOnes.push_back(new SilentOne(-4.8f, -2.0f, soMesh));
	silentOnes.push_back(new SilentOne(-7.2f, -2.0f, soMesh));
	silentOnes.push_back(new SilentOne(-9.6f, -2.0f, soMesh));
	loadBGM();
}
void GameplayState::genColors(int runesCollected) {
	waterLight = glm::vec3(1.0f, -0.35f, -0.2f);
	waterDark = glm::vec3(0.45f, -0.5f, 0.25f);
	skyLight = glm::vec3(0.0f);
	skyDark = glm::vec3(0.0f);
	groundLight = glm::vec3(1.5f, 0.0f, 0.0f);
	groundDark = glm::vec3(2.0f, 0.0f, 0.0f);
	waterLight = glm::vec3(1.0f, 0.596f, 0.211f);
	waterDark = glm::vec3(0.0f, 0.596f, 0.211f);
}
void GameplayState::loadActors() {
	initBuilder();
	FILE* file = fopen("data/map/5.a", "r");
	int actorID;
	float x, y;
	while (feof(file) == 0) {
		fscanf(file, "%d %f %f\n", &actorID, &x, &y);
		actors.push_back(buildActor(actorID, x, y));
	}
	fclose(file);
}
void GameplayState::loadBGM() {
	bgm = new Sound("data/bgm/0.wav");
}
void GameplayState::checkKeyInput(int key, int action) {
	player->checkKeyInput(key, action);
}
float GameplayState::calcY(float x, float z) {
	int gridX = (int) (x / -4.0f);
	int gridY = (int) (z / -4.0f);
	int index = (gridX + (map->getWidth() - 1) * gridY) * 2;
	if (z + gridY * 4.0f < -4.0f - x - gridX * 4.0f) {
		index++;
	}
	index *= 18;
	glm::vec3 p1 = glm::vec3(map->getVertBuffer()[index], map->getVertBuffer()[index + 1], map->getVertBuffer()[index + 2]);
	glm::vec3 p2 = glm::vec3(map->getVertBuffer()[index + 6], map->getVertBuffer()[index + 7], map->getVertBuffer()[index + 8]);
	glm::vec3 p3 = glm::vec3(map->getVertBuffer()[index + 12], map->getVertBuffer()[index + 13], map->getVertBuffer()[index + 14]);
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float d1 = ((p2.z - p3.z) * (-x - p3.x) + (p3.x - p2.x) * (-z - p3.z)) / det;
	float d2 = ((p3.z - p1.z) * (-x - p3.x) + (p1.x - p3.x) * (-z - p3.z)) / det;
	float d3 = 1.0f - d1 - d2;
	return -d1 * p1.y - d2 * p2.y - d3 * p3.y - 1.0f;
}
void GameplayState::checkMouseInput(float deltaX, float deltaY) {
	player->checkMouseInput(deltaX, deltaY);
}
void GameplayState::update(int delta) {
	player->update(map, actors, delta);
	player->setY(calcY(player->getX(), player->getZ()));
	player->updateMatrix(delta);
	if (prevRunes != player->getNumRunes()) {
		resetThreshold();
		for (int i = 0; i < silentOnes.size(); i++) {
			silentOnes.at(i)->collidingWith(player);
			silentOnes.at(i)->update(delta);
			silentOnes.at(i)->setY(calcY(silentOnes.at(i)->getX(), silentOnes.at(i)->getZ()));
			silentOnes.at(i)->updateMatrix();
		}
	}
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		(*it)->update(delta);
		(*it)->setY(calcY((*it)->getX(), (*it)->getZ()));
		(*it)->updateMatrix();
	}
	if (startTimer > 0) {
		startTimer -= delta;
	}
	if (mapNum != player->getMap()) {
		if (player->getMap() < 0) {
			setState(MAIN_MENU);
		} else {
			mapNum = player->getMap();
			for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
				delete (*it);
			}
			actors.clear();
			delete map;
			delete player;
			delete bgm;
			destroyBuilder();
			initBuilder();
			loadMap();
		}
	}
}
void GameplayState::render(int stateTime) {
	surfaceShader->use();
	water->render(player->getX(), player->getY(), player->getZ(), waterLight, waterDark, player->getViewProjectionMatrix(), player->getViewMatrix(), surfaceShader);
	map->render(player->getX(), -player->getY(), player->getZ(), groundLight, groundDark, player->getViewProjectionMatrix(), player->getViewMatrix(), surfaceShader);
	glm::mat4 modelMatrix;
	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, glm::vec3(6.0f, 0.0f, -4.0f));
	modelMatrix = glm::rotate(modelMatrix, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 3.0f, 2.0f));
	stakeMesh->render(player->getX(), player->getY(), player->getZ(), skyLight, player->getViewProjectionMatrix() * modelMatrix, player->getViewMatrix() * modelMatrix, surfaceShader);
	for (int i = 0; i < 5; i++) {
		modelMatrix = glm::mat4();
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.0f, 0.0f, i * 14.0f + 4.0f));
		modelMatrix = glm::rotate(modelMatrix, 1.57f, glm::vec3(0.0f, -1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 3.0f, 2.0f));
		stakeMesh->render(player->getX(), player->getY(), player->getZ(), skyLight, player->getViewProjectionMatrix() * modelMatrix, player->getViewMatrix() * modelMatrix, surfaceShader);
		modelMatrix = glm::mat4();
		modelMatrix = glm::translate(modelMatrix, glm::vec3(16.0f, 0.0f, i * 14.0f + 4.0f));
		modelMatrix = glm::rotate(modelMatrix, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 3.0f, 2.0f));
		stakeMesh->render(player->getX(), player->getY(), player->getZ(), skyLight, player->getViewProjectionMatrix() * modelMatrix, player->getViewMatrix() * modelMatrix, surfaceShader);
	}
	if (prevRunes != player->getNumRunes()) {
		for (vector<SilentOne*>::iterator it = silentOnes.begin(); it != silentOnes.end(); it++) {
			(*it)->render(player->getX(), -player->getY(), player->getZ(), skyLight, player->getViewProjectionMatrix(), player->getViewMatrix(), surfaceShader);
		}
	}
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++) {
		(*it)->render(player->getX(), -player->getY(), player->getZ(), skyLight, player->getViewProjectionMatrix(), player->getViewMatrix(), surfaceShader);
	}
	if (startTimer > 0) {
		fontShader->use();
		font->render("FIND THEM.", -0.375f, 0.15f, fontShader);
	}
}