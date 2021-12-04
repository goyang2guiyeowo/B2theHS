#include <bangtal>
#include <cstdlib>
#include <random>
#include <ctime>
#include <windows.h>
#include <cmath>
using namespace bangtal;

#define MENU_SIZE 150
#define TRAY_LENGTH 303
#define TRAY_WIDTH 144
#define CHAM_LENGTH 102
#define CHAM_WIDTH 142

ObjectPtr rush_start;
ObjectPtr kodari_hate_message;
ObjectPtr rush_great;
ObjectPtr game_clear;

TimerPtr meal_rush_timer;
TimerPtr menu_timer;

extern ObjectPtr life_point_bar;
extern ObjectPtr life_point[4];
extern void life_scene_2(ScenePtr scene);
extern void life_cut();
extern void is_wrong_main();
extern int life;

//�÷��� bgm*****************************************************************************************************************************************************
auto meal_rush_bgm = Sound::create("mealsound/�޽Ľǻ���.mp3");
auto eat_menu = Sound::create("mealsound/yum.mp3");
auto kodari_sound = Sound::create("mealsound/ohno.mp3");
auto meal_start_sound = Sound::create("mealsound/�޽Ľ���.mp3");
auto meal_rush_clear_sound = Sound::create("mealsound/�޽ļ���.mp3");
auto meal_rush_fail_sound = Sound::create("mealsound/�޽Ľ���.mp3");


float menu_drop_time = 0.05f;
float school_meal_time = 20.f;

// ���� ù ȭ��
auto meal_screen1 = Scene::create("����ȭ��", "mealimages/menu.png");
auto meal_screen2 = Scene::create("����ȭ��", "mealimages/schoolmeal_screen.jpg");

int menucheck[6] = { 0, };

int move = 30;	// �÷��̾� 1ȸ �̵� X��
int menu_count = 0;	//�޴� ȹ�� count. �ڴٸ��� ������ 5���� ��� ������ ���� Ŭ����

int score = 0;

constexpr int MIN = 1;	
constexpr int MAX = 1180;	// �޴� X�� ���� ���� ����. 1~1180

auto studentX = 530, studentY = 6;
auto student = Object::create("mealimages/tray.png", meal_screen2, studentX, studentY);	// �÷��̾�(����) ����

int su = 550;	// �޴� Y�� ����. X���� ���� �Ҵ�->�����Ҷ����� �ٸ� ���� �������� �ߴµ�, ������ ������ X (Graderush������ ��������)
auto SpaX = rand()% MAX, CurX = rand() % MAX, DonX = rand() % MAX, JangX = rand() % MAX, ChamX = rand() % MAX, KodariX = rand() % MAX;
auto SpaY = su, CurY = su, DonY = su, JangY = su, ChamY = su, KodariY = su;

auto Spa = Object::create("mealimages/spa.png", meal_screen2, SpaX, SpaY);

auto Cur = Object::create("mealimages/cur.png", meal_screen2, CurX, CurY);

auto Don = Object::create("mealimages/don.png", meal_screen2, DonX, DonY);

auto Jang = Object::create("mealimages/jang.png", meal_screen2, JangX, JangY);

auto Cham = Object::create("mealimages/cham.png", meal_screen2, ChamX, ChamY);

auto Kodari = Object::create("mealimages/kodari.png", meal_screen2, KodariX, KodariY);

auto GAMEOVER = Object::create("mealimages/GAMEOVER.png", meal_screen2, 0, 250, false);

void run_time_over()
{
	menu_timer->stop();
	meal_rush_timer->stop();
	GAMEOVER->show();
	meal_rush_bgm->stop();
	hideTimer();
	// meal_rush_fail_sound->play();
	life_cut();
}


bool is_rush_game_clear()	//���� Ŭ���� ���� Ȯ��(count 5ȸ)
{
	if (menu_count >= 5) return true;
	else return false;
}

void rush_game_clear()	//�޽ķ� ���� Ŭ����
{
	menu_timer->stop();
	meal_rush_timer->stop();
	meal_rush_bgm->stop();
	meal_rush_clear_sound->play();
	rush_great->show();
	hideTimer();
}

void kodari_over()	//�ڴٸ��� ���� ���
{
	menu_timer->stop();
	meal_rush_timer->stop();
	meal_rush_bgm->stop();
	kodari_sound->play();
	hideTimer();
	// GAMEOVER->show();
	life_cut();
}

// ���� ��ư ������ ���� ��ư ������� ���� ����ȭ������ �̵�, Ÿ�̸� ���� 
void start_game()
{
	rush_start->hide();
	meal_screen2->enter();

	meal_rush_timer = Timer::create(school_meal_time);
	meal_rush_timer->set(school_meal_time);
	meal_rush_timer->start();

	meal_rush_timer->setOnTimerCallback([&](TimerPtr)-> bool {
		// 1���� ������ ���� �����ǵ��� ����.
		run_time_over();
		return true;
		});
	showTimer(meal_rush_timer);
	
	meal_rush_bgm->play();

	//���� Y��ǥ ��>�Ʒ�

	menu_timer = Timer::create(menu_drop_time);
	menu_timer->setOnTimerCallback([&](auto)->bool
		{
			if (menucheck[5] != 1)
			{
				SpaY -= 7;
				Spa->locate(meal_screen2, SpaX, SpaY);

				CurY -= 4;
				Cur->locate(meal_screen2, CurX, CurY);

				ChamY -= 2;
				Cham->locate(meal_screen2, ChamX, ChamY);

				KodariY -= 3.2;
				Kodari->locate(meal_screen2, KodariX, KodariY);

				JangY -= 1.3;
				Jang->locate(meal_screen2, JangX, JangY);

				DonY -= 5;
				Don->locate(meal_screen2, DonX, DonY);

				if (is_student_get_spa())
				{
					Spa->hide();
					if (menucheck[0] != 1)
					{
						menu_count += 1;
						menucheck[0] = 1;
						eat_menu->play();

					}

				}

				if (is_student_get_cur())
				{
					Cur->hide();
					if (menucheck[1] != 1)
					{
						menu_count += 1;
						menucheck[1] = 1;
						eat_menu->play();
					}

				}

				if (is_student_get_don())
				{
					Don->hide();
					if (menucheck[2] != 1)
					{
						menu_count += 1;
						menucheck[2] = 1;
						eat_menu->play();
					}
				}

				if (is_student_get_jang())
				{
					Jang->hide();
					if (menucheck[3] != 1)
					{
						menu_count += 1;
						menucheck[3] = 1;
						eat_menu->play();
					}
				}

				if (is_student_get_cham())
				{
					Cham->hide();
					if (menucheck[4] != 1)
					{
						menu_count += 1;
						menucheck[4] = 1;
						eat_menu->play();
					}

				}

				if (is_student_get_kodari())
				{
					if (menucheck[5] != 1)
					{
						menucheck[5] = 1;
						kodari_hate_message->show();
						kodari_over();
					}
				}



				if (menu_count != 5)
				{
					menu_timer->set(menu_drop_time);
					menu_timer->start();
				}
			}
			return 0;
		});
		menu_timer->start();
	
}


bool is_student_get_spa()	//���İ�Ƽ
{
	if (studentX + TRAY_LENGTH >= SpaX && studentX + TRAY_LENGTH <= SpaX + MENU_SIZE + TRAY_LENGTH && studentY + TRAY_WIDTH >= SpaY && studentY + TRAY_WIDTH <= SpaY + MENU_SIZE + TRAY_WIDTH)
		return true;
	else return false;
}


bool is_student_get_cur()	//ī��
{
	if (studentX + TRAY_LENGTH >= CurX && studentX + TRAY_LENGTH <= CurX + MENU_SIZE + TRAY_LENGTH && studentY + TRAY_WIDTH >= CurY && studentY + TRAY_WIDTH <= CurY + MENU_SIZE + TRAY_WIDTH)
		return true;
	else return false;
}

bool is_student_get_don()	//���
{
	if (studentX + TRAY_LENGTH >= DonX && studentX + TRAY_LENGTH <= DonX + MENU_SIZE + TRAY_LENGTH && studentY + TRAY_WIDTH >= DonY && studentY + TRAY_WIDTH <= DonY + MENU_SIZE + TRAY_WIDTH)
		return true;
	else return false;
}

bool is_student_get_jang()	//����
{
	if (studentX + TRAY_LENGTH >= JangX && studentX + TRAY_LENGTH <= JangX + MENU_SIZE + TRAY_LENGTH && studentY + TRAY_WIDTH >= JangY && studentY + TRAY_WIDTH <= JangY + MENU_SIZE + TRAY_WIDTH)
		return true;
	else return false;
}

bool is_student_get_cham()	//���ٷ�
{
	if (studentX + TRAY_LENGTH >= ChamX && studentX + TRAY_LENGTH <= ChamX + CHAM_LENGTH + TRAY_LENGTH && studentY + TRAY_WIDTH >= ChamY && studentY + TRAY_WIDTH <= ChamY + CHAM_WIDTH + TRAY_WIDTH)
		return true;
	else return false;
}

bool is_student_get_kodari()	//�ڴٸ���->������ ����
{
	if (studentX + TRAY_LENGTH >= KodariX && studentX + TRAY_LENGTH <= KodariX + MENU_SIZE + TRAY_LENGTH && studentY + TRAY_WIDTH >= KodariY && studentY + TRAY_WIDTH <= KodariY + MENU_SIZE + TRAY_WIDTH)
		return true;
	else return false;
}

void school_lunch_main()
{
	std::srand(std::time(nullptr));	
	srand((unsigned int)time(NULL));	//�޴� x��ǥ ��������
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	// ���� ��ư ������ ��Ÿ���� �÷��� ȭ��
	// Ű�����ݹ�-�¿� ���� ����.
	meal_screen2->setOnKeyboardCallback([&](ScenePtr meal_screen2, KeyCode key, bool pressed)->bool {
		if (key == KeyCode::KEY_RIGHT_ARROW) {
			studentX = studentX + move;		//move: 30, �ӵ� ������ ���� ���� �����ϰ� ���ݾ� ��������. 30�� ���� �ӵ�
			student->locate(meal_screen2, studentX, studentY);
			if (is_rush_game_clear())
			{
				rush_game_clear();
			}
	
			return true;
		}
		else if (key == KeyCode::KEY_LEFT_ARROW) {
			studentX = studentX - move;
			student->locate(meal_screen2, studentX, studentY);
			if (is_rush_game_clear())
			{
				rush_game_clear();
			}
			
			return true;
		}
		});

	//�ڴٸ� �Ⱦ� �޽���
	kodari_hate_message = Object::create("mealimages/kodarihate.png", meal_screen2, 0, 0);
	kodari_hate_message->hide();
	kodari_hate_message->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			meal_rush_fail_sound->stop();
			is_wrong_main();
			return 0;
		});

	GAMEOVER->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			is_wrong_main();
			return 0;
		});

	//���� ���� �ؽ�Ʈ
	rush_great = Object::create("mealimages/rush_great.png", meal_screen2, 0, 0);
	rush_great->hide();
	rush_great->setOnMouseCallback([&](ObjectPtr button, auto x, auto y, auto action)-> bool
		{
			meal_rush_clear_sound->stop();
			is_wrong_main();
			return 0;
		});

	//���� ���� �޽���(CLEAR)
	// game_clear = Object::create("mealimages/GAMECLEAR.png", meal_screen2, 0, 250);
	// game_clear->hide();
	
	//����ȭ�� �Լ��� �̵�
	rush_start = Object::create("mealimages/meal_start.png", meal_screen1, 890, 100);
	rush_start->setScale(0.25f);
	rush_start->setOnMouseCallback([&](auto, auto, auto, auto)->bool
		{
			meal_start_sound->play();
			start_game();
			return true;
		});
	

	life_scene_2(meal_screen2);
	// startGame(meal_screen1);
	meal_screen1->enter();
}

