#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bangtal>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// � �̴ϰ��� PLAYER, TRAP object ���μ��� �ȼ�����
#define PLAYER_SIZE 40
#define TRAP_SIZE 40
// � �̴ϰ��� ��ֹ� �ӵ�, �ּҼӵ�(�ִ�ӵ��� �ӵ� + �ּҼӵ�)
#define TRAP_SPEED 10
#define MIN_TRAP_SPEED 5

using namespace bangtal;

// �⺻ ��Ʈ
extern int story_idx;
extern int clear[2];
extern ObjectPtr story_text_box[37];
extern int story_idx;
extern TimerPtr TeacherTimer;
extern void fade_out();

extern void s2m_main();

// ��� �÷��̾� object�� ��ֹ� object, Ÿ�̸Ӱ� ���� �� ���̰Բ� �ϴ� �簢��
extern ScenePtr go_to_school;
extern ScenePtr post_go_to_school;

TimerPtr TeacherTimer;
extern float animationTime;

// �������� ������ �ִϸ��̼��� ���� ��ǥ
int teacher_x = 700;

extern ObjectPtr school_door_teacher;

ObjectPtr player, trap, timer_box;

extern void life_scene_2(ScenePtr scene);

// ��� ���� �÷��̾� ��ǥ�� ��ֹ� ��ǥ, ��ǳ�� ��ǥ
float playerX = 0, playerY = 0;
// float playerX = 500, playerY = 400;
float trapX = 1000;
float trapY = 340;
float enemy_box_X = 0, enemy_box_Y = 0;
float player_box_X = 0, player_box_Y = 0;
int school_box_idx = 0;

auto school_bell = Sound::create("Sound/school_bell.mp3");

// � �̴ϰ��� ���� �ð��� ������ �ð�
clock_t school_start_time, school_end_time;

// � �̴ϰ��� �ð� ���Ѱ� Ÿ�̸�
float go_to_school_time_limit = 37;

TimerPtr go_to_school_game_timer;

// � �̴ϰ��� ��ֹ��� �����̴� Ÿ�̸ӿ� �����̴� �ð� ����
float trap_move_time = 0.05f;

// ������ �����̴� �ӵ�, �Լ� ȣ���� �� ���� �޾Ƽ� init
int trapX_speed = 12, trapY_speed = 12;
TimerPtr go_to_school_trap_timer;
ObjectPtr school_enemy_box[3];
ObjectPtr school_player_box[3];

// ����
bool check_easter_dress();
extern void life_cut();

// ��������� ������ ���� ���丮�ε����� ��ȯ
void check_school_door()
{
	// �̽��Ϳ��׸� �ٷ� ���, �� �ܿ��� ��� ���, �ʸ� ���, ��� ���, ��� ����
	if (check_easter_dress()) story_idx = 15;
	else
	{
		// ������ �������� ���� ����� �� ���� ���
		if (clear[0] == 0 && clear[1] == 1) story_idx = 13;
		// �ʵ� �� �԰� ���� ������ �ϸ�
		else if (clear[0] == 0 && clear[1] == 0) story_idx = 14;
		// ���� ����� �Ծ��µ� ������ ���
		else if (clear[0] == 1 && clear[1] == 0) story_idx = 14;
		// �� �� ������ ���
		else story_idx = 12;
	}

	printf("%d %d", clear[0], clear[1]);
}


// �̽��Ϳ���
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

void go_to_school_game_over() // ���� ������ ������ �ѱ��
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

void check_school_time_over() // Ÿ�ӿ����Ǿ����� Ȯ��
{
	if ((school_end_time - school_start_time) / CLOCKS_PER_SEC >= go_to_school_time_limit && clear[1] == 2)
	{
		go_to_school_game_over();
	}
}

void school_game_clear() // ���� Ŭ���� ������ �����ߴ��� Ȯ���� ��, ���̸� ȣ��
{
	school_enemy_box[school_box_idx]->hide();
	school_player_box[school_box_idx]->hide();
	player->hide();
	trap->hide();
	// 11��° �޽��� �ڽ��� Ŭ���� �����̹Ƿ� 2 �����ش�, clear_check�� Ű �Է¸��� �ϹǷ� �ѹ���
	if (clear[1] == 2)
		story_idx += 2;
	// 2��° ���� Ŭ����
	clear[1] = 1;
	story_text_box[story_idx]->show();
	school_bell->stop();
	timer_box->hide();
	go_to_school_game_timer->stop();
	go_to_school_trap_timer->stop();
	hideTimer();
	fade_out();
}

bool is_player_on_trap() // ���� ��ġ�� ��Ҵ��� üũ
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
		// ���ڵ� ����Ѵ�. ���� 200x150px�� ���� �ȴ�.
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

			if (is_player_on_trap()) // �÷��̾�� ��Ҵ��� üũ
			{	
				playerX = trapX - PLAYER_SIZE;
				playerY = trapY;
				// �÷��̾��� x��ǥ���� 200 ���ְ�, y��ǥ�� �÷��̾� ũ�� �����ָ� �ȴ�.
				player_box_X = playerX - 200;
				player_box_Y = playerY + PLAYER_SIZE;
				// ������ ��������(�������̴ϱ� ��������)
				school_player_box[school_box_idx]->locate(go_to_school, player_box_X, player_box_Y);
				school_player_box[school_box_idx]->show();
				player->locate(go_to_school, playerX, playerY);
			}
			trap->locate(go_to_school, trapX, trapY);
			if (trapY > 0) // ������ ȭ�� �عٴ� ���� ������ �ݺ�
			{
				go_to_school_trap_timer->set(trap_move_time);
				go_to_school_trap_timer->start();
			}
			else // ������ ȭ�� ������ ������� ���� �޾Ƽ� �ӵ� ���� ����
			{
				int trap_relocate_x = 900 + (int)(rand() % 300);
				trapX_speed = rand() % TRAP_SPEED + MIN_TRAP_SPEED;
				trapY_speed = rand() % TRAP_SPEED + MIN_TRAP_SPEED;
				trapX = trap_relocate_x;
				trapY = -0.2 * trap_relocate_x + 540;
				trap->locate(go_to_school, trapX, trapY);
				// ������ ������ �÷��̾� �ڽ� ���ְ�, �ε��� �ϳ� �÷��ֱ�
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

// option�� 0�̸� x��, 1�̸� y�� üũ��
bool check_movable(int option)
{
	if (option == 0)
	{	// �ش� Y ��ǥ���� ������ �� �ִ� �ִ� X��ǥ��
		float check_loc_x = (playerY - 45 + PLAYER_SIZE) / 0.38;
		// ���⿡ player�� x��ǥ, y��ǥ�� �޾Ƽ� ������ �� �ִ� ���� ������ Ȯ��
		// �㺭���� ���� ��ǥ�� 0,45��, �� ��ǥ�� 847,370
		// ����غ��� y = 0.38x + 45�� �����Լ�
		// �÷��̾��� y ��ġ���� �Լ��� �����Ͽ� ���� x����, �÷��̾� x��ǥ���� "������" ������ �� ����
		// �� �� üũ�ϴ� y��ǥ���� �÷��̾� �̹����� y��ǥ(���� �Ʒ�)�� �ƴ� �Ӹ��κ�(���� ��)�̱� ������
		// player object�� ���� ������ 40�� ���Ѵ�.

		if (playerX > check_loc_x) return true;
		else return false;
	}

	else if (option == 1)
	{
		// �ش� X ��ǥ���� ������ �� �ִ� �ִ� y��ǥ��
		float check_loc_y = playerX * 0.38 + 45;
		if (playerY + PLAYER_SIZE < check_loc_y) return true;
		else return false;
	}
}

// option�� 0�̸� x��ǥ, 1�̸� y��ǥ üũ
bool school_clear_check()
// ���������� ������ǥ�� ����ǥ�� ���� (847,370) ~ (1280,280)
// �����Լ��� ��ȯ�ϸ� �� y = -0.2x + 540
// player�� ��ǥ�� �޾Ƽ� �����ϰ� üũ�Ѵ�. �ٸ� y��ǥ�� ���ؼ��� üũ�� �ʿ䰡 ����. x��ǥ�� üũ����.
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