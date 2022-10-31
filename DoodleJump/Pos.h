#pragma once
class Pos
{
protected:
	float x, y;
public:
	float getX() const { return x; }
	float getY() const { return y; }
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }
};