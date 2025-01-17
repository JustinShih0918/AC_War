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
#include "Scene/MainPlayScene.hpp"
#include "UI/Animation/Warning.hpp"
#include <iostream>
#include <iostream>
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
    Title = nullptr;
    circle = nullptr;
    intro = nullptr;
    rec = nullptr;
    player = 1;
    player1_select = 0;
    player2_select = 0;
    playerName_1.clear();
    playerName_2.clear();
    display = false;
    for(int i = 0;i<5;i++){
        selectedImg_1[i] = nullptr;
        selectedImg_2[i] = nullptr;
    }
    characterList[0][0] = 1;
    characterList[0][1] = 2;
    characterList[0][2] = 3;
    characterList[0][3] = 4;
    characterList[1][0] = 5;
    characterList[1][1] = 6;
    characterList[1][2] = 1;
    characterList[1][3] = 1;
    characterList[2][0] = 1;
    characterList[2][1] = 1;
    characterList[2][2] = 1;
    characterList[2][3] = 1;
    character_img[0] = "character-select/Selected.png";
    character_img[1] = "character-select/character-circle/archer_circle.png";
    character_img[2] = "character-select/character-circle/cs_circle.png";
    character_img[3] = "character-select/character-circle/bomber_circle.png";
    character_img[4] = "character-select/character-circle/titan_circle.png";
    character_img[5] = "character-select/character-circle/shadow_circle.png";
    character_img[6] = "character-select/character-circle/dragon_circle.png";
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
    circlePos = Engine::Point(0,0);
    AddNewObject(new Engine::Image("character-select/background.png", 0, 0, 0, 0, 0.0, 0.0));
    DrawTitle();

    AddNewObject(UIGroup = new Group());

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
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 500, 400, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::GoOnClick, this));
    AddNewControlObject(btn); 
    AddNewObject(new Engine::Label("Go", "pirulen.ttf", 36, halfW, halfH / 2 + 550, 0, 0, 0, 255, 0.5, 0.5));
    int initX = 420;
    int initY = 89;
    int detX = 185;
    int detY = 185;
    DrawCircle();
    UpdateCircle();
    bgmInstance = AudioHelper::PlaySample("interstellar_theme.ogg", true, AudioHelper::BGMVolume);
}

void CharacterSelectScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void CharacterSelectScene::GoOnClick(){
    if(Check()) Engine::GameEngine::GetInstance().ChangeScene("MainPlay");
}

void CharacterSelectScene::UpdateSelected(int mode){
    if(mode == 1) {
        if(player == 1) selected_1[player1_select] = characterList[(int)circlePos.x][(int)circlePos.y];
        else if(player == 2) selected_2[player2_select] = characterList[(int)circlePos.x][(int)circlePos.y];
    }
    else if(mode == 2){
        if(player == 1) selected_1[player1_select] = 0;
        else if(player == 2) selected_2[player2_select] = 0;
    }
    cout << "player 1:";
    for(int i = 0;i<5;i++) cout << " " << selected_1[i]; 
    cout << "\nplayer 2:";
    for(int i = 0;i<5;i++) cout << " " << selected_2[i];
    cout << "\n";
}

bool CharacterSelectScene::Check(){
    if(playerName_1.empty() || playerName_2.empty()) {
        UIGroup->AddNewObject(new Warning(2, 800, 50, 32));
        return false;
    }

    for(int i = 0;i<5;i++){
        if(selected_1[i] == 0 || selected_2[i] == 0){
            UIGroup->AddNewObject(new Warning(1, 800, 50, 32));
            return false;
        }
    }

    return true;
}

void CharacterSelectScene::DrawCircle(){
    int initX = 420;
    int initY = 89;
    int detX = 185;
    int detY = 185;
    Engine::Image* circle;

    for(int i = 0;i<3;i++){
        for(int j = 0;j<4;j++){
            circle = new Engine::Image(character_img[characterList[i][j]], initX + detX*j + 97, initY + detY*i + 80,126,126,0.5,0.5);
            AddNewObject(circle);
        }
    }
}

void CharacterSelectScene::UpdateCircle(){
    int initX = 420;
    int initY = 89;
    int detX = 185;
    int detY = 185;
    if(circle) RemoveObject(circle->GetObjectIterator());

    if(circlePos.x < 0 ) circlePos.x++;
    else if(circlePos.x > 2) circlePos.x--;

    if(circlePos.y < 0) circlePos.y++;
    else if(circlePos.y > 3) circlePos.y--;

    circle = new Engine::Image("character-select/circle.png",initX + detX * (int)circlePos.y, initY + detY * (int)circlePos.x, 0, 0);
    AddNewObject(circle);
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
    if(keyCode == ALLEGRO_KEY_3) {
        playerName_1.push_back('a');
        playerName_2.push_back('b');

        for(int i = 0;i<5;i++){
            int a = i+1;
            selected_1[i] = a;
            selected_2[i] = a;
        }
        UpdateSelectImg();
    }
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
	else if(keyCode == ALLEGRO_KEY_UP || keyCode == ALLEGRO_KEY_DOWN || keyCode == ALLEGRO_KEY_LEFT || keyCode == ALLEGRO_KEY_RIGHT){
        if(keyCode == ALLEGRO_KEY_UP) circlePos.x--;
        else if(keyCode == ALLEGRO_KEY_DOWN) circlePos.x++;
        else if(keyCode == ALLEGRO_KEY_RIGHT) circlePos.y++;
        else if(keyCode == ALLEGRO_KEY_LEFT) circlePos.y--;
        UpdateCircle();
    }
    else if(keyCode == ALLEGRO_KEY_ENTER || keyCode == ALLEGRO_KEY_TAB || keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT && (player1_select || player2_select)){
        if(keyCode == ALLEGRO_KEY_ENTER) UpdateSelected(1);
        else if(keyCode == ALLEGRO_KEY_TAB) UpdateSelected(2);
        else if(keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT){
            if(!display) DrawIntroduction(characterList[(int)circlePos.x][(int)circlePos.y]);
            else{
                RemoveObject(intro->GetObjectIterator());
                display = false;
            }
        }
        UpdateSelectImg();
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
        player1_select = stage;
        DrawSelected_1(stage);
    }
    else if(player == 2 && from == 2){
        player2_select = stage;
        DrawSelected_2(stage);
    }
}

void CharacterSelectScene::UpdateSelectImg(){
    for(int i = 0;i<5;i++) if(selectedImg_1[i]) RemoveObject(selectedImg_1[i]->GetObjectIterator());
    for(int i = 0;i<5;i++) if(selectedImg_2[i]) RemoveObject(selectedImg_2[i]->GetObjectIterator());

    for(int i = 0;i<5;i++){
        selectedImg_1[i] = new Engine::Image(character_img[selected_1[i]], posX_1[i] - detX/2 - 1, posY_1[i] - detY/2 + 10, 187, 167);
        AddNewObject(selectedImg_1[i]);
    }

    for(int i = 0;i<5;i++){
        selectedImg_2[i] = new Engine::Image(character_img[selected_2[i]], posX_2[i] - detX/2 - 1, posY_2[i] - detY/2 + 10, 187, 167);
        AddNewObject(selectedImg_2[i]);
    }
}
void CharacterSelectScene::DrawIntroduction(int index){
    // if(intro) RemoveObject(intro->GetObjectIterator());

    intro = new Engine::Image("introduce/" + to_string(index) + ".png", 800, 420, 800, 800, 0.5, 0.5);
    AddNewObject(intro);
    display = true;
}