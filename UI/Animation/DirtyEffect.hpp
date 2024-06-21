#ifndef DIRTYEFFECT_HPP
#define DIRTYEFFECT_HPP
#include <string>

#include "Engine/Sprite.hpp"

class MainPlayScene;

class DirtyEffect : public Engine::Sprite {
protected:
	float timeSpan;
	float alpha;
	MainPlayScene* getMainPlayScene();
public:
	DirtyEffect(std::string img, float timeSpan, float x, float y);
	void Update(float deltaTime) override;
};
#endif // DIRTYEFFECT_HPP
