#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace bangtal;

extern ScenePtr post_go_to_school;
extern ScenePtr is_wrong;
extern ScenePtr pre_is_wrong;

extern ScenePtr dress_room;
extern ScenePtr pre_go_to_school;
extern ScenePtr go_to_school;
extern void fade_out();

// 서사 진행용 말풍선과 전체 게임에서 index를 체크할 전역변수
extern ObjectPtr story_text_box[37];
extern int story_idx;

extern int clear[6];

auto is_wrong_BGM = Sound::create("Sound/Wolf Mother - Loopop.mp3");

// 틀린그림찾기 클릭
ObjectPtr is_wrong_finger;

auto is_wrong_click = Sound::create("Sound/button.mp3");

extern ObjectPtr life_point_bar;
extern ObjectPtr life_point[4];
extern void life_scene_2(ScenePtr scene);
extern void life_cut();
extern void is_wrong_main();

extern ObjectPtr is_wrong_picnic;

void init_is_wrong()
{
	is_wrong_finger = Object::create("/Images/is_wrong_finger.png", is_wrong, 0, 0, false);

	auto is_wrong_clock = Object::create("/Images/clock1.png", is_wrong, 1100, 570);
	is_wrong_clock->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			// 아직 건드린 적 없는 경우만
			if (clear[4] == 2)
			{
				// 정답은 시계
				clear[4] = 1;
				is_wrong_finger->locate(is_wrong, 1000, 470);
				is_wrong_finger->show();
				is_wrong_click->play();
				story_idx = 28;
				story_text_box[story_idx]->show();
			}
			// clear 인덱스 체크하고, 숨기고, 메시지 출력
			return 0;
		});
	is_wrong_picnic->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			if (clear[4] == 2)
			{
				clear[4] = 0;
				is_wrong_finger->locate(is_wrong, 220, 55);
				is_wrong_finger->show();
				is_wrong_click->play();
				story_idx = 26;
				story_text_box[story_idx]->show();
			}
			return 0;
		});
	auto is_wrong_door = Object::create("/Images/door1.png", is_wrong, 7, 277);
	is_wrong_door->setScale(2.0f);
	is_wrong_door->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			if (clear[4] == 2)
			{
				clear[4] = 0;
				is_wrong_finger->locate(is_wrong, 50, 177);
				is_wrong_finger->show();
				is_wrong_click->play();
				story_idx = 27;
				story_text_box[story_idx]->show();
			}
			return 0;
		});
}

void is_wrong_main()
{
	srand((unsigned int)time(NULL));
	pre_is_wrong->enter();
	init_is_wrong();
}