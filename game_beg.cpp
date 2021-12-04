#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>	
using namespace bangtal;

extern void dress_room_main();
extern void life_init();

ScenePtr beg_cau_scene, beg_tang_scene, beg_oops_scene, beg_drown_scene, beg_water_scene, beg_dress_room_scene;
ObjectPtr beg_text[29], beg_paper_hold, beg_paper_gone, beg_arm;
SoundPtr beg_wind, beg_splash, beg_water_bubble, beg_knock;
TimerPtr beg_animationTimer, beg_animationTimer2;

// ���� ��ü ����������Ʈ ���� ����, ������ �Ҹ�
extern ObjectPtr life_point_bar;
extern ObjectPtr life_point[4];

void life_init()
{
    life_point_bar = Object::create("Images/life_bar.png", beg_dress_room_scene, 30, 620);

    for (int i = 0; i < 4; i++)
    {
        life_point[i] = Object::create("Images/life_point_0.png", beg_dress_room_scene, 30 + (10 * (i + 1) + 50 * (i)), 630);
    }
}

int beg_text_i = 0;
float beg_scene_light = 1.0f; //���̵�ƿ��� ���� scene ��� ����
float beg_animationTime = 0.01f; //���̵�ƿ��� ���� �ð����� ����

//���̵�ƿ���Ű�� �Լ�
void beg_fade_out()
{
    beg_animationTimer = Timer::create(beg_animationTime);
    beg_animationTimer->setOnTimerCallback([&](auto)->bool
        {
			if (beg_scene_light >= 0)
			{
				beg_scene_light -= 0.002f;
				beg_water_scene->setLight(beg_scene_light);
				beg_animationTimer->set(beg_animationTime);
				beg_animationTimer->start();
			}
			else
			{
				beg_water_bubble->stop();
                beg_dress_room_scene->enter();
                beg_text[beg_text_i]->show(); //19 >> ����, ���Ⱑ �����..?
                life_init();
			}
            
            return 0;
        });
    beg_animationTimer->start();
}

void beg_main() {
    //���ʿ��� �⺻�ɼ� ����
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    //���� ����
    beg_cau_scene = Scene::create("", "beginning_images/cau_scene.jpg"); //ùȭ��
    beg_tang_scene = Scene::create("", "beginning_images/tang_scene.png"); //û���� ȭ��
    beg_oops_scene = Scene::create("", "beginning_images/oops_scene.png"); //û������ ����Ʈ ȭ��
    beg_drown_scene = Scene::create("", "beginning_images/drown_scene.png"); //� ȭ��
    beg_water_scene = Scene::create("", "beginning_images/water_scene.png"); //�� �� ȭ��
    beg_dress_room_scene = Scene::create("", "beginning_images/beg_dress_room_scene.png"); //�� �� ȭ��

    //��� �̹������� �ؽ�Ʈ���� ���� ����
    beg_arm = Object::create("beginning_images/arm.png", beg_oops_scene, 700, -300, false); //��
    beg_paper_hold = Object::create("beginning_images/paper_hold.png", beg_tang_scene, 700, 100, false); //������ ����Ʈ

    //����� ����
    beg_wind = Sound::create("beginning_sounds/wind.mp3"); //������
    beg_splash = Sound::create("beginning_sounds/splash.mp3"); //ǳ��
    beg_water_bubble = Sound::create("beginning_sounds/water_bubble.mp3"); //���ۺ���
    beg_knock = Sound::create("beginning_sounds/knock.mp3"); //������

    //�ؽ�Ʈ �̹����� ����
    char path[30];
    for (int i = 0; i < 29; i++) {
        sprintf(path, "beginning_images/%d.png", i);
        if (i < 3) beg_text[i] = Object::create(path, beg_cau_scene, 0, 0, false);
        else if (3 <= i && i <= 5) beg_text[i] = Object::create(path, beg_tang_scene, 0, 0, false);
        else if (6 <= i && i <= 7) beg_text[i] = Object::create(path, beg_oops_scene, 0, 0, false);
        else if (8 <= i && i <= 12) beg_text[i] = Object::create(path, beg_drown_scene, 0, 0, false);
        else if (13 <= i && i <= 18) beg_text[i] = Object::create(path, beg_water_scene, 0, 0, false);
        else beg_text[i] = Object::create(path, beg_dress_room_scene, 0, 0, false);

        beg_text[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
            beg_text[beg_text_i]->hide();
            beg_text[beg_text_i + 1]->show();

            switch (beg_text_i) { //�߰���� �ʿ��� �ؽ�Ʈ�� Ư��ó��
            case 2 : //��� û�������� ��å �� �ұ�
                beg_tang_scene->enter();
                beg_paper_hold->show(); break; //������ ����Ʈ
            case 4 : //��.. �ٽ� ���ư���...!
                beg_paper_hold->hide();
                beg_paper_gone = Object::create("beginning_images/paper_gone.png", beg_tang_scene, 200, 300); //���ư��� ������ ����Ʈ
                beg_paper_gone->setScale(0.7);
                beg_wind->play(); break;
            case 5 : //��, �� ������ ����Ʈ��...!
                beg_oops_scene->enter(); break;
            case 6 : //�ٶ��� ���� û������ �������ȴ�!
                beg_arm->show(); break;
            case 7 : //�ȵ�-!
                beg_drown_scene->enter();
                beg_splash->play(); break;
            case 9 : //�� ���� �� ����..?
                beg_water_bubble->play(); break;
            case 12 : //����..? ���� ���ϴ� ����..?
                beg_water_scene->enter(); break;
            case 17 : //Ǫ������ Ǫ���� Ǫ��!
                beg_water_bubble->play(); break;
            case 18 : //���� ������..
                beg_text[beg_text_i + 1]->hide();
                beg_fade_out(); break;
            case 21 : //����.. ���ŷ� ���ƿ°ǰ�??
                beg_knock->play(); break;
            case 26 : //4���� ��ȸ..?
                beg_knock->play(); break;
            case 28 : //�ϴ� ���� �б��� ���߰ڴ�.
                dress_room_main();
            }

            beg_text_i++;
            
            return true;
            });
    }

    //ùȭ�� ����
    beg_text[0]->show();
    
	// startGame(beg_cau_scene)
	beg_cau_scene->enter();
}