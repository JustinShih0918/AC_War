#include <allegro5/base.h>
#include <random>
#include <string>
#include "Character/Character.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "LightBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

class Turret;
LightBullet::LightBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent) :
	Bullet("play/bullet-7.png", 1000, 0.5, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent, nullptr) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'LightBullet.hpp', 'LightBullet.cpp' to create a new bullet.
}
void LightBullet::OnExplode(Character* character) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
	getMainPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), character->Position.x, character->Position.y));
}