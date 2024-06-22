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

void HeadScene::Initialize() {
    AddNewObject(AnimationGroup = new Group());
    animation = new HeadAnimation();
    AnimationGroup->AddNewObject(animation);
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