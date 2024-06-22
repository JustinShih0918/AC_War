
#ifndef HeadScene_HPP
#define HeadScene_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Animation/HeadAnimation.hpp"
class HeadScene final : public Engine::IScene {
public:
    float tick = 0;
    HeadAnimation *animation;
    Engine::Group *AnimationGroup;
    explicit HeadScene() = default;
    void Initialize() override;
    void Terminate() override;
    void ChangeScene();
    //void Update(float deltaTime) override;
};
#endif