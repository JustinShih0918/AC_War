#include <cmath>
#include <string>

#include "TestAnimation.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "Scene/HeadScene.hpp"
#include <iostream>

using namespace std;
HeadScene* TestAnimation::getHeadScene() {
    return dynamic_cast<HeadScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

TestAnimation::TestAnimation() : Sprite("animation/test/0.png",800,416), timeTicks(0){
    for(int i = 0;i<8;i++){
        bmps.push_back(Engine::Resources::GetInstance().GetBitmap("animation/test/" + std::to_string(i) + ".png"));
    }
}

void TestAnimation::Update(float deltaTime) {
    timeTicks += deltaTime;
    if(timeTicks >= timeSpan){
        getHeadScene()->AnimationGroup->RemoveObject(objectIterator);
        return;
    }
    int phase = floor(timeTicks / timeSpan * bmps.size());
    bmp = bmps[phase];
    Sprite::Update(deltaTime);
}

