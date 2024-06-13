#ifndef CharacterSelectScene_HPP
#define CharacterSelectScene_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include <vector>
#include <iostream>

class CharacterSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	int player; // 1 for player1, 2 for player2
	vector<char> playerName_1;
	vector<Engine::IObject*> nowDrawing_1;
	vector<char> playerName_2;
	vector<Engine::IObject*> nowDrawing_2;
	int startIndex_1;
	int startIndex_2;
	int x1;
	int y1;
	int x2;
	int y2;
	Engine::Label* Title;
	Engine::Image* rec;
    explicit CharacterSelectScene() = default;
	void Initialize() override;
	void Terminate() override;
	void PlayOnClick(int stage);
    void ScoreboardOnClick();
	void BackOnClick(int stage);
	void OnKeyDown(int keyCode) override;
	char parseKeyCode(int keyCode,int mode);
	void DrawName();
	void RemoveChar();
	void DrawTitle();
	void SelectedOnClick(int stage, int from);
	void DrawSelected_1(int stage);
	void DrawSelected_2(int stage);
};

#endif // CharacterSelectScene_HPP