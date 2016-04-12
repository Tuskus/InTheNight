#ifndef CIRCLE_H_
#define CIRCLE_H_

class Circle {
public:
	Circle(float startX, float startY, float r);
	~Circle();
	float getX();
	float getY();
	void setY(float newY);
	float getVelocityX();
	float getVelocityY();
	void addVelocity(float x, float y);
	void move(float w, float h, int delta);
	void checkCollision(Circle* c);
private:
	float xPos, yPos, xVel, yVel, radius;
	float getDistSquared(float x1, float y1, float x2, float y2);
};

#endif
