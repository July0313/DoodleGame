#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <string>

#include "ConstValue.h"
#include "Platform.h"

using namespace sf;
using namespace std;

void textPrint(Text& text, Font& font, int size, int x, int y, const Color& color, bool isBold, string msg)
{
	text.setFont(font);
	text.setCharacterSize(size);
	text.setPosition(x, y);
	text.setFillColor(color);
	text.setString(msg);

	if(isBold) text.setStyle(Text::Bold);
}

int main()
{
	srand((unsigned)time(NULL));

#pragma region ������ ����
	RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "Doodle Game",  Style::Fullscreen);
	window.setFramerateLimit(60);
#pragma endregion
#pragma region ��������Ʈ ����
	Texture tBackground, tPlatform, tCharacter;
	tBackground.loadFromFile("Resources/background.png");
	tPlatform.loadFromFile("Resources/platform.png");
	tCharacter.loadFromFile("Resources/character.png");
	Sprite sBackground(tBackground), sPlatform(tPlatform), sCharacter(tCharacter);
#pragma endregion
#pragma region ȿ����
	Sound sound;

	SoundBuffer buffer;
	buffer.loadFromFile("Resources/jumpSound.wav");

	sound.setBuffer(buffer);
#pragma endregion

#pragma region �÷��� �迭 ���� �� �ʱ� ��ġ ����
	Platform* platforms = new Platform[PLATFORM_COUNT];

	for (int i = 0; i < PLATFORM_COUNT; i++)
	{
		platforms[i].setFirst(&platforms[i], i);	// platform[-1]�� �� �� nullptr�� �ƴ� �� ���Ƽ� i �ް� �����ڷ�...
	}
#pragma endregion

	// �÷��̾� ��ü ����
	Player player;
	player.setX(100);
	player.setY(100);

	// ���� ����
	int stayHeight = WIN_HEIGHT/2 - PLAYER_HEIGHT;	// �÷��̾ ��½� �ִ�� ������ ����(������ �� �ƴϸ� ��� ��ġ�� y��)
	int scoreNum = 0;
	bool isGameOver = false;
	float gravityScale = 0.2f;

	while (window.isOpen())	// ���� ����
	{
#pragma region ������ �̺�Ʈ ó��
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}
#pragma endregion

#pragma region Ű���� INPUT ó��
		if (Mouse::isButtonPressed(Mouse::Button::Right) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.moveX(3);
		}
		if (Mouse::isButtonPressed(Mouse::Button::Left) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.moveX(-3);
		}
#pragma endregion

#pragma region �߷� ó��
		player.gravity(gravityScale);
#pragma endregion

#pragma region ���� �浹 �� Ƣ�����
		if (player.checkGround())
		{
			// break;	// ����
			player.setDy(-10);	// ���ٴ�(�÷���X)�� ����� �� �۵�(Ƣ�������)
			isGameOver = scoreNum == 0 ? false : true;	// ������ ���� ���� ������ ���� ��´ٰ� �ٷ� ���� ������ ó������ ����
		}
#pragma endregion

#pragma region �÷��̾� ��¿� ���� �÷��� �ٽ� �׸���*
		if (player.getY() < stayHeight)	// �÷��̾� y ��ǥ�� stayHeight(initVal WIN_HEIGHT/2)���� ���� ���� ��(��� ��)
		{
			player.setY(stayHeight);	// �÷��̾ ȭ�鿡�� �׷����� y ���� �� �������� �ȵǹǷ� �ִ�ġ�� stayHeight���� y�� �ٽ� ���� ����

			for (int i = 0; i < 10; i++)
			{
				platforms[i].redraw(player);
			}
		}
#pragma endregion

#pragma region �÷��� �浹 �˻�
		for (int i = 0; i < 10; i++)	// �÷��� ��ü üũ
		{
			if (platforms[i].checkCol(player))
			{
				scoreNum++;
				// �ܼ�â Ȯ�ο�
				cout << i << "th Platform ; Pos(" << player.getX() << ", " << player.getY() << ")" << endl;
				player.setDy(-10);	// ���
				sound.play();
			}
		}
#pragma endregion

#pragma region �ؽ�Ʈ ����
		Text score, gameOver;
		Font font;
		font.loadFromFile("Resources/font.ttf");
		textPrint(score, font, 20, 10, 10, Color::Cyan, false, to_string(scoreNum));
#pragma endregion
#pragma region ������
		if (!isGameOver)
		{
			sCharacter.setPosition(player.getX(), player.getY());	// ĳ���� ������ �����ϴ� �ڵ带 ������ ���� ������ �ִ� �� �³�...?

			window.draw(sBackground);
			window.draw(sCharacter);
			for (int i = 0; i < 10; i++)
			{
				sPlatform.setPosition(platforms[i].getX(), platforms[i].getY());
				window.draw(sPlatform);
			}
			window.draw(score);
		}
		else
		{
			window.clear(Color::Black);	// ȭ�� �����
			delete[] platforms;			// �÷��� ����

			textPrint(gameOver, font, 50, 60, 50, Color::Red, true, "GAME OVER");
			textPrint(score, font, 30, 190, 150, Color::White, false, to_string(scoreNum));

			// backgroundMusic.pause();	// ������� ���ߴ� �ڵ尡 ���� �ִ� �� �³�...?

			window.draw(gameOver);
			window.draw(score);
		}

		window.display();
#pragma endregion
	}

	return 0;

}