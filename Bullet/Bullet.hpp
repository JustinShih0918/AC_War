#ifndef BULLET_HPP
#define BULLET_HPP
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;
class Character;
namespace Engine {
struct Point;
}  // namespace Engine

class Bullet : public Engine::Sprite {
protected:
	float speed;
	float damage;
	Character* parent;
	PlayScene* getPlayScene();
	virtual void OnExplode(Character* character);
public:
	Character* Target = nullptr;
	explicit Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent);
	void Update(float deltaTime) override;
};
#endif // BULLET_HPP
