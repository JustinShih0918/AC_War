#ifndef LASERBULLET_HPP
#define LASERBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class LaserBullet : public Bullet {
public:
	explicit LaserBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent);
	void OnExplode(Character* character) override;
};
#endif // LASERBULLET_HPP
