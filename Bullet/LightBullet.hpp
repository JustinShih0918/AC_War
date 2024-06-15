#ifndef LIGHTBULLET_HPP
#define LIGHTBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class LightBullet : public Bullet {
public:
	explicit LightBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent);
	void OnExplode(Character* character) override;
};
#endif // LightBullet_HPP