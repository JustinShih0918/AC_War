#ifndef CharacterSelectScene_HPP
#define CharacterSelectScene_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include <vector>
#include <iostream>
using namespace std;
class CharacterSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	int player; // 1 for player1, 2 for player2
	int player1_select;
	int player2_select;
	vector<char> playerName_1;
	vector<Engine::IObject*> nowDrawing_1;
	vector<char> playerName_2;
	vector<Engine::IObject*> nowDrawing_2;
	int characterList[3][4];
	int startIndex_1;
	int startIndex_2;
	int x1;
	int y1;
	int x2;
	int y2;
	int selected_1[5];
	int selected_2[5];
	Engine::Image* intro;
	bool display;
	Engine::Image* selectedImg_1[5];
	Engine::Image* selectedImg_2[5];
	string character_img[7];
	Engine::Label* Title;
	Engine::Image* rec;
	Engine::Point circlePos;
	Engine::Image* circle;
	Engine::Group* UIGroup;
    explicit CharacterSelectScene() = default;
	void Initialize() override;
	void Terminate() override;
	void OnKeyDown(int keyCode) override;
	char parseKeyCode(int keyCode,int mode);
	void DrawName();
	void RemoveChar();
	void DrawTitle();
	void SelectedOnClick(int stage, int from);
	void GoOnClick();
	void DrawSelected_1(int stage);
	void DrawSelected_2(int stage);
	void UpdateCircle();
	void UpdateSelected(int mode); // 1 for add, 2 for delete
	bool Check();
	void DrawCircle();
	void UpdateSelectImg();
	void DrawIntroduction(int index);
};

#endif // CharacterSelectScene_HPP