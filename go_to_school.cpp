#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// 등교 미니게임 PLAYER, TRAP object 가로세로 픽셀길이
#define PLAYER_SIZE 40
#define TRAP_SIZE 40
// 등교 미니게임 장애물 속도, 최소속도(최대속도는 속도 + 최소속도)
#define TRAP_SPEED 10
#define MIN_TRAP_SPEED 5

using namespace bangtal;

// 기본 세트
extern int story_idx;
extern int clear[2];
extern ObjectPtr story_text_box[37];
extern int story_idx;
extern TimerPtr TeacherTimer;
extern void fade_out();

extern void s2m_main();

// 등굣길 플레이어 object와 장애물 object, 타이머가 눈에 잘 보이게끔 하는 사각형
extern ScenePtr go_to_school;
extern ScenePtr post_go_to_school;

TimerPtr TeacherTimer;
extern float animationTime;

// 선도부장 선생님 애니메이션을 위한 좌표
int teacher_x = 700;

extern ObjectPtr school_door_teacher;

ObjectPtr player, trap, timer_box;

extern void life_scene_2(ScenePtr scene);

// 등굣길 시작 플레이어 좌표와 장애물 좌표, 말풍선 좌표
float playerX = 0, playerY = 0;
// float playerX = 500, playerY = 400;
float trapX = 1000;
float trapY = 340;
float enemy_box_X = 0, enemy_box_Y = 0;
float player_box_X = 0, player_box_Y = 0;
int school_box_idx = 0;

auto school_bell = Sound::create("Sound/school_bell.mp3");

// 등교 미니게임 시작 시간과 끝나는 시간
clock_t school_start_time, school_end_time;

// 등교 미니게임 시간 제한과 타이머
float go_to_school_time_limit = 37;

TimerPtr go_to_school_game_timer;

// 등교 미니게임 장애물이 움직이는 타이머와 움직이는 시간 단위
float trap_move_time = 0.05f;

// 함정이 움직이는 속도, 함수 호출할 때 난수 받아서 init
int trapX_speed = 12, trapY_speed = 12;
TimerPtr go_to_school_trap_timer;
ObjectPtr school_enemy_box[3];
ObjectPtr school_player_box[3];

// 순서
bool check_easter_dress();
extern void life_cut();

// 옷입히기와 등교결과에 따른 스토리인덱스값 반환
void check_school_door()
{
	// 이스터에그면 바로 통과, 그 외에는 모두 통과, 옷만 통과, 등교만 통과, 모두 실패
	if (check_easter_dress()) story_idx = 15;
	else
	{
		// 지각은 안했지만 옷을 제대로 안 입은 경우
		if (clear[0] == 0 && clear[1] == 1) story_idx = 13;
		// 옷도 안 입고 오고 지각도 하면
		else if (clear[0] == 0 && clear[1] == 0) story_idx = 14;
		// 옷은 제대로 입었는데 지각한 경우
		else if (clear[0] == 1 && clear[1] == 0) story_idx = 14;
		// 둘 다 만족한 경우
		else story_idx = 12;
	}

	printf("%d %d", clear[0], clear[1]);
}


// 이스터에그
void run_teacher()
{
	TeacherTimer = Timer::create(0.001f);
	TeacherTimer->setOnTimerCallback([&](auto)->bool
		{
			if (teacher_x < 1400)
			{
				teacher_x += 3;
				school_door_teacher->locate(post_go_to_school, teacher_x, 0);
				TeacherTimer->set(0.001f);
				TeacherTimer->start();
			}
			else s2m_main();
			// else pre_is_wrong->enter();
			return 0;
		});
	TeacherTimer->start();
}

void go_to_school_game_over() // 게임 오버된 것으로 넘기기
{
	go_to_school_trap_timer->stop();
	go_to_school_game_timer->stop();
	school_enemy_box[school_box_idx]->hide();
	school_player_box[school_box_idx]->hide();
	player->hide();
	trap->hide();
	school_bell->stop();
	timer_box->hide();
	life_cut();

	if (clear[1] == 2)
	{
		story_text_box[++story_idx]->show();
		clear[1] = 0;
	}
	hideTimer();
	fade_out();
}

void check_school_time_over() // 타임오버되었는지 확인
{
	if ((school_end_time - school_start_time) / CLOCKS_PER_SEC >= go_to_school_time_limit && clear[1] == 2)
	{
		go_to_school_game_over();
	}
}

void school_game_clear() // 게임 클리어 조건을 만족했는지 확인한 뒤, 참이면 호출
{
	school_enemy_box[school_box_idx]->hide();
	school_player_box[school_box_idx]->hide();
	player->hide();
	trap->hide();
	// 11번째 메시지 박스가 클리어 내용이므로 2 더해준다, clear_check은 키 입력마다 하므로 한번만
	if (clear[1] == 2)
		story_idx += 2;
	// 2번째 게임 클리어
	clear[1] = 1;
	story_text_box[story_idx]->show();
	school_bell->stop();
	timer_box->hide();
	go_to_school_game_timer->stop();
	go_to_school_trap_timer->stop();
	hideTimer();
	fade_out();
}

bool is_player_on_trap() // 함정 위치에 닿았는지 체크
{
	if (playerX + PLAYER_SIZE >= trapX && playerX + PLAYER_SIZE <= trapX + TRAP_SIZE + PLAYER_SIZE && playerY + PLAYER_SIZE >= trapY && playerY + PLAYER_SIZE <= trapY + TRAP_SIZE + PLAYER_SIZE) return true;

	else return false;
}

void init_go_to_school_timer()
{
	go_to_school_game_timer = Timer::create(go_to_school_time_limit);
	school_start_time = clock();
	go_to_school_game_timer->set(go_to_school_time_limit);
	go_to_school_game_timer->start();

	for (int i = 0; i < 3; i++)
	{
		// 상자들 출력한다. 얼추 200x150px로 보면 된다.
		char enemy_path[40] = { 0, };
		sprintf(enemy_path, "Images/school_enemy_box_%d.png", i);
		school_enemy_box[i] = Object::create(enemy_path, go_to_school, 0, 0, false);

		char player_path[40] = { 0, };
		sprintf(player_path, "Images/school_player_box_%d.png", i);
		school_player_box[i] = Object::create(player_path, go_to_school, 0, 0, false);
	}

	showTimer(go_to_school_game_timer);

	go_to_school_trap_timer = Timer::create(trap_move_time);
	go_to_school_trap_timer->setOnTimerCallback([&](auto)->bool
		{
			school_enemy_box[school_box_idx]->show();
			school_end_time = clock();
			check_school_time_over();
			trapX -= trapX_speed;
			trapY -= trapY_speed;
			enemy_box_X = trapX + PLAYER_SIZE;
			enemy_box_Y = trapY + PLAYER_SIZE;
			school_enemy_box[school_box_idx]->locate(go_to_school, enemy_box_X, enemy_box_Y);

			if (is_player_on_trap()) // 플레이어와 닿았는지 체크
			{	
				playerX = trapX - PLAYER_SIZE;
				playerY = trapY;
				// 플레이어의 x좌표에서 200 빼주고, y좌표는 플레이어 크기 더해주면 된다.
				player_box_X = playerX - 200;
				player_box_Y = playerY + PLAYER_SIZE;
				// 함정도 마찬가지(오른쪽이니까 더해주자)
				school_player_box[school_box_idx]->locate(go_to_school, player_box_X, player_box_Y);
				school_player_box[school_box_idx]->show();
				player->locate(go_to_school, playerX, playerY);
			}
			trap->locate(go_to_school, trapX, trapY);
			if (trapY > 0) // 함정이 화면 밑바닥 찍을 때까지 반복
			{
				go_to_school_trap_timer->set(trap_move_time);
				go_to_school_trap_timer->start();
			}
			else // 함정이 화면 밖으로 사라지면 난수 받아서 속도 새로 설정
			{
				int trap_relocate_x = 900 + (int)(rand() % 300);
				trapX_speed = rand() % TRAP_SPEED + MIN_TRAP_SPEED;
				trapY_speed = rand() % TRAP_SPEED + MIN_TRAP_SPEED;
				trapX = trap_relocate_x;
				trapY = -0.2 * trap_relocate_x + 540;
				trap->locate(go_to_school, trapX, trapY);
				// 땅까지 왔으면 플레이어 박스 없애고, 인덱스 하나 늘려주기
				school_enemy_box[school_box_idx]->hide();
				school_player_box[school_box_idx++]->hide();
				if (school_box_idx > 2) school_box_idx %= 3;
				go_to_school_trap_timer->set(trap_move_time);
				go_to_school_trap_timer->start();
			}
			return 0;
		});

	go_to_school_trap_timer->start();

}

// option이 0이면 x를, 1이면 y를 체크함
bool check_movable(int option)
{
	if (option == 0)
	{	// 해당 Y 좌표에서 움직일 수 있는 최대 X좌표값
		float check_loc_x = (playerY - 45 + PLAYER_SIZE) / 0.38;
		// 여기에 player의 x좌표, y좌표를 받아서 움직일 수 있는 범위 내인지 확인
		// 담벼락의 시작 좌표는 0,45고, 끝 좌표는 847,370
		// 계산해보면 y = 0.38x + 45의 일차함수
		// 플레이어의 y 위치값을 함수에 대입하여 나온 x값이, 플레이어 x좌표보다 "작으면" 움직일 수 있음
		// 이 때 체크하는 y좌표값은 플레이어 이미지의 y좌표(왼쪽 아래)가 아닌 머리부분(왼쪽 위)이기 때문에
		// player object의 세로 길이인 40을 더한다.

		if (playerX > check_loc_x) return true;
		else return false;
	}

	else if (option == 1)
	{
		// 해당 X 좌표에서 움직일 수 있는 최대 y좌표값
		float check_loc_y = playerX * 0.38 + 45;
		if (playerY + PLAYER_SIZE < check_loc_y) return true;
		else return false;
	}
}

// option은 0이면 x좌표, 1이면 y좌표 체크
bool school_clear_check()
// 골인지점의 시작좌표와 끝좌표는 각각 (847,370) ~ (1280,280)
// 일차함수로 변환하면 약 y = -0.2x + 540
// player의 좌표를 받아서 동일하게 체크한다. 다만 y좌표에 대해서는 체크할 필요가 없다. x좌표만 체크하자.
{
	float check_loc_x = (playerY - 540 + PLAYER_SIZE) / -0.2;
	if (playerX > check_loc_x) return true;
	else return false;
}

void init_go_to_school()
{
	school_bell->play();

	life_scene_2(go_to_school);
	
	timer_box = Object::create("Images/timerbox.png", go_to_school, 590, 660);

	player = Object::create("Images/player.png", go_to_school, 0, 0);
	player->setScale(0.4f);

	trap = Object::create("Images/trap.png", go_to_school, trapX, trapY);
	trap->setScale(0.4);

	init_go_to_school_timer();

	go_to_school->setOnKeyboardCallback([&](ScenePtr go_to_school, KeyCode key, bool pressed)-> bool
		{
			if (key == KeyCode::KEY_RIGHT_ARROW)
			{
				playerX += 6;
				player->locate(go_to_school, playerX, playerY);

				if (school_clear_check()) school_game_clear();
			}
			else if (key == KeyCode::KEY_LEFT_ARROW)
			{
				if (check_movable(0))
				{
					playerX -= 6;
					player->locate(go_to_school, playerX, playerY);
				}
				if (school_clear_check()) school_game_clear();

			}
			else if (key == KeyCode::KEY_UP_ARROW)
			{
				if (check_movable(1))
				{
					playerY += 3;
					player->locate(go_to_school, playerX, playerY);
				}
				if (school_clear_check()) school_game_clear();
			}
			else if (key == KeyCode::KEY_DOWN_ARROW)
			{
				if (check_movable(1))
				{
					playerY -= 3;
					player->locate(go_to_school, playerX, playerY);
				}
				if (school_clear_check()) school_game_clear();
			}
			return 0;
		});
}

void go_to_school_main()
{
}