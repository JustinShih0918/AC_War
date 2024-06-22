#include <cmath>
#include <string>

#include "Warning.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "Scene/CharacterSelectScene.hpp"
#include "Scene/MainPlayScene.hpp"
#include "UI/Component/Label.hpp"
#include <iostream>
using namespace std;

MainPlayScene* Warning::getMainPlayScene(){
    return dynamic_cast<MainPlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
CharacterSelectScene* Warning::getCharacterSelectScene(){
    return dynamic_cast<CharacterSelectScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Warning::Warning(int m, int x, int y, int size) : Engine::Label("", "OpenSans-Regular.ttf",size,x,y,255,0,0,255, 0.5, 0.5), timeTicks(0), mode(m), posX(x), posY(y) {
    switch (mode)
    {
    case 1:
        warning = "You must fill in all the spaces to play the game";
        break;
    case 2:
        warning = "You must fill in your Name";
        break;
    case 3:
        warning = "You don't have enough Gold";
        break;
    case 4:
        warning = "You can't place your Character here";
        break;
    default:
        warning = "";
        break;
    }
    this->Text = warning;
}

void Warning::Update(float deltaTime){
    timeTicks += deltaTime;
    if(timeTicks >= timeSpan){
        // if(label){
        //     if(mode == 1 || mode == 2) getCharacterSelectScene()->UIGroup->RemoveObject(label->GetObjectIterator());
        //     else if(mode == 3 || mode == 4) getMainPlayScene()->UIGroup->RemoveObject(label->GetObjectIterator());
        // }
        if(mode == 1 || mode == 2) getCharacterSelectScene()->UIGroup->RemoveObject(objectIterator);
        else if(mode == 3 || mode == 4) getMainPlayScene()->UIGroup->RemoveObject(objectIterator);
        return;
    }
}