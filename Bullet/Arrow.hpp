#ifndef ARROW_HPP
#define ARROW_HPP
#include <allegro5/base.h>
#include <list>

#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class Arrow : public Bullet {
protected:
	const float rotateRadian = 2 * ALLEGRO_PI;
	std::list<Bullet*>::iterator lockedBulletIterator;
public:
	explicit Arrow(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent, Character* target);
	void Update(float deltaTime) override;
	void OnExplode(Character* character) override;
};
#endif // Arrow_HPP