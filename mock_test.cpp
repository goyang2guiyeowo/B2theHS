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

//������ Ŭ������ �� ���������� ó���� �ϵ�
void mock_right_result() {
    mock_text[mock_test_i]->show();
    mock_pokemon->stop();
    mock_right->play();
}

//������ Ŭ������ �� ���������� ó���� �ϵ�
void mock_wrong_result() {
    mock_text[++mock_test_i]->show();
    mock_pokemon->stop();
    // mock_wrong->play();
	life_cut();
}

//����° ���� �Լ�
void mock_thirdQ() {
    mockQ3->enter();
	// ������ �߰�
	life_scene_2(mockQ3);
    mock_right->stop();
    mock_wrong->stop();
    mock_pokemon->play();

    char path[50]; //�̹��� ��θ� ������ ����
    int optionX[5] = { 650, 825, 990, 750, 950 }, optionY[5] = { 340, 340, 342, 270, 270 }; //�̹��� X��ǥ�� Y��ǥ�� ������ ����
    for (int i = 0; i < 5; i++) {
        sprintf(path, "mock_images/3-%d.png", i + 1);
        option[i] = Object::create(path, mockQ3, optionX[i], optionY[i]); //������ 5�� ����

        if (i == 3) { //����(4��)�� Ŭ���ϸ�
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_right_result(); //mock_test_i == 15, ������ : ��.. �׷�, �����̴�.
                return true;
                });
        }
        else { //����(1,2,3,5��)�� Ŭ���ϸ�
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_wrong_result(); //++mock_test_i == 16, ������ : ��! ������ ��..
                return true;
                });
        }
    }
}

//�ι�° ���� �Լ�
void mock_secondQ() {
    mockQ2->enter();
	life_scene_2(mockQ2);
    mock_right->stop();
    mock_wrong->stop();
    mock_pokemon->play();

    char path[50]; //�̹��� ��θ� ������ ����
    int optionX[5] = { 650, 830, 1015, 750, 950 }, optionY[5] = { 290, 290, 295, 200, 197 }; //�̹��� X��ǥ�� Y��ǥ�� ������ ����
    for (int i = 0; i < 5; i++) {
        sprintf(path, "mock_images/2-%d.png", i + 1);
        option[i] = Object::create(path, mockQ2, optionX[i], optionY[i]); //������ 5�� ����

        if (i == 3) { //����(4��)�� Ŭ���ϸ�
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_right_result(); //mock_test_i == 11, ������ : ��.. �׷�, �����̴�. 
               return true;
                });
        }
        else { //����(1,2,3,5��)�� Ŭ���ϸ�
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_wrong_result(); //++mock_test_i == 12, ������ : ���̰� �̰� ���� ������..
               return true;
                });
        }
    }
}

//ù��° ���� �Լ�
void mock_firstQ() {
    mockQ1->enter();
	life_scene_2(mockQ1);
    mock_heart->stop();
    mock_pokemon->play();

    char path[50]; //�̹��� ��θ� ������ ����
    int optionX[5] = { 660, 830, 1010, 750, 930 }, optionY[5] = { 300, 300, 298, 200, 200 }; //�̹��� X��ǥ�� Y��ǥ�� ������ ����
    for (int i = 0; i < 5; i++) {
        sprintf(path, "mock_images/1-%d.png", i + 1);
        option[i] = Object::create(path, mockQ1, optionX[i], optionY[i]); //������ 5�� ����
        
        if (i == 1) { //����(2��)�� Ŭ���ϸ�
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_right_result(); // mock_test_i == 7, ������ : ��.. �׷�, �����̴�.
                return true;
                });
        }
        else { //����(1,3,4,5��)�� Ŭ���ϸ�
            option[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
                mock_wrong_result(); //++mock_test_i == 8, ������ : ���̰� 1������ Ʋ����.. 
                return true;
                });
        }
    }
}

//���� �ʱ�ȭ
void init_mock_game() {
    //���ʿ��� �⺻�ɼ� ����
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    //���� ����
    teacher_scene = Scene::create("", "mock_images/teacher_scene.png"); //ùȭ��
    mockQ1 = Scene::create("", "mock_images/mockQ1.png"); //ù��° ���� ȭ��
    mockQ2 = Scene::create("", "mock_images/mockQ2.png"); //�ι�° ���� ȭ��
    mockQ3 = Scene::create("", "mock_images/mockQ3.png"); //����° ���� ȭ��

    //����� ����
    mock_heart = Sound::create("mock_sounds/mock_heartbeat.mp3"); //�αٵα�
    mock_pokemon = Sound::create("mock_sounds/mock_pokemon.mp3"); //���ϸ� ���
    mock_right = Sound::create("mock_sounds/mock_right.mp3"); //��~~
    mock_wrong = Sound::create("mock_sounds/mock_wrong.mp3"); //������

    //�ؽ�Ʈ�� ����
    char path[20];
    for (int i = 0; i < MOCK_TEXT_MAX; i++) {
        sprintf(path, "mock_images/%d.png", i);
        if (i < 7) mock_text[i] = Object::create(path, teacher_scene, 0, 50, false); //ùȭ�� �ؽ�Ʈ��
        else if (7 <= i && i <= 10) mock_text[i] = Object::create(path, mockQ1, 0, 0, false); //ù��° ���� ȭ�� �ؽ�Ʈ��
        else if (11 <= i && i <= 14) mock_text[i] = Object::create(path, mockQ2, 0, 0, false); //�ι�° ���� ȭ�� �ؽ�Ʈ��
        else mock_text[i] = Object::create(path, mockQ3, 0, 0, false); //����° ���� ȭ�� �ؽ�Ʈ��

        mock_text[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
            mock_text[mock_test_i]->hide();
			text_box_next->play();
			if (i != MOCK_TEXT_MAX - 2) mock_text[mock_test_i + 1]->show();

            switch (mock_test_i) { //�߰���� �ʿ��� �ؽ�Ʈ�� Ư��ó��
            case 2 : //¦�� : ��! �� �ҽ��ϴ�!��������
                mock_heart->play(); break;
            case 6 : //��... ���ڱ� �������� ����..!
                mock_text[mock_test_i + 1]->hide(); //�ؽ�Ʈ �����
                mock_firstQ(); break; //ù��° ���� �Լ�
            case 7 : //������ : ��.. �׷�, �����̴�.
                mock_text[mock_test_i + 1]->hide(); //�ؽ�Ʈ �����
                mock_test_i += 2; //mock_test_i == 9
                mock_text[mock_test_i + 1]->show(); break;
            case 10 : //������ : �׷� 2���� Ǯ�����.
                mock_text[mock_test_i + 1]->hide(); //�ؽ�Ʈ �����
                mock_secondQ(); break; //�ι�° ���� �Լ�
            case 11 : //������ : ��.. �׷�, �����̴�.
                mock_text[mock_test_i + 1]->hide(); //�ؽ�Ʈ �����
                mock_test_i += 2; //mock_test_i == 13
                mock_text[mock_test_i + 1]->show(); break;
            case 14 : //������ : �׷� ���������� 3���� Ǯ�����
                mock_text[mock_test_i + 1]->hide(); //�ؽ�Ʈ �����
                mock_thirdQ(); break;
            case 15 : //������ : ��.. �׷�, �����̴�.
                mock_text[mock_test_i + 1]->hide(); //�ؽ�Ʈ ����� 
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

    //ùȭ�� ����
    mock_text[0]->show();

	teacher_scene->enter();
    // startGame(teacher_scene);
}


void mock_test_main()
{
    init_mock_game();
}