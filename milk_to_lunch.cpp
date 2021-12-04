#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
using namespace bangtal;

ScenePtr milk_lunch;

ObjectPtr milk_lunch_text_box[5];
int milk_lunch_idx = 0;

extern void school_lunch_main();

void milk_lunch_init()
{
	milk_lunch = Scene::create("", "텍스트상자-유빈/milk_2_lunch_bg.png");

	for (int i = 0; i < 4; i++)
	{
		char milk_lunch_path[50] = { 0, };
		sprintf(milk_lunch_path, "텍스트상자-유빈/%d.png", i);
		//모든 텍스트 상자는 이전 텍스트상자가 클릭되어야 나타남

		milk_lunch_text_box[i] = Object::create(milk_lunch_path, milk_lunch, 0, 0, false);
		milk_lunch_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)->bool
			{
				milk_lunch_text_box[milk_lunch_idx]->hide();
				if (milk_lunch_idx == 3)
				{
					school_lunch_main();
					return 0;
				}
				milk_lunch_text_box[++milk_lunch_idx]->show();
				return 0;
			});
	}
}

void milk_2_lunch_main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	milk_lunch_init();
	milk_lunch_text_box[0]->show();
	// startGame(milk_lunch);
	milk_lunch->enter();
}
