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

#pragma region 윈도우 세팅
	RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "Doodle Game",  Style::Fullscreen);
	window.setFramerateLimit(60);
#pragma endregion
#pragma region 스프라이트 세팅
	Texture tBackground, tPlatform, tCharacter;
	tBackground.loadFromFile("Resources/background.png");
	tPlatform.loadFromFile("Resources/platform.png");
	tCharacter.loadFromFile("Resources/character.png");
	Sprite sBackground(tBackground), sPlatform(tPlatform), sCharacter(tCharacter);
#pragma endregion
#pragma region 효과음
	Sound sound;

	SoundBuffer buffer;
	buffer.loadFromFile("Resources/jumpSound.wav");

	sound.setBuffer(buffer);
#pragma endregion

#pragma region 플랫폼 배열 선언 및 초기 위치 세팅
	Platform* platforms = new Platform[PLATFORM_COUNT];

	for (int i = 0; i < PLATFORM_COUNT; i++)
	{
		platforms[i].setFirst(&platforms[i], i);	// platform[-1]로 들어갈 시 nullptr이 아닐 것 같아서 i 받고 참조자로...
	}
#pragma endregion

	// 플레이어 객체 생성
	Player player;
	player.setX(100);
	player.setY(100);

	// 변수 선언
	int stayHeight = WIN_HEIGHT/2 - PLAYER_HEIGHT;	// 플레이어가 상승시 최대로 도달할 높이(떨어질 때 아니면 계속 위치할 y값)
	int scoreNum = 0;
	bool isGameOver = false;
	float gravityScale = 0.2f;

	while (window.isOpen())	// 게임 루프
	{
#pragma region 윈도우 이벤트 처리
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}
#pragma endregion

#pragma region 키보드 INPUT 처리
		if (Mouse::isButtonPressed(Mouse::Button::Right) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.moveX(3);
		}
		if (Mouse::isButtonPressed(Mouse::Button::Left) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.moveX(-3);
		}
#pragma endregion

#pragma region 중력 처리
		player.gravity(gravityScale);
#pragma endregion

#pragma region 땅과 충돌 시 튀어오름
		if (player.checkGround())
		{
			// break;	// 종료
			player.setDy(-10);	// 땅바닥(플랫폼X)에 닿았을 때 작동(튀어오르기)
			isGameOver = scoreNum == 0 ? false : true;	// 발판을 밟은 적이 없으면 땅을 밟는다고 바로 게임 오버로 처리되지 않음
		}
#pragma endregion

#pragma region 플레이어 상승에 따라 플랫폼 다시 그리기*
		if (player.getY() < stayHeight)	// 플레이어 y 좌표가 stayHeight(initVal WIN_HEIGHT/2)보다 위에 있을 때(상승 시)
		{
			player.setY(stayHeight);	// 플레이어가 화면에서 그려지는 y 값은 더 높아지면 안되므로 최대치인 stayHeight으로 y값 다시 강제 세팅

			for (int i = 0; i < 10; i++)
			{
				platforms[i].redraw(player);
			}
		}
#pragma endregion

#pragma region 플랫폼 충돌 검사
		for (int i = 0; i < 10; i++)	// 플랫폼 전체 체크
		{
			if (platforms[i].checkCol(player))
			{
				scoreNum++;
				// 콘솔창 확인용
				cout << i << "th Platform ; Pos(" << player.getX() << ", " << player.getY() << ")" << endl;
				player.setDy(-10);	// 상승
				sound.play();
			}
		}
#pragma endregion

#pragma region 텍스트 세팅
		Text score, gameOver;
		Font font;
		font.loadFromFile("Resources/font.ttf");
		textPrint(score, font, 20, 10, 10, Color::Cyan, false, to_string(scoreNum));
#pragma endregion
#pragma region 렌더링
		if (!isGameOver)
		{
			sCharacter.setPosition(player.getX(), player.getY());	// 캐릭터 움직임 관리하는 코드를 렌더링 리전 안으로 넣는 게 맞나...?

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
			window.clear(Color::Black);	// 화면 지우기
			delete[] platforms;			// 플랫폼 삭제

			textPrint(gameOver, font, 50, 60, 50, Color::Red, true, "GAME OVER");
			textPrint(score, font, 30, 190, 150, Color::White, false, to_string(scoreNum));

			// backgroundMusic.pause();	// 배경음악 멈추는 코드가 여기 있는 게 맞나...?

			window.draw(gameOver);
			window.draw(score);
		}

		window.display();
#pragma endregion
	}

	return 0;

}