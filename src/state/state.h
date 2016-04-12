#ifndef STATE_H_
#define STATE_H_

class State {
public:
	State() {}
	virtual ~State() {}
	virtual void checkKeyInput(int key, int action) {}
	virtual void checkMouseInput(float deltaX, float deltaY) {}
	virtual void update(int delta) {}
	virtual void render(int stateTime) {}
};

#endif
