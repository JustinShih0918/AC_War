#ifndef TestAnimation_HPP
#define TestAnimation_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"
class HeadScene;

class TestAnimation : public Engine::Sprite {
protected:
	HeadScene* getHeadScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 1;
public:
	TestAnimation();
	void Update(float deltaTime) override;
};
#endif