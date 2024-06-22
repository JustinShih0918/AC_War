#ifndef StartAnimation_HPP
#define StartAnimation_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"
class StartScene;

class StartAnimation : public Engine::Sprite {
protected:
	StartScene* getStartScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 2;
public:
	StartAnimation();
	void Update(float deltaTime) override;
};
#endif