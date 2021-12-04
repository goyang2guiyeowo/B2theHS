#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <bangtal>
using namespace bangtal;
#define MOCK_TEXT_MAX 18

ScenePtr teacher_scene, mockQ1, mockQ2, mockQ3;
ObjectPtr option[5], mock_text[MOCK_TEXT_MAX];
SoundPtr mock_heart, mock_pokemon, mock_right, mock_wrong;
int mock_test_i = 0;

extern ObjectPtr life_point_bar;
extern ObjectPtr life_point[4];
extern void life_scene_2(ScenePtr scene);
extern void life_cut();
extern void game_clear_ending_main();
extern SoundPtr text_box_next;

//정답을 클릭했을 때 공통적으로 처리할 일들
void mock_right_result() {
    mock_text[mock_test_i]->show();
    mock_pokemon->stop();
    mock_right->play();
}

//오답을 클릭했을 때 공통적으로 처리할 일들
void mock_wrong_result() {
    mock_text[++mock_test_i]->show();
    mock_pokemon->stop();
    // mock_wrong->play();
	life_cut();
}

//세번째 문제 함수
void mock_thirdQ() {
    mockQ3->enter();
	// 라이프 추가
	life_scene_2(mockQ3);
    mock_right->stop();
    mock_wrong->stop();
    mock_pokemon->play();

    char path[50]; //이미지 경로를 저장할 변수
    int optionX[5] = { 650, 825, 990, 750, 950 }, optionY[5] = { 340, 340, 342, 270, 270 }; //이미지 X좌표와 Y좌표를 저장할 변수
    for (int i = 0; i < 5; i++) {
        sprintf(path, "mock_images/3-%d.png", i + 1);
        option[i] = Object::create(path, mockQ3, optionX[i], optionY[i]); //선택지 5개 생성

        if (i == 3) { //정답(4번)을 클릭하면
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_right_result(); //mock_test_i == 15, 선생님 : 음.. 그래, 정답이다.
                return true;
                });
        }
        else { //오답(1,2,3,5번)을 클릭하면
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_wrong_result(); //++mock_test_i == 16, 선생님 : 땡! 유종의 미..
                return true;
                });
        }
    }
}

//두번째 문제 함수
void mock_secondQ() {
    mockQ2->enter();
	life_scene_2(mockQ2);
    mock_right->stop();
    mock_wrong->stop();
    mock_pokemon->play();

    char path[50]; //이미지 경로를 저장할 변수
    int optionX[5] = { 650, 830, 1015, 750, 950 }, optionY[5] = { 290, 290, 295, 200, 197 }; //이미지 X좌표와 Y좌표를 저장할 변수
    for (int i = 0; i < 5; i++) {
        sprintf(path, "mock_images/2-%d.png", i + 1);
        option[i] = Object::create(path, mockQ2, optionX[i], optionY[i]); //선택지 5개 생성

        if (i == 3) { //정답(4번)을 클릭하면
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_right_result(); //mock_test_i == 11, 선생님 : 음.. 그래, 정답이다. 
               return true;
                });
        }
        else { //오답(1,2,3,5번)을 클릭하면
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_wrong_result(); //++mock_test_i == 12, 선생님 : 아이고 이건 쉬운 문젠데..
               return true;
                });
        }
    }
}

//첫번째 문제 함수
void mock_firstQ() {
    mockQ1->enter();
	life_scene_2(mockQ1);
    mock_heart->stop();
    mock_pokemon->play();

    char path[50]; //이미지 경로를 저장할 변수
    int optionX[5] = { 660, 830, 1010, 750, 930 }, optionY[5] = { 300, 300, 298, 200, 200 }; //이미지 X좌표와 Y좌표를 저장할 변수
    for (int i = 0; i < 5; i++) {
        sprintf(path, "mock_images/1-%d.png", i + 1);
        option[i] = Object::create(path, mockQ1, optionX[i], optionY[i]); //선택지 5개 생성
        
        if (i == 1) { //정답(2번)을 클릭하면
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_right_result(); // mock_test_i == 7, 선생님 : 음.. 그래, 정답이다.
                return true;
                });
        }
        else { //오답(1,3,4,5번)을 클릭하면
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_wrong_result(); //++mock_test_i == 8, 선생님 : 아이고 1번부터 틀리네.. 
                return true;
                });
        }
    }
}

//게임 초기화
void init_mock_game() {
    //불필요한 기본옵션 제거
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    //배경들 생성
    teacher_scene = Scene::create("", "mock_images/teacher_scene.png"); //첫화면
    mockQ1 = Scene::create("", "mock_images/mockQ1.png"); //첫번째 문제 화면
    mockQ2 = Scene::create("", "mock_images/mockQ2.png"); //두번째 문제 화면
    mockQ3 = Scene::create("", "mock_images/mockQ3.png"); //세번째 문제 화면

    //사운드들 생성
    mock_heart = Sound::create("mock_sounds/mock_heartbeat.mp3"); //두근두근
    mock_pokemon = Sound::create("mock_sounds/mock_pokemon.mp3"); //포켓몬 브금
    mock_right = Sound::create("mock_sounds/mock_right.mp3"); //오~~
    mock_wrong = Sound::create("mock_sounds/mock_wrong.mp3"); //하하하

    //텍스트들 생성
    char path[20];
    for (int i = 0; i < MOCK_TEXT_MAX; i++) {
        sprintf(path, "mock_images/%d.png", i);
        if (i < 7) mock_text[i] = Object::create(path, teacher_scene, 0, 50, false); //첫화면 텍스트들
        else if (7 <= i && i <= 10) mock_text[i] = Object::create(path, mockQ1, 0, 0, false); //첫번째 문제 화면 텍스트들
        else if (11 <= i && i <= 14) mock_text[i] = Object::create(path, mockQ2, 0, 0, false); //두번째 문제 화면 텍스트들
        else mock_text[i] = Object::create(path, mockQ3, 0, 0, false); //세번째 문제 화면 텍스트들

        mock_text[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
            mock_text[mock_test_i]->hide();
			text_box_next->play();
			if (i != MOCK_TEXT_MAX - 2) mock_text[mock_test_i + 1]->show();

            switch (mock_test_i) { //추가기능 필요한 텍스트들 특별처리
            case 2 : //짝꿍 : 야! 너 불쌍하다!ㅋㅋㅋㅋ
                mock_heart->play(); break;
            case 6 : //헉... 갑자기 현기증이 난다..!
                mock_text[mock_test_i + 1]->hide(); //텍스트 숨기기
                mock_firstQ(); break; //첫번째 문제 함수
            case 7 : //선생님 : 음.. 그래, 정답이다.
                mock_text[mock_test_i + 1]->hide(); //텍스트 숨기기
                mock_test_i += 2; //mock_test_i == 9
                mock_text[mock_test_i + 1]->show(); break;
            case 10 : //선생님 : 그럼 2번도 풀어봐라.
                mock_text[mock_test_i + 1]->hide(); //텍스트 숨기기
                mock_secondQ(); break; //두번째 문제 함수
            case 11 : //선생님 : 음.. 그래, 정답이다.
                mock_text[mock_test_i + 1]->hide(); //텍스트 숨기기
                mock_test_i += 2; //mock_test_i == 13
                mock_text[mock_test_i + 1]->show(); break;
            case 14 : //선생님 : 그럼 마지막으로 3번도 풀어봐라
                mock_text[mock_test_i + 1]->hide(); //텍스트 숨기기
                mock_thirdQ(); break;
            case 15 : //선생님 : 음.. 그래, 정답이다.
                mock_text[mock_test_i + 1]->hide(); //텍스트 숨기기 
				game_clear_ending_main();
				break;
			case 17 : 
                mock_right->stop();
                mock_wrong->stop();
				game_clear_ending_main();
				break;
            }

            mock_test_i++;

            return true;
            });
    }

    //첫화면 세팅
    mock_text[0]->show();

	teacher_scene->enter();
    // startGame(teacher_scene);
}


void mock_test_main()
{
    init_mock_game();
}