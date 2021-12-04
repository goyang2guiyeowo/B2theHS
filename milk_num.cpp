#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <bangtal>
using namespace bangtal;
#include <time.h>

ScenePtr classroom, milk_scene, milk_check_scene, milk_result_scene;
ObjectPtr milk_box, bigger, smaller, milk_text[14], milk_my_num[2], milk_my_num2[2], milk_your_num[2];
TimerPtr timer;
SoundPtr milk_kids, milk_bgm, milk_fail, milk_win;

char message[50];
int my_num, your_num, text_i = 0;
bool issmaller;

extern void milk_2_lunch_main();
extern void init_ending_scene();
extern void life_scene_2(ScenePtr scene);
extern void life_cut();

//��� �����ִ� �Լ�
void milk_result() {
	bigger->hide(); smaller->hide(); //��ư�� �����
	milk_result_scene->enter(); //��� Ȯ�� ȭ������ �Ѿ��
	milk_bgm->stop();

	//�� ������ ���� �� ���� ����
	char num_path[20];
	for (int i = 0; i < 10; i++) {
		if (i == your_num / 10) {
			sprintf(num_path, "milk_images/%d.png", i);
			milk_your_num[0] = Object::create(num_path, milk_result_scene, 880, 280);

		}
	}
	for (int i = 0; i < 10; i++) {
		if (i == your_num % 10) {
			sprintf(num_path, "milk_images/%d.png", i);
			milk_your_num[1] = Object::create(num_path, milk_result_scene, 920, 280);

		}
	}

	if ((your_num < my_num && issmaller) || (your_num > my_num && !issmaller)) { //������ ��
		milk_text[text_i]->show(); //text_i == 8
		milk_win->play();
	}
	else if (your_num == my_num) { //���ڰ� �Ȱ��� ��
		text_i += 2;
		milk_text[text_i]->show(); //text_i == 10
	}
	else { //Ʋ���� ��
		text_i += 4;
		milk_text[text_i]->show(); //text_i == 12
		milk_fail->play();
		life_scene_2(milk_result_scene);
		life_cut();
	}
}

//���� �����ϴ� �Լ�
void create_milk_num() {
	milk_kids->stop();
	milk_bgm->play();

	my_num = rand() % 100; //�� ���� ����
	your_num = rand() % 100; //ģ�� ���� ����

	//�� ������ ���� �� ���� ����
	char num_path[20];
	for (int i = 0; i < 10; i++) {
		if (i == my_num / 10) {
			sprintf(num_path, "milk_images/%d.png", i);
			milk_my_num[0] = Object::create(num_path, milk_check_scene, 300, 300);
			milk_my_num2[0] = Object::create(num_path, milk_result_scene, 300, 300); //��� Ȯ�� ȭ���
		}
	}
	for (int i = 0; i < 10; i++) {
		if (i == my_num % 10) {
			sprintf(num_path, "milk_images/%d.png", i);
			milk_my_num[1] = Object::create(num_path, milk_check_scene, 340, 300);
			milk_my_num2[1] = Object::create(num_path, milk_result_scene, 340, 300); //��� Ȯ�� ȭ���
		}
	}
}

//���� �ʱ�ȭ
void init_milk_game() {
	//���ʿ��� �⺻�ɼ� ����
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	//���� ����
	classroom = Scene::create("", "milk_images/class.png"); //ùȭ��
	milk_scene = Scene::create("", "milk_images/milk_scene.png"); //���� ���� ȭ�� (å�� ���� �� ��)
	milk_check_scene = Scene::create("", "milk_images/milk_check_scene.png"); //�� ���� Ȯ�� ȭ��
	milk_result_scene = Scene::create("", "milk_images/milk_result_scene.png"); //��� Ȯ�� ȭ��

	//����� ����
	milk_kids = Sound::create("milk_sounds/milk_kids.mp3");
	milk_bgm = Sound::create("milk_sounds/milk_bgm.mp3");
	milk_fail = Sound::create("milk_sounds/milk_fail.mp3");
	milk_win = Sound::create("milk_sounds/milk_win.mp3");

	//�ؽ�Ʈ �̹����� ����
	char path[20];
	for (int i = 0; i < 14; i++) {
		sprintf(path, "milk_images/t%d.png", i);
		if (i < 3) milk_text[i] = Object::create(path, classroom, 0, 0, false); //ùȭ�� �ؽ�Ʈ��
		else if (3 <= i && i <= 5) milk_text[i] = Object::create(path, milk_scene, 0, 0, false); //���� ���� ȭ�� �ؽ�Ʈ��
		else if (6 <= i && i <= 7) milk_text[i] = Object::create(path, milk_check_scene, 0, 0, false); //�� ���� Ȯ�� ȭ�� �ؽ�Ʈ��
		else milk_text[i] = Object::create(path, milk_result_scene, 0, 0, false); //��� Ȯ�� ȭ�� �ؽ�Ʈ��

		milk_text[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
			milk_text[text_i]->hide();

			milk_text[text_i + 1]->show();

			switch (text_i) { //�߰���� �ʿ��� �ؽ�Ʈ�� Ư��ó��
			case 2: //�� ���� �� ������ �ٰ��´�
				milk_scene->enter();
				break;
			case 5: //�Ƹ� 0���� 99���� ������ ����?
				milk_check_scene->enter();
				create_milk_num();
				break;
			case 7: //ģ�� : �� ���ڰ� �� ���ں��� ū�� ������ ������
				milk_text[text_i + 1]->hide(); //�ؽ�Ʈ �����
				bigger->show(); smaller->show(); //ũ�� ��ư ���̱�
				break;
			case 9: //ģ�� : ��! �̰� ������!
				milk_text[text_i + 1]->hide(); //�ؽ�Ʈ �����
				milk_2_lunch_main();
				break;
			case 11: //ģ�� : �� ��� �Ȱ��� �� ����? ���� �ٲ㼭 �ٽ� ����
				milk_text[text_i + 1]->hide(); //�ؽ�Ʈ �����
				text_i = 6; //��� ����, �� ���ڴ�... ����!
				create_milk_num();
			case 13: // ��� �߸�����~
				milk_2_lunch_main();
			}

			text_i++;

			return true;
			});
	}

	//ũ�� ��ư ����
	bigger = Object::create("milk_images/bigger.png", milk_check_scene, 800, 400, false);
	smaller = Object::create("milk_images/smaller.png", milk_check_scene, 950, 400, false);

	bigger->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		issmaller = false; //ũ�� ����
		milk_result(); //��� �����ִ� �Լ�
		return true;
		});

	smaller->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		issmaller = true; //�۴� ����
		milk_result(); //��� �����ִ� �Լ�
		return true;
		});

	//ùȭ�� ����
	milk_box = Object::create("milk_images/milk_box.png", classroom, 440, 250);
	milk_text[0]->show();
	milk_kids->play();
	// startGame(classroom);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	classroom->enter();
}

void milk_num_main()
{
	srand((unsigned int)time(NULL));

	init_milk_game();
}