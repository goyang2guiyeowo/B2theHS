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

//결과 보여주는 함수
void milk_result() {
	bigger->hide(); smaller->hide(); //버튼들 숨기기
	milk_result_scene->enter(); //결과 확인 화면으로 넘어가기
	milk_bgm->stop();

	//니 우유곽 위에 니 숫자 생성
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

	if ((your_num < my_num && issmaller) || (your_num > my_num && !issmaller)) { //맞혔을 때
		milk_text[text_i]->show(); //text_i == 8
		milk_win->play();
	}
	else if (your_num == my_num) { //숫자가 똑같을 때
		text_i += 2;
		milk_text[text_i]->show(); //text_i == 10
	}
	else { //틀렸을 때
		text_i += 4;
		milk_text[text_i]->show(); //text_i == 12
		milk_fail->play();
		life_scene_2(milk_result_scene);
		life_cut();
	}
}

//숫자 생성하는 함수
void create_milk_num() {
	milk_kids->stop();
	milk_bgm->play();

	my_num = rand() % 100; //내 숫자 생성
	your_num = rand() % 100; //친구 숫자 생성

	//내 우유곽 위에 내 숫자 생성
	char num_path[20];
	for (int i = 0; i < 10; i++) {
		if (i == my_num / 10) {
			sprintf(num_path, "milk_images/%d.png", i);
			milk_my_num[0] = Object::create(num_path, milk_check_scene, 300, 300);
			milk_my_num2[0] = Object::create(num_path, milk_result_scene, 300, 300); //결과 확인 화면용
		}
	}
	for (int i = 0; i < 10; i++) {
		if (i == my_num % 10) {
			sprintf(num_path, "milk_images/%d.png", i);
			milk_my_num[1] = Object::create(num_path, milk_check_scene, 340, 300);
			milk_my_num2[1] = Object::create(num_path, milk_result_scene, 340, 300); //결과 확인 화면용
		}
	}
}

//게임 초기화
void init_milk_game() {
	//불필요한 기본옵션 제거
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	//배경들 생성
	classroom = Scene::create("", "milk_images/class.png"); //첫화면
	milk_scene = Scene::create("", "milk_images/milk_scene.png"); //내기 시작 화면 (책상에 우유 두 개)
	milk_check_scene = Scene::create("", "milk_images/milk_check_scene.png"); //내 숫자 확인 화면
	milk_result_scene = Scene::create("", "milk_images/milk_result_scene.png"); //결과 확인 화면

	//사운드들 생성
	milk_kids = Sound::create("milk_sounds/milk_kids.mp3");
	milk_bgm = Sound::create("milk_sounds/milk_bgm.mp3");
	milk_fail = Sound::create("milk_sounds/milk_fail.mp3");
	milk_win = Sound::create("milk_sounds/milk_win.mp3");

	//텍스트 이미지들 생성
	char path[20];
	for (int i = 0; i < 14; i++) {
		sprintf(path, "milk_images/t%d.png", i);
		if (i < 3) milk_text[i] = Object::create(path, classroom, 0, 0, false); //첫화면 텍스트들
		else if (3 <= i && i <= 5) milk_text[i] = Object::create(path, milk_scene, 0, 0, false); //내기 시작 화면 텍스트들
		else if (6 <= i && i <= 7) milk_text[i] = Object::create(path, milk_check_scene, 0, 0, false); //내 숫자 확인 화면 텍스트들
		else milk_text[i] = Object::create(path, milk_result_scene, 0, 0, false); //결과 확인 화면 텍스트들

		milk_text[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
			milk_text[text_i]->hide();

			milk_text[text_i + 1]->show();

			switch (text_i) { //추가기능 필요한 텍스트들 특별처리
			case 2: //앗 누가 내 쪽으로 다가온다
				milk_scene->enter();
				break;
			case 5: //아마 0부터 99까지 나오곤 했지?
				milk_check_scene->enter();
				create_milk_num();
				break;
			case 7: //친구 : 내 숫자가 네 숫자보다 큰지 작은지 맞히면
				milk_text[text_i + 1]->hide(); //텍스트 숨기기
				bigger->show(); smaller->show(); //크작 버튼 보이기
				break;
			case 9: //친구 : 아! 이걸 맞히네!
				milk_text[text_i + 1]->hide(); //텍스트 숨기기
				milk_2_lunch_main();
				break;
			case 11: //친구 : 헐 어떻게 똑같을 수 있지? 우유 바꿔서 다시 ㄱㄱ
				milk_text[text_i + 1]->hide(); //텍스트 숨기기
				text_i = 6; //어디 보자, 내 숫자는... 아하!
				create_milk_num();
			case 13: // 돈까스 잘먹을게~
				milk_2_lunch_main();
			}

			text_i++;

			return true;
			});
	}

	//크작 버튼 생성
	bigger = Object::create("milk_images/bigger.png", milk_check_scene, 800, 400, false);
	smaller = Object::create("milk_images/smaller.png", milk_check_scene, 950, 400, false);

	bigger->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		issmaller = false; //크다 저장
		milk_result(); //결과 보여주는 함수
		return true;
		});

	smaller->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		issmaller = true; //작다 저장
		milk_result(); //결과 보여주는 함수
		return true;
		});

	//첫화면 세팅
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