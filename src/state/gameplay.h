#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "state/state.h"

#include "actor/actors.h"
#include "gameplay/map.h"
#include "gameplay/player.h"
#include "gameplay/water.h"
#include "util/font.h"
#include "util/mesh.h"
#include "util/shader.h"
#include "util/sound.h"

class GameplayState : public State {
public:
	GameplayState();
	~GameplayState();
	float calcY(float x, float z);
	void checkKeyInput(int key, int action);
	void checkMouseInput(float deltaX, float deltaY);
	void update(int delta);
	void render(int stateTime);
private:
	int mapNum, startTimer, prevRunes;
	Shader* surfaceShader;
	Shader* fontShader;
	Font* font;
	Map* map;
	Player* player;
	Water* water;
	Sound* bgm;
	Mesh* soMesh;
	Mesh* stakeMesh;
	vector<SilentOne*> silentOnes;
	vector<Actor*> actors;
	glm::vec3 skyLight, skyDark, groundLight, groundDark, waterLight, waterDark;
	void loadMap();
	void genColors(int runesCollected);
	void loadActors();
	void loadBGM();
};

#endif
