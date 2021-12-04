#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
using namespace bangtal;
#define S2M_TEXT_MAX 4

ScenePtr s2m_class_scene;
ObjectPtr s2m_text[S2M_TEXT_MAX];
int s2m_text_i = 0;

extern void milk_num_main();

void s2m_main() {
	//불필요한 기본옵션 제거
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	//배경 생성
	s2m_class_scene = Scene::create("", "school_to_milk_images/class_scene.png");

	//텍스트 이미지들 생성
	char path[30];
	for (int i = 0; i < S2M_TEXT_MAX ; i++) {
		sprintf(path, "school_to_milk_images/%d.png", i);
		s2m_text[i] = Object::create(path, s2m_class_scene, 0, 0, false);

		s2m_text[i]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
			s2m_text[s2m_text_i]->hide();
			s2m_text[s2m_text_i + 1]->show();
			s2m_text_i++;

			return true;
			});
	}

	s2m_text[S2M_TEXT_MAX - 1]->setOnMouseCallback([&](auto p, auto x, auto y, auto action)->bool {
		s2m_text[S2M_TEXT_MAX - 1]->hide();
		milk_num_main();
		return true;
		});

	//첫화면 세팅
	s2m_text[0]->show();

	s2m_class_scene->enter();
	// startGame(s2m_class_scene);
}