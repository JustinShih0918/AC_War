#include "HeadScene.hpp"
#include <allegro5/allegro_audio.h>
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
#include "UI/Animation/HeadAnimation.hpp"
#include <iostream>
using namespace std;
void HeadScene::Initialize() {
    AddNewObject(AnimationGroup = new Group());
    
    animation = new HeadAnimation();
    AnimationGroup->AddNewObject(animation);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 1200 + 300, 200 - 200, 100, 40);
    btn->SetOnClickCallback(std::bind(&HeadScene::SkipOnClick,this,1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Skip", "pirulen.ttf", 24, 1250 + 300, 220 - 200, 0, 0, 0, 255, 0.5, 0.5));

}

void HeadScene::SkipOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

// void HeadScene::Update(float deltaTime){
//     tick += deltaTime;
//     //if(animation->finish) Engine::GameEngine::GetInstance().ChangeScene("start");
// }
void HeadScene::ChangeScene(){
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void HeadScene::Terminate() {
    IScene::Terminate();
}