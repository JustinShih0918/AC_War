#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "CharacterSelectScene.hpp"
#include "MainPlayScene.cpp"
using namespace std;

const float detX = 192;
const float detY = 184.5;
const float initX_1 = 112;
const float initY_1 = 304.5;
const float posX_1[5] = {initX_1, initX_1 + detX, initX_1, initX_1 + detX, initX_1};
const float posY_1[5] = {initY_1, initY_1, initY_1 + detY, initY_1 + detY, initY_1 + detY*2};

const float initX_2 = 1312;
const float initY_2 = 304.5;
const float posX_2[5] = {initX_2, initX_2 + detX, initX_2, initX_2 + detX, initX_2 + detX};
const float posY_2[5] = {initY_2, initY_2, initY_2 + detY, initY_2 + detY, initY_2 + detY*2};

void CharacterSelectScene::Initialize() {
    player = 1;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    int W1_3 = w / 3;
    int H1_3 = h / 3;
    int W2_3 = w / 3 * 2;
    x1 = 26;
    y1 = 123;
    x2 = 1228;
    y2 = 123;
    AddNewObject(new Engine::Image("character-select/background.png", 0, 0, 0, 0, 0.0, 0.0));
    DrawTitle();

    Engine::Label *lab;
    lab = new Engine::Label("Player1","OpenSans-Regular.ttf", 90, 38 + 150, 27 + 30, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(lab);
    lab = new Engine::Label("Player2","OpenSans-Regular.ttf", 90, 1239 + 150, 27 + 30, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(lab);
    DrawTitle();

    Engine::ImageButton* btn;
    for(int i = 0;i<5;i++){
        btn = new Engine::ImageButton("character-select/Selected.png", "character-select/SelectRec.png", posX_1[i] - detX/2 - 1, posY_1[i] - detY/2 + 10, 187, 167);
        btn->SetOnClickCallback(std::bind(&CharacterSelectScene::SelectedOnClick, this, i, 1));
        AddNewControlObject(btn); 
    }

    for(int i = 0;i<5;i++){
        btn = new Engine::ImageButton("character-select/Selected.png", "character-select/SelectRec.png", posX_2[i] - detX/2 - 1, posY_2[i] - detY/2 + 10, 187, 167);
        btn->SetOnClickCallback(std::bind(&CharacterSelectScene::SelectedOnClick, this, i, 2));
        AddNewControlObject(btn); 
    }

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW, halfH, 400, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::GoOnClick, this));
    AddNewControlObject(btn); 
    AddNewObject(new Engine::Label("Go", "pirulen.ttf", 36, halfW + 500, halfH / 2 + 300, 0, 0, 0, 255, 0.5, 0.5));

}

void CharacterSelectScene::Terminate() {
	IScene::Terminate();
}

void CharacterSelectScene::GoOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("MainPlay");
}

void CharacterSelectScene::DrawTitle(){
    if(Title) RemoveObject(Title->GetObjectIterator());
    if(player == 1) Title = new Engine::Label("Player1","OpenSans-Regular.ttf", 90, 38 + 150, 27 + 30, 255, 222, 89, 255, 0.5, 0.5);
    else if(player == 2) Title = new Engine::Label("Player2","OpenSans-Regular.ttf", 90, 1239 + 150, 27 + 30, 255, 222, 89, 255, 0.5, 0.5);
    AddNewObject(Title);
}

void CharacterSelectScene::OnKeyDown(int keyCode){
    IScene::OnKeyDown(keyCode);
	char del;
	if(keyCode == ALLEGRO_KEY_BACKSPACE){
        if(player == 1){
            if(playerName_1.empty()) return;
            del = playerName_1.back();
            playerName_1.pop_back();
            startIndex_1--;
            RemoveChar();
        }
        else if(player == 2){
            if(playerName_2.empty()) return;
            del = playerName_2.back();
            playerName_2.pop_back();
            startIndex_2--;
            RemoveChar();
        }
	}
	else if(keyCode >= 1 && keyCode <= 26){
		if(player == 1 && playerName_1.size() <= 8){
            if(playerName_1.empty()){
                playerName_1.push_back(parseKeyCode(keyCode,1));
                startIndex_1 = 0;
            }
            else{
                playerName_1.push_back(parseKeyCode(keyCode,0));
                startIndex_1++;
            }
            DrawName();
        }
        else if(player == 2 && playerName_2.size() <= 8){
            if(playerName_2.empty()){
                playerName_2.push_back(parseKeyCode(keyCode,1));
                startIndex_2 = 0;
            }
            else{
                playerName_2.push_back(parseKeyCode(keyCode,0));
                startIndex_2++;
            }
            DrawName();
        }
	}
    else if(keyCode == ALLEGRO_KEY_1){
        player = 1;
        DrawTitle();
        SelectedOnClick(0, 1);
    }
    else if(keyCode == ALLEGRO_KEY_2){
        player = 2;
        DrawTitle();
        SelectedOnClick(0, 2);
    }
}

void CharacterSelectScene::RemoveChar(){
    if(player == 1){
        if(nowDrawing_1.empty()) return;
	    RemoveObject(nowDrawing_1.back()->GetObjectIterator());
	    nowDrawing_1.pop_back();
    }
    else if(player == 2){
        if(nowDrawing_2.empty()) return;
	    RemoveObject(nowDrawing_2.back()->GetObjectIterator());
	    nowDrawing_2.pop_back();
    }
}

void CharacterSelectScene::DrawName(){
    int x,y,startIndex;
    vector<char> playerName;
    if(player == 1){
        x = x1;
        y = y1;
        startIndex = startIndex_1;
        playerName = playerName_1;
    }
    else if(player == 2){
        x = x2;
        y = y2;
        startIndex = startIndex_2;
        playerName = playerName_2;
    }
	Engine::Label *draw = new Engine::Label(string(1,playerName.back()),"OpenSans-Regular.ttf", 36, x + startIndex*20 + 50, y + 30, 53, 51, 205, 255 , 0.5, 0.5);
	AddNewObject(draw);
	if(player == 1) nowDrawing_1.push_back(draw);
    else if(player == 2) nowDrawing_2.push_back(draw);
}

char CharacterSelectScene::parseKeyCode(int keyCode,int mode){
	if(mode == 0){
		if(keyCode <= 26 && keyCode >= 1) return keyCode + 96;
	}
	else if(mode == 1){
		if(keyCode <= 26 && keyCode >= 1) return keyCode + 64; // upper
	}
	else return 0;

    return 0;
}

void CharacterSelectScene::DrawSelected_1(int stage){
    int x = posX_1[stage];
    int y = posY_1[stage];

    if(rec) RemoveObject(rec->GetObjectIterator());

    rec = new Engine::Image("character-select/SelectRec.png", x, y, 217, 195, 0.5, 0.5);
    AddNewObject(rec);
}

void CharacterSelectScene::DrawSelected_2(int stage){
    int x = posX_2[stage];
    int y = posY_2[stage];

    if(rec) RemoveObject(rec->GetObjectIterator());
    
    rec = new Engine::Image("character-select/SelectRec.png", x, y, 217, 195, 0.5, 0.5);
    AddNewObject(rec);
}

void CharacterSelectScene::SelectedOnClick(int stage, int from){
    if(player == 1 && from == 1){
        DrawSelected_1(stage);
    }
    else if(player == 2 && from == 2){
        DrawSelected_2(stage);
    }
}