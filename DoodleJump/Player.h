#pragma once
#include "Pos.h"
#include "ConstValue.h"

class Player : public Pos
{
	float dy = 0;	// deltaY
public:
	float getDy() const { return dy; }
	void setDy(float value) { dy = value; }
	void addDy(float value) { dy += value; }
	void moveX(float value) { setX(x + value); }
	void moveY(float value) { setY(y + value); }
	void gravity(float gravityScale)
	{
		dy += gravityScale;	// 내려오기
		y += dy;			// 플레이어의 DeltaY 값만큼 내려오기
	}
	bool checkGround()
	{
		return y > WIN_HEIGHT - PLAYER_HEIGHT ? true : false;
	}
};