#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <random>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace bangtal;

extern void beg_main();
extern int life;

SoundPtr text_box_next;

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	text_box_next = Sound::create("Sound/next_box_next.mp3");
	ScenePtr firstScene = Scene::create("", "Images/start_scene.png");
	ObjectPtr start_game = Object::create("Images/start_button.png", firstScene, 540, 30);
	start_game->setScale(0.2f);
	start_game->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool
		{
			beg_main();
			return 0;
		});

	startGame(firstScene);
}