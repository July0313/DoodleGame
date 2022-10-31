#pragma once
#include <stdlib.h>

#include "Pos.h"
#include "Player.h"
#include "ConstValue.h"

class Platform : public Pos
{
public:
	void setFirst(const Platform* platform, int count);
	void redraw(const Player& player);
	bool checkCol(const Player& player);
};