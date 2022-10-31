#include "Platform.h"

void Platform::setFirst(const Platform* platform, int count)
{
	if (count == 0)
		setY(WIN_HEIGHT - (WIN_HEIGHT / (2 * 10)));
	else
		setY((--platform)->getY() - (WIN_HEIGHT / PLATFORM_COUNT));

	setX(rand() % (WIN_WIDTH - PLATFORM_WIDTH));
}

void Platform::redraw(const Player& player)
{
	setY(y - player.getDy());

	if (y > WIN_HEIGHT)
	{
		setX(rand() % (WIN_WIDTH - PLAYER_WIDTH));
		setY(0);
	}
}

bool Platform::checkCol(const Player& player)
{
	if (player.getDy() > 0 &&				
		player.getX() >= x - (PLAYER_WIDTH / 2) &&
		player.getX() <= x + (PLAYER_WIDTH / 2) &&
		player.getY() <= y - (PLAYER_HEIGHT * 0.95) &&
		player.getY() >= y - (PLAYER_HEIGHT * 1.05))
	{
		return true;
	}

	return false;
}