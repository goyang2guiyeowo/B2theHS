#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace bangtal;

ScenePtr you_win, you_lose;

ObjectPtr ending_text_box[17];
int ending_idx = 1;
float end_scene_light;

ObjectPtr quiet_t;
TimerPtr endTimer;
float endingTime = 0.01f;

void ending_fade_out()
{
	you_win->setImage("�ؽ�Ʈ����-����/black_scene.png");
	end_scene_light = 1.0f;
	endTimer = Timer::create(endingTime);
	endTimer->setOnTimerCallback([&](auto)->bool
		{
			if (end_scene_light > 0)
			{
				end_scene_light -= 0.005f;
				you_win->setLight(end_scene_light);
				endTimer->set(endingTime);
				endTimer->start();
			}
			else
			{
				ending_text_box[++ending_idx]->show();
			}
			return 0;
		});
	endTimer->start();
}

void ending_animation()
{
	bool check = false;
	end_scene_light = 0.f;
	you_win->setImage("�ؽ�Ʈ����-����/final_clear.png");
	endTimer = Timer::create(endingTime);
	endTimer->setOnTimerCallback([&](auto)->bool
		{
			if (end_scene_light < 1)
			{
				end_scene_light += 0.005f;
				you_win->setLight(end_scene_light);
				endTimer->set(endingTime);
				endTimer->start();
			}
			else 
			{
				endGame();
			}
			return 0;
		});
	endTimer->start();
}

ScenePtr ending_scene(int idx)
{
	if (idx <= 12) return you_win;
	else if (idx <= 16) return you_lose;
}

void init_ending_scene()
{
	you_lose = Scene::create("", "�ؽ�Ʈ����-����/û����2.png");
	you_win = Scene::create("", "�ؽ�Ʈ����-����/û���� ����̹���.jpg");
}

void ending_init()
{
	// quiet_t = Object::create("�ؽ�Ʈ����-����/professor.png", you_win, 700, 150);

	for (int i = 1; i < 17; i++)
	{
		char ending_path[50] = { 0, };
		sprintf(ending_path, "�ؽ�Ʈ����-����/ending (%d).png", i);
		//��� �ؽ�Ʈ ���ڴ� ���� �ؽ�Ʈ���ڰ� Ŭ���Ǿ�� ��Ÿ��

		ending_text_box[i] = Object::create(ending_path, ending_scene(i), 0, 0, false);
		ending_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)->bool
			{
				ending_text_box[ending_idx]->hide();

				if (ending_idx == 8) // Ŭ���� ȭ������ ��ȯ
					ending_fade_out();

				if (ending_idx == 15) // ���� ���� ���� �ؽ�Ʈ���
					you_lose->setImage("�ؽ�Ʈ����-����/game_final_ending.png");

				if (ending_idx == 16) // ���� ���� �κ� ������ �ؽ�Ʈ���
					endGame();

				if (ending_idx == 12) // ���� ���� �κ� ������ �ؽ�Ʈ���
					ending_animation();

				if (ending_idx != 8 && ending_idx != 16) ending_text_box[++ending_idx]->show();
				
				return 0;
			});
	}
}

// life_cut���� üũ�ؼ� �ҷ�����
void game_over_ending_main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	init_ending_scene();
	ending_init();
	you_lose->enter();
	ending_idx = 13;
	ending_text_box[13]->show();
}

void game_clear_ending_main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	init_ending_scene();
	ending_init();
	you_win->enter();
	ending_text_box[1]->show();
}