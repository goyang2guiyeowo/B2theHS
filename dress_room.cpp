#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace bangtal;

// 옷 종류
#define DRESS_VAR_SIZE 4

extern int story_idx;;
extern void life_init();
extern void life_scene(int idx);

extern void go_to_school_main();
extern void s2m_main();
extern void mock_test_main();

int clear[2] = {2, 2};

// 여기서 인덱스랑 박스 모두 선언
int story_idx = 1;
ObjectPtr story_text_box[37];

// 옷 선택창에서 어떤 옷을 고를지 미리 보여주는 창과 이전, 다음버튼
ObjectPtr dress_select[DRESS_VAR_SIZE];
ObjectPtr dress_select_box[DRESS_VAR_SIZE];
ObjectPtr dress_prev_button[DRESS_VAR_SIZE];
ObjectPtr dress_next_button[DRESS_VAR_SIZE];

ObjectPtr school_door_teacher;

// 옷 입히기 선택버튼 클릭 소리
auto button_click = Sound::create("Sound/button.mp3");

// 옷 입히기 선택창에서 prev 또는 next 클릭 횟수에 따른 이미지 index
int dress_select_index[4] = { 0, };

// 정답, 이스터에그 인덱스
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
	// 해당 장면의 마지막 텍스트 상자의 인덱스인지(클릭하면 장면 전환해야 하는지) 판단
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
				// 지저분하지만 일단 실행을 위해서
				else if (story_idx == 13 || story_idx == 14)
				{
					story_idx = 17;
					story_text_box[story_idx]->show();
					// 여기서 뭔가 생길듯
					s2m_main();
				}
				else if (next_scene(story_to_scene(story_idx)) == pre_is_wrong)
				{
					// 어쩌면 이렇게 사는 것도 괜찮을지도
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

// 텍스트 상자들 처음부터 끝까지 다 만들고, 하나 누르면 지금 보이는 텍스트 상자 없애고 다음 텍스트 상자 띄우는 콜백함수 등록

// 게임 전체 라이프포인트 배경과 개수, 깨지는 소리
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
// 텍스트 상자 맨 뒤로 가게 순서
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

// story_to_scene을 사용할 수 없는 경우
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

// 라이프포인트 잘라내기
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

// 나중에 이거 extern으로 받기
void story_init()
{
	for (int i = 1; i < 36; i++)
	{
		char story_path[50] = { 0, };
		sprintf(story_path, "Images/story_text (%d).png", i);
		// 모든 텍스트 상자는 이전 텍스트 상자가 클릭되기 전에는 안 보임

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

	// 벌써 종소리가.. 더 늦기 전에 뛰어야 돼!
	story_text_box[9]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			// 4번째 인덱스의 말풍선이면 클릭과 동시에 init하고 다음 말풍선 출력 X
			// 5번째와 6번째는 실패 or 성공에 따른 텍스트 상자이므로 
			// 후술할 함수에서 story_idx를 다루고 해당하는 텍스트 상자를 출력함
			story_text_box[story_idx]->hide();
			init_go_to_school();
			return 0;
		});

	// 이럴수가, 내가 지각을 하다니...
	story_text_box[10]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			return 0;
		});

	// 휴... 아슬아슬했다!
	story_text_box[11]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			// 성공한 경우 클릭하면 사라지고, 장면 전환
			story_text_box[story_idx]->hide();
			return 0;
		});

	// 그래, 늦지 말고 다녀라 / 그래... 넌 옷이 그게 뭐니? // 넌 학생이세요 학부모세요? /
	for (int i = 12; i <= 15; i++)
	{
		story_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				story_text_box[story_idx]->hide();
				life_scene(story_idx);
				// 모두 통과 혹은 이스터에그가 아니면 라이프포인트 까고 페이드아웃
				if (story_idx != 12 && story_idx != 15)
				{
					life_cut();
					fade_out();
				}
				else if (story_idx == 12) // 모두 통과면 그냥 넘어가기
				{
					story_idx = 17;
					story_text_box[story_idx]->show();
					s2m_main();
				}
				else
				{
					// 이스터에그 다음 창 띄우기
					story_text_box[++story_idx]->show();
				}

				return 0;
			});
	}

	// 이스터에그
	story_text_box[16]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			story_text_box[++story_idx]->show();
			run_teacher();
			return 0;
		});


	// 
	// 여기까지가 옷 입기 ~ 등교. 이후 학교로 넘어가서 우유 급식
	//


	// 장면 전환
	story_text_box[19]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			fade_out();
			return 0;
		});

	// 틀린 그림찾기 입장
	story_text_box[25]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			is_wrong_BGM->play();
			return 0;
		});

	// 틀린그림찾기 선택
	for (int i = 26; i <= 28; i++)
	{
		story_text_box[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				is_wrong_BGM->stop();
				story_text_box[story_idx]->hide();
				is_wrong_finger->hide();
				if (story_idx != 28) // 오답
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

	// 그치? 나만...
	story_text_box[30]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			story_idx = 33;
			story_text_box[story_idx]->show();
			return 0;
		});
	// 어?
	story_text_box[31]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			life_cut();
			story_text_box[++story_idx]->show();
			return 0;
		});

	// 당황한 기색이 역력
	story_text_box[32]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			is_wrong->setImage("Images/is_wrong.png");
			story_text_box[story_idx]->hide();
			story_idx = 33;
			story_text_box[story_idx]->show();
			return 0;
		});

	// 마지막 인덱스
	story_text_box[35]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			story_text_box[story_idx]->hide();
			mock_test_main();
			return 0;
		});

}

// 옷입히기 Scene에 있는 Object들의 좌표값을 구해주는 함수
// 첫번째로 전달받는 인자인 idx는 object 배열의 인덱스
// 두번째로 전달받는 인자인 var는 "어떤" object인지 체크
// var는 0, 1, 2로 정해져있으며, 0일 경우 prev, 1일 경우 next, 2일 경우 select로 설정
int dress_loc_x(int idx, int var)
{
	// 옷 선택창에서 "이전" 버튼으로, 총 4개로 구성되어 있다.
	// 인덱스 차례대로 머리, 상의, 하의, 신발이 할당된다.
	// x좌표는 720, y좌표는 차례대로 720-((155+20)*(인덱스번호+1))
	// 80x155px
	if (var == 0)	return 720;

	// 옷 선택창에서 "다음" 버튼, x좌표는 1120, y좌표는 prev와 동일
	else if (var == 1) return 1120;

	// 플레이어가 옷을 입은 모습을 미리보기로 보여주는 창
	else if (var == 2) return 885;
}

// prev, next, select 모두 y좌표는 인덱스에 따라 동일하다.
int dress_loc_y(int idx)
{
	return (720 - ((155 + 20) * (idx + 1)));
}

// sprintf에서 idx를 전달받고 경로를 붙일 때, idx에 따라 문자열 변경
const char* dress_sort(int idx)
{
	if (idx == 0) return "Images/dress_head_%d.png";
	else if (idx == 1) return "Images/dress_top_%d.png";
	else if (idx == 2) return "Images/dress_bottom_%d.png";
	else if (idx == 3) return "Images/dress_shoe_%d.png";
}

// 마우스 콜백할 때, 플레이어가 클릭한 object 배열의 원소 인덱스 반환
// 어떤 object인지 2번째 매개변수 var로 전달하며, 마찬가지로 0은 prev, 1은 next
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

// 정답에 맞는 옷을 택했는지 확인
bool check_dress()
{
	for (int i = 0; i < DRESS_VAR_SIZE; i++)
	{
		if (dress_select_index[i] != dress_answer_index[i])
			return false;
		
	}
	return true;
}
// 이스터에그에 맞는 옷을 택했는지 확인
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
	// 아바타가 옷을 입은 모습을 미리보기로 보여주는 창
	// 좌표값은 160, 20
	// 320x660px
	auto dress_preview = Object::create("Images/dress_preview.png", dress_room, 170, 130);
	dress_preview->setScale(0.8f);

	for (int i = 0; i < DRESS_VAR_SIZE; i++)
	{
		char sort_path[30] = { 0, };
		sprintf(sort_path, dress_sort(i), 0);

		dress_select_box[i] = Object::create("Images/dress_select_box.png", dress_room, 830, dress_loc_y(i));
		dress_select[i] = Object::create(sort_path, dress_room, dress_loc_x(i, 2), dress_loc_y(i));
		// 옷입히기 골격인형 좌표는 임의로 설정
		dress_prev_button[i] = Object::create("Images/dress_room_prev.png", dress_room, dress_loc_x(i, 0), dress_loc_y(i));
		dress_prev_button[i]->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
			{
				button_click->play();
				char path[30] = { 0, };
				// 내가 지금 클릭한 "prev_button"이 dress_select 배열 중 몇번째 object인지 check_index를 통해 확인한다. 
				// 요컨대 머리, 상의, 하의, 신발 중 어느 것인지 확인한다.
				// 확인한 뒤 해당 object의 클릭 횟수를 체크한다. "이전 버튼"을 누르는 것이므로 -- 연산자를 통해 출력해야 하기 때문이다.
				// 만약 체크한 클릭 횟수가 0이라면, -1로 감소할 경우 인덱스 오류가 발생하므로 환형으로 돌아갈 수 있게끔 DRESS_VAR_SIZE로 변수를 바꿔준다.
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
			if (check_easter_dress()) // 이스터에그면 story_idx 하나 늘려주기
			{
				// 현재 idx가 1이고, 이스터에그용 idx는 2라서 미리 증가
				story_idx++;
				story_text_box[story_idx]->show();
				// 이스터에그여도 성공으로 체크
				clear[0] = 1;
			}
			else
			{
				// 현재 idx가 1이고, 그 다음 말풍선은 이스터에그니까 스킵
				story_text_box[story_idx]->show();
				story_idx++;
				// 정답 인덱스 배열이면 성공으로 체크, 아니면 실패로 체크
				if (check_dress()) clear[0] = 1;
				else clear[0] = 0;
			}

			// 옷 입히기 끝, go_to_school로
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
