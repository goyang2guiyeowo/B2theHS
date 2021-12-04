#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>	
using namespace bangtal;

extern void dress_room_main();
extern void life_init();

ScenePtr beg_cau_scene, beg_tang_scene, beg_oops_scene, beg_drown_scene, beg_water_scene, beg_dress_room_scene;
ObjectPtr beg_text[29], beg_paper_hold, beg_paper_gone, beg_arm;
SoundPtr beg_wind, beg_splash, beg_water_bubble, beg_knock;
TimerPtr beg_animationTimer, beg_animationTimer2;

// 게임 전체 라이프포인트 배경과 개수, 깨지는 소리
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
float beg_scene_light = 1.0f; //페이드아웃을 위한 scene 밝기 변수
float beg_animationTime = 0.01f; //페이드아웃을 위한 시간간격 변수

//페이드아웃시키는 함수
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
                beg_text[beg_text_i]->show(); //19 >> 으음, 여기가 어디지..?
                life_init();
			}
            
            return 0;
        });
    beg_animationTimer->start();
}

void beg_main() {
    //불필요한 기본옵션 제거
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    //배경들 생성
    beg_cau_scene = Scene::create("", "beginning_images/cau_scene.jpg"); //첫화면
    beg_tang_scene = Scene::create("", "beginning_images/tang_scene.png"); //청룡탕 화면
    beg_oops_scene = Scene::create("", "beginning_images/oops_scene.png"); //청룡탕에 프린트 화면
    beg_drown_scene = Scene::create("", "beginning_images/drown_scene.png"); //까만 화면
    beg_water_scene = Scene::create("", "beginning_images/water_scene.png"); //물 속 화면
    beg_dress_room_scene = Scene::create("", "beginning_images/beg_dress_room_scene.png"); //물 속 화면

    //몇몇 이미지들을 텍스트보다 먼저 생성
    beg_arm = Object::create("beginning_images/arm.png", beg_oops_scene, 700, -300, false); //팔
    beg_paper_hold = Object::create("beginning_images/paper_hold.png", beg_tang_scene, 700, 100, false); //객지프 프린트

    //사운드들 생성
    beg_wind = Sound::create("beginning_sounds/wind.mp3"); //휘이잉
    beg_splash = Sound::create("beginning_sounds/splash.mp3"); //풍덩
    beg_water_bubble = Sound::create("beginning_sounds/water_bubble.mp3"); //보글보글
    beg_knock = Sound::create("beginning_sounds/knock.mp3"); //쿵쿵쿵

    //텍스트 이미지들 생성
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

            switch (beg_text_i) { //추가기능 필요한 텍스트들 특별처리
            case 2 : //잠시 청룡탕에서 산책 좀 할까
                beg_tang_scene->enter();
                beg_paper_hold->show(); break; //객지프 프린트
            case 4 : //나.. 다시 돌아갈래...!
                beg_paper_hold->hide();
                beg_paper_gone = Object::create("beginning_images/paper_gone.png", beg_tang_scene, 200, 300); //날아가는 객지프 프린트
                beg_paper_gone->setScale(0.7);
                beg_wind->play(); break;
            case 5 : //앗, 내 객지프 프린트가...!
                beg_oops_scene->enter(); break;
            case 6 : //바람에 날라가 청룡탕에 빠져버렸다!
                beg_arm->show(); break;
            case 7 : //안돼-!
                beg_drown_scene->enter();
                beg_splash->play(); break;
            case 9 : //왜 발이 안 닿지..?
                beg_water_bubble->play(); break;
            case 12 : //뭐라구..? 누가 말하는 거지..?
                beg_water_scene->enter(); break;
            case 17 : //푸하하항 푸앙항 푸앙!
                beg_water_bubble->play(); break;
            case 18 : //숨이 막힌다..
                beg_text[beg_text_i + 1]->hide();
                beg_fade_out(); break;
            case 21 : //정말.. 과거로 돌아온건가??
                beg_knock->play(); break;
            case 26 : //4번의 기회..?
                beg_knock->play(); break;
            case 28 : //일단 빨리 학교에 가야겠다.
                dress_room_main();
            }

            beg_text_i++;
            
            return true;
            });
    }

    //첫화면 세팅
    beg_text[0]->show();
    
	// startGame(beg_cau_scene)
	beg_cau_scene->enter();
}