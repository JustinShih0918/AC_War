#include <cmath>
#include <string>

#include "HeadAnimation.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Scene/HeadScene.hpp"
#include "Engine/Resources.hpp"

#include <iostream>
using namespace std;
HeadScene* HeadAnimation::getHeadScene() {
    return dynamic_cast<HeadScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

HeadAnimation::HeadAnimation() : Sprite("animation/head/0.png",800,416), timeTicks(0){
    for(int i = 0;i<500;i++){
        bmps.push_back(Engine::Resources::GetInstance().GetBitmap("animation/head/" + std::to_string(i) + ".png"));
    }
}

void HeadAnimation::Update(float deltaTime) {
    timeTicks += deltaTime;
    if(timeTicks >= timeSpan){
        getHeadScene()->AnimationGroup->RemoveObject(objectIterator);
        getHeadScene()->ChangeScene();
        return;
    }
    int phase = floor(timeTicks / timeSpan * bmps.size());
    bmp = bmps[phase];
    Sprite::Update(deltaTime);
}

