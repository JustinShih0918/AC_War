#include <cmath>
#include <string>

#include "StartAnimation.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Scene/StartScene.h"
#include "Engine/Resources.hpp"

#include <iostream>
using namespace std;
StartScene* StartAnimation::getStartScene() {
    return dynamic_cast<StartScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

StartAnimation::StartAnimation() : Sprite("animation/start/0.png",800,416), timeTicks(0){
    for(int i = 0;i<60;i++){
        bmps.push_back(Engine::Resources::GetInstance().GetBitmap("animation/start/" + std::to_string(i) + ".png"));
    }
}

void StartAnimation::Update(float deltaTime) {
    timeTicks += deltaTime;
    if(timeTicks >= timeSpan){
        timeTicks = 0;
        return;
    }
    int phase = floor(timeTicks / timeSpan * bmps.size());
    cout << phase << "\n";
    bmp = bmps[phase];
    Sprite::Update(deltaTime);
}

