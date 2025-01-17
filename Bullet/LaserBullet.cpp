#include <allegro5/base.h>
#include <random>
#include <string>
#include "Character/Character.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "LaserBullet.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

class Turret;
LaserBullet::LaserBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent) :
	Bullet("play/bullet-2.png", 800, 2, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent, nullptr) {
}
void LaserBullet::OnExplode(Character* character) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
	getMainPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), character->Position.x, character->Position.y));
}
