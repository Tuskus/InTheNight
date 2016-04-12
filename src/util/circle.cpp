#include "util/circle.h"

#include <math.h>

Circle::Circle(float startX, float startY, float r) {
	xPos = startX;
	yPos = startY;
	radius = r;
	xVel = 0.0f;
	yVel = 0.0f;
}
Circle::~Circle() {
}
float Circle::getX() {
	return xPos;
}
float Circle::getY() {
	return yPos;
}
void Circle::setY(float newY) {
	yPos = newY;
}
float Circle::getVelocityX() {
	return xVel;
}
float Circle::getVelocityY() {
	return yVel;
}
void Circle::addVelocity(float x, float y) {
	xVel += x;
	yVel += y;
}
void Circle::move(float w, float h, int delta) {
	if (delta > 0) {
		if (xVel > 0.0f) {
			xVel -= xVel * 0.004f * delta;
			if (xVel < 0.0f) {
				xVel = 0.0f;
			}
		} else if (xVel < 0.0f) {
			xVel -= xVel * 0.004f * delta;
			if (xVel > 0.0f) {
				xVel = 0.0f;
			}
		}
		if (yVel > 0.0f) {
			yVel -= yVel * 0.004f * delta;
			if (yVel < 0.0f) {
				yVel = 0.0f;
			}
		} else if (yVel < 0.0f) {
			yVel -= yVel * 0.004f * delta;
			if (yVel > 0.0f) {
				yVel = 0.0f;
			}
		}
	}
	xPos += xVel;
	yPos += yVel;
	if (xPos > -1.0f) {
		xPos = -1.0f;
	} else if (xPos < w * -4.0f + 5.0f) {
		xPos = w * -4.0f + 5.0f;
	}
	if (yPos > -1.0f) {
		yPos = -1.0f;
	} else if (yPos < h * -4.0f + 5.0f) {
		yPos = h * -4.0f + 5.0f;
	}
}
float Circle::getDistSquared(float x1, float y1, float x2, float y2) {
	return pow(x1 - x2, 2.0f) + pow(y1 - y2, 2.0f);
}
#include <stdio.h>
void Circle::checkCollision(Circle* c) {
	float distSquared = getDistSquared(getX(), getY(), c->getX(), c->getY());
	if (distSquared < pow(radius + c->radius, 2.0f)) {
		float overlap = radius + c->radius - sqrt(distSquared);
		float xDiff = xPos - c->xPos;
		float yDiff = yPos - c->yPos;
		float angle = 0.0f;
		if (xDiff != 0.0f) {
			angle = atan(yDiff / xDiff);
		}
		if (xDiff < 0.0f) {
			c->xPos += overlap * cos(angle);
			c->yPos += overlap * sin(angle);
		} else {
			c->xPos -= overlap * cos(angle);
			c->yPos -= overlap * sin(angle);
		}
	}
}