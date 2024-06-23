#ifndef HeadAnimation_HPP
#define HeadAnimation_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"
class HeadScene;

class HeadAnimation : public Engine::Sprite {
protected:
	HeadScene* getHeadScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 16.69;
public:
	HeadAnimation();
	void Update(float deltaTime) override;
};
#endif