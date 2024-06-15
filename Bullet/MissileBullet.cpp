#include <cmath>
#include <random>
#include <string>
#include <utility>
#include <iostream>
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Character/Character.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "MissileBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

class Turret;

MissileBullet::MissileBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent) :
	Bullet("play/bullet-3.png", 1000, 20, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent) {
}
void MissileBullet::Update(float deltaTime) {
	if (!Target) {
		float minDistance = INFINITY;
		Character* character = nullptr;
		if (parent->player == 1){
			for (auto& it : getPlayScene()->CharacterGroup_Player2->GetObjects()) {
				Character* c = dynamic_cast<Character*>(it);
				float distance = (c->Position - Position).Magnitude();
				if (distance < minDistance) {
					minDistance = distance;
					character = c;
				}
			}
		}
		else if (parent->player == 2){
			for (auto& it : getPlayScene()->CharacterGroup_Player1->GetObjects()) {
				Character* c = dynamic_cast<Character*>(it);
				float distance = (c->Position - Position).Magnitude();
				if (distance < minDistance) {
					minDistance = distance;
					character = c;
				}
			}
		}
		else std::cout << "MissileBullet Update error\n";
		if (!character) {
			Bullet::Update(deltaTime);
			return;
		}
		Target = character;
		Target->lockedBullets.push_back(this);
		lockedBulletIterator = std::prev(Target->lockedBullets.end());
	}
	Engine::Point originVelocity = Velocity.Normalize();
	Engine::Point targetVelocity = (Target->Position - Position).Normalize();
	float maxRotateRadian = rotateRadian * deltaTime;
	float cosTheta = originVelocity.Dot(targetVelocity);
	// Might have floating-point precision error.
	if (cosTheta > 1) cosTheta = 1;
	else if (cosTheta < -1) cosTheta = -1;
	float radian = acos(cosTheta);
	if (abs(radian) <= maxRotateRadian)
		Velocity = targetVelocity;
	else
		Velocity = ((abs(radian) - maxRotateRadian) * originVelocity + maxRotateRadian * targetVelocity) / radian;
	Velocity = speed * Velocity.Normalize();
	Rotation = atan2(Velocity.y, Velocity.x) + ALLEGRO_PI / 2;
	Bullet::Update(deltaTime);
}
void MissileBullet::OnExplode(Character* character) {
	Target->lockedBullets.erase(lockedBulletIterator);
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(4, 10);
	getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-3.png", dist(rng), character->Position.x, character->Position.y));
}
