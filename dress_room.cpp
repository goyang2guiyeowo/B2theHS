#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace bangtal;

// �� ����
#define DRESS_VAR_SIZE 4

extern int story_idx;;
extern void life_init();
extern void life_scene(int idx);

extern void go_to_school_main();
extern void s2m_main();
extern void mock_test_main();

int clear[2] = {2, 2};

// ���⼭ �ε����� �ڽ� ��� ����
int story_idx = 1;
ObjectPtr story_text_box[37];

// �� ����â���� � ���� ���� �̸� �����ִ� â�� ����, ������ư
ObjectPtr dress_select[DRESS_VAR_SIZE];
ObjectPtr dress_select_box[DRESS_VAR_SIZE];
ObjectPtr dress_prev_button[DRESS_VAR_SIZE];
ObjectPtr dress_next_button[DRESS_VAR_SIZE];

ObjectPtr school_door_teacher;

// �� ������ ���ù�ư Ŭ�� �Ҹ�
auto button_click = Sound::create("Sound/button.mp3");

// �� ������ ����â���� prev �Ǵ� next Ŭ�� Ƚ���� ���� �̹��� index
int dress_select_index[4] = { 0, };

// ����, �̽��Ϳ��� �ε���
int dress_answer_index[4] = { 2, 2, 2, 2 };
int dress_easter_index[4] = { 1, 1, 3, 0 };

TimerPtr animationTimer;
float animationTime = 0.01f;
float scene_light = 1.0f;

ScenePtr dress_room;
ScenePtr pre_go_to_school;
ScenePtr go_to_school;
ScenePtr post_go_to_school;
ScenePtr pre_is_wrong;
ScenePtr is_wrong;

extern void run_teacher();
extern void check_school_door();
extern void init_go_to_school();
extern ObjectPtr is_wrong_finger;
extern SoundPtr is_wrong_BGM;

extern void game_over_ending_main();

ObjectPtr is_wrong_picnic;

bool is_scene_ch_box(int idx)
{
	// �ش� ����� ������ �ؽ�Ʈ ������ �ε�������(Ŭ���ϸ� ��� ��ȯ�ؾ� �ϴ���) �Ǵ�
	if (idx == 3 || idx == 6 || idx == 11 || idx == 16 || idx == 19 || idx == 35) return true;

	else return false;
}

void fade_out()
{
	scene_light = 1.0f;
	animationTimer = Timer::create(animationTime);
	animationTimer->setOnTimerCallback([&](auto)->bool
		{
			if (scene_light > 0)
			{
				scene_light -= 0.005f;
				story_to_scene(story_idx)->setLight(scene_light);
				animationTimer->set(animationTime);
				animationTimer->start();
			}
			else
			{
				next_scene(story_to_scene(story_idx))->enter();
				if (story_idx == 10 || story_idx == 11)
				{
					check_school_door();
					story_text_box[story_idx]->show();
				}
				// ������������ �ϴ� ������ ���ؼ�
				else if (story_idx == 13 || story_idx == 14)
				{
					story_idx = 17;
					story_text_box[story_idx]->show();
					// ���⼭ ���� �����
					s2m_main();
				}
				else if (next_scene(story_to_scene(story_idx)) == pre_is_wrong)
				{
					// ��¼�� �̷��� ��� �͵� ����������
					story_idx = 20;
					story_text_box[story_idx]->show();
				}
				else if (next_scene(story_to_scene(story_idx)) == is_wrong)
				{
					story_text_box[++story_idx]->show();
					life_scene(story_idx);
				}
			}
			return 0;
		});
	animationTimer->start();
}

// �ؽ�Ʈ ���ڵ� ó������ ������ �� �����, �ϳ� ������ ���� ���̴� �ؽ�Ʈ ���� ���ְ� ���� �ؽ�Ʈ ���� ���� �ݹ��Լ� ���

// ���� ��ü ����������Ʈ ���� ����, ������ �Ҹ�
ObjectPtr life_point_bar;
ObjectPtr life_point[4];
int life = 4;
auto life_sound = Sound::create("Sound/fail.mp3");

ScenePtr next_scene(ScenePtr pre_scene)
{
	if (pre_scene == dress_room) return pre_go_to_school;
	else if (pre_scene == pre_go_to_school) return go_to_school;
	else if (pre_scene == go_to_school) return post_go_to_school;
	else if (pre_scene == post_go_to_school) return pre_is_wrong;
	else if (pre_scene == pre_is_wrong) return is_wrong;
}

ScenePtr story_to_scene(int idx)
{
	if (idx <= 3) return dress_room;
	else if (idx <= 6) return pre_go_to_school;
	else if (idx <= 11) return go_to_school;
	else if (idx <= 16) return post_go_to_school;
	else if (idx <= 19) return pre_is_wrong;
	else if (idx <= 37) return is_wrong;
}
// �ؽ�Ʈ ���� �� �ڷ� ���� ����
void init_obj()
{
	is_wrong_picnic = Object::create("/Images/picnic.png", is_wrong, 320, 155);
	school_door_teacher = Object::create("Images/school_door_teacher.png", post_go_to_school, 700, 0);
}

void life_scene(int idx)
{
	life_point_bar->locate(story_to_scene(idx), 30, 620);

	for (int i = 0; i < life; i++)
	{
		life_point[i]->locate(story_to_scene(idx), 30 + (10 * (i + 1) + 50 * (i)), 630);
	}
}

// story_to_scene�� ����� �� ���� ���
void life_scene_2(ScenePtr scene)
{
	life_point_bar->locate(scene, 30, 620);

	for (int i = 0; i < life; i++)
	{
		life_point[i]->locate(scene, 30 + (10 * (i + 1) + 50 * (i)), 630);
	}
}

//void life_init()
//{
//	life_point_bar = Object::create("Images/life_bar.png", go_to_school, 30, 620);
//
//	for (int i = 0; i < life; i++)
//	{
//		life_point[i] = Object::create("Images/life_point_0.png", go_to_school, 30 + (10 * (i + 1) + 50 * (i)), 630);
//	}
//}

// ����������Ʈ �߶󳻱�
void life_cut()
{
	if (life >= 1)
	{
		life_sound->play();
		life_point[life - 1]->setImage("Images/life_point_1.png");
		life -= 1;
	}
	else
	{
		life_sound->play();
		game_over_ending_main();
	}
}

void init_scene()
{
	post_go_to_school = Scene::create("", "Images/post_go_to_school.png");
	is_wrong = Scene::create("", "Images/is_wrong.png");
	pre_is_wrong = Scene::create("", "Images/pre_is_wrong.png");
	go_to_school = Scene::create("", "Images/go_to_school.png");
	dress_room = Scene::create("", "Images/dress_room_scene.png");
	pre_go_to_school = Scene::create("", "Images/pre_go_to_school.png");
}

// ���߿� �̰� extern���� �ޱ�
void story_init()
{
	for (int i = 1; i < 36; i++)
	{
		char story_path[50] = { 0, };
		sprintf(story_path, "Images/story_text (%d).png", i);
		// ��� �ؽ�Ʈ ���ڴ� ���� �ؽ�Ʈ ���ڰ� Ŭ���Ǳ� ������ �� ����

		story_text_box[i] = Object::create(story_path, story_to_scene(i), 0, 0, false);
		story_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				story_text_box[story_idx]->hide();
				if (is_scene_ch_box(story_idx)) story_to_scene(story_idx + 1)->enter();
				if (story_idx != 35)
					story_text_box[++story_idx]->show();

				return 0;
			});
	}

	story_text_box[1]->show();
	story_text_box[1]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx++]->hide();
			return 0;
		});

	// ���� ���Ҹ���.. �� �ʱ� ���� �پ�� ��!
	story_text_box[9]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			// 4��° �ε����� ��ǳ���̸� Ŭ���� ���ÿ� init�ϰ� ���� ��ǳ�� ��� X
			// 5��°�� 6��°�� ���� or ������ ���� �ؽ�Ʈ �����̹Ƿ� 
			// �ļ��� �Լ����� story_idx�� �ٷ�� �ش��ϴ� �ؽ�Ʈ ���ڸ� �����
			story_text_box[story_idx]->hide();
			init_go_to_school();
			return 0;
		});

	// �̷�����, ���� ������ �ϴٴ�...
	story_text_box[10]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			return 0;
		});

	// ��... �ƽ��ƽ��ߴ�!
	story_text_box[11]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			// ������ ��� Ŭ���ϸ� �������, ��� ��ȯ
			story_text_box[story_idx]->hide();
			return 0;
		});

	// �׷�, ���� ���� �ٳ�� / �׷�... �� ���� �װ� ����? // �� �л��̼��� �кθ𼼿�? /
	for (int i = 12; i <= 15; i++)
	{
		story_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				story_text_box[story_idx]->hide();
				life_scene(story_idx);
				// ��� ��� Ȥ�� �̽��Ϳ��װ� �ƴϸ� ����������Ʈ ��� ���̵�ƿ�
				if (story_idx != 12 && story_idx != 15)
				{
					life_cut();
					fade_out();
				}
				else if (story_idx == 12) // ��� ����� �׳� �Ѿ��
				{
					story_idx = 17;
					story_text_box[story_idx]->show();
					s2m_main();
				}
				else
				{
					// �̽��Ϳ��� ���� â ����
					story_text_box[++story_idx]->show();
				}

				return 0;
			});
	}

	// �̽��Ϳ���
	story_text_box[16]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			story_text_box[++story_idx]->show();
			run_teacher();
			return 0;
		});


	// 
	// ��������� �� �Ա� ~ �. ���� �б��� �Ѿ�� ���� �޽�
	//


	// ��� ��ȯ
	story_text_box[19]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			fade_out();
			return 0;
		});

	// Ʋ�� �׸�ã�� ����
	story_text_box[25]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			is_wrong_BGM->play();
			return 0;
		});

	// Ʋ���׸�ã�� ����
	for (int i = 26; i <= 28; i++)
	{
		story_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				is_wrong_BGM->stop();
				story_text_box[story_idx]->hide();
				is_wrong_finger->hide();
				if (story_idx != 28) // ����
				{
					story_idx = 31;
					is_wrong->setImage("Images/is_wrong_neg.jpg");
					story_text_box[story_idx]->show();
				}
				else
				{
					story_text_box[++story_idx]->show();
				}
				return 0;
			});
	}

	// ��ġ? ����...
	story_text_box[30]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			story_idx = 33;
			story_text_box[story_idx]->show();
			return 0;
		});
	// ��?
	story_text_box[31]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			life_cut();
			story_text_box[++story_idx]->show();
			return 0;
		});

	// ��Ȳ�� ����� ����
	story_text_box[32]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			is_wrong->setImage("Images/is_wrong.png");
			story_text_box[story_idx]->hide();
			story_idx = 33;
			story_text_box[story_idx]->show();
			return 0;
		});

	// ������ �ε���
	story_text_box[35]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			mock_test_main();
			return 0;
		});

}

// �������� Scene�� �ִ� Object���� ��ǥ���� �����ִ� �Լ�
// ù��°�� ���޹޴� ������ idx�� object �迭�� �ε���
// �ι�°�� ���޹޴� ������ var�� "�" object���� üũ
// var�� 0, 1, 2�� ������������, 0�� ��� prev, 1�� ��� next, 2�� ��� select�� ����
int dress_loc_x(int idx, int var)
{
	// �� ����â���� "����" ��ư����, �� 4���� �����Ǿ� �ִ�.
	// �ε��� ���ʴ�� �Ӹ�, ����, ����, �Ź��� �Ҵ�ȴ�.
	// x��ǥ�� 720, y��ǥ�� ���ʴ�� 720-((155+20)*(�ε�����ȣ+1))
	// 80x155px
	if (var == 0)	return 720;

	// �� ����â���� "����" ��ư, x��ǥ�� 1120, y��ǥ�� prev�� ����
	else if (var == 1) return 1120;

	// �÷��̾ ���� ���� ����� �̸������ �����ִ� â
	else if (var == 2) return 885;
}

// prev, next, select ��� y��ǥ�� �ε����� ���� �����ϴ�.
int dress_loc_y(int idx)
{
	return (720 - ((155 + 20) * (idx + 1)));
}

// sprintf���� idx�� ���޹ް� ��θ� ���� ��, idx�� ���� ���ڿ� ����
const char* dress_sort(int idx)
{
	if (idx == 0) return "Images/dress_head_%d.png";
	else if (idx == 1) return "Images/dress_top_%d.png";
	else if (idx == 2) return "Images/dress_bottom_%d.png";
	else if (idx == 3) return "Images/dress_shoe_%d.png";
}

// ���콺 �ݹ��� ��, �÷��̾ Ŭ���� object �迭�� ���� �ε��� ��ȯ
// � object���� 2��° �Ű����� var�� �����ϸ�, ���������� 0�� prev, 1�� next
int check_index(ObjectPtr image, int var)
{
	if (var == 0)
	{
		for (int i = 0; i < DRESS_VAR_SIZE; i++)
		{
			if (dress_prev_button[i] == image)
				return i;
		}
	}
	else if (var == 1)
	{
		for (int i = 0; i < DRESS_VAR_SIZE; i++)
		{
			if (dress_next_button[i] == image)
				return i;
		}
	}
	return 0;
}

// ���信 �´� ���� ���ߴ��� Ȯ��
bool check_dress()
{
	for (int i = 0; i < DRESS_VAR_SIZE; i++)
	{
		if (dress_select_index[i] != dress_answer_index[i])
			return false;
		
	}
	return true;
}
// �̽��Ϳ��׿� �´� ���� ���ߴ��� Ȯ��
bool check_easter_dress()
{
	for (int i = 0; i < DRESS_VAR_SIZE; i++)
	{
		if (dress_select_index[i] != dress_easter_index[i])
			return false;
	}
	return true;
}

void init_dress_room()
{
	// �ƹ�Ÿ�� ���� ���� ����� �̸������ �����ִ� â
	// ��ǥ���� 160, 20
	// 320x660px
	auto dress_preview = Object::create("Images/dress_preview.png", dress_room, 170, 130);
	dress_preview->setScale(0.8f);

	for (int i = 0; i < DRESS_VAR_SIZE; i++)
	{
		char sort_path[30] = { 0, };
		sprintf(sort_path, dress_sort(i), 0);

		dress_select_box[i] = Object::create("Images/dress_select_box.png", dress_room, 830, dress_loc_y(i));
		dress_select[i] = Object::create(sort_path, dress_room, dress_loc_x(i, 2), dress_loc_y(i));
		// �������� ������� ��ǥ�� ���Ƿ� ����
		dress_prev_button[i] = Object::create("Images/dress_room_prev.png", dress_room, dress_loc_x(i, 0), dress_loc_y(i));
		dress_prev_button[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				button_click->play();
				char path[30] = { 0, };
				// ���� ���� Ŭ���� "prev_button"�� dress_select �迭 �� ���° object���� check_index�� ���� Ȯ���Ѵ�. 
				// ������ �Ӹ�, ����, ����, �Ź� �� ��� ������ Ȯ���Ѵ�.
				// Ȯ���� �� �ش� object�� Ŭ�� Ƚ���� üũ�Ѵ�. "���� ��ư"�� ������ ���̹Ƿ� -- �����ڸ� ���� ����ؾ� �ϱ� �����̴�.
				// ���� üũ�� Ŭ�� Ƚ���� 0�̶��, -1�� ������ ��� �ε��� ������ �߻��ϹǷ� ȯ������ ���ư� �� �ְԲ� DRESS_VAR_SIZE�� ������ �ٲ��ش�.
				if (dress_select_index[check_index(button, 0)] == 0) dress_select_index[check_index(button, 0)] = DRESS_VAR_SIZE;
				sprintf(path, dress_sort(check_index(button, 0)), --dress_select_index[check_index(button, 0)]);

				dress_select[check_index(button, 0)]->setImage(path);
				return 0;
			});

		dress_next_button[i] = Object::create("Images/dress_room_next.png", dress_room, dress_loc_x(i, 1), dress_loc_y(i));
		dress_next_button[i]->setOnMouseCallback([&](auto button, auto x, auto y, auto action)-> bool
			{
				button_click->play();
				char path[30] = { 0, };
				if (dress_select_index[check_index(button, 1)] == DRESS_VAR_SIZE - 1) dress_select_index[check_index(button, 1)] = -1;
				sprintf(path, dress_sort(check_index(button, 1)), ++dress_select_index[check_index(button, 1)]);
				dress_select[check_index(button, 1)]->setImage(path);
				return 0;
			});

	}

	auto dress_confirm = Object::create("Images/dress_confirm.png", dress_room, 160, 40);
	dress_confirm->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			life_init();
			if (check_easter_dress()) // �̽��Ϳ��׸� story_idx �ϳ� �÷��ֱ�
			{
				// ���� idx�� 1�̰�, �̽��Ϳ��׿� idx�� 2�� �̸� ����
				story_idx++;
				story_text_box[story_idx]->show();
				// �̽��Ϳ��׿��� �������� üũ
				clear[0] = 1;
			}
			else
			{
				// ���� idx�� 1�̰�, �� ���� ��ǳ���� �̽��Ϳ��״ϱ� ��ŵ
				story_text_box[story_idx]->show();
				story_idx++;
				// ���� �ε��� �迭�̸� �������� üũ, �ƴϸ� ���з� üũ
				if (check_dress()) clear[0] = 1;
				else clear[0] = 0;
			}

			// �� ������ ��, go_to_school��
			go_to_school_main();
			return 0;
		});
}

void dress_room_main()
{
	init_scene();
	init_dress_room();
	init_obj();
	story_init();
	dress_room->enter();
}
