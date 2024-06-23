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
#include "Star_Archer_Bullet.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"


StarArcher_Bullet::StarArcher_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent, Character* target) :
	Bullet("Mainplay/Star_Archer_Bullet.png", 300, 20, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent, target) {
		if(!Target)
			std::cout << "Target Miss!\n";
		Target->lockedBullets.push_back(this);
		lockedBulletIterator = std::prev(Target->lockedBullets.end());
}
void StarArcher_Bullet::Update(float deltaTime) {
	if (!Target) {
		float minDistance = INFINITY;
		Character* character = nullptr;
		if (parent->player == 1){
			if (parent->type == MEELE){
				for (auto& it : getMainPlayScene()->GroundGroup_Player2->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player2->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
			}
			else if (parent->type == REMOTE || parent->type == TOWER || parent->type == FLY){
				for (auto& it : getMainPlayScene()->GroundGroup_Player2->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player2->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
				for (auto& it : getMainPlayScene()->FlyGroup_Player2->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
			}
		}
		else if (parent->player == 2){
			if (parent->type == MEELE){
				for (auto& it : getMainPlayScene()->GroundGroup_Player1->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player1->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
			}
			else if (parent->type == REMOTE || parent->type == TOWER || parent->type == FLY){
				for (auto& it : getMainPlayScene()->GroundGroup_Player1->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player1->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
				}
				for (auto& it : getMainPlayScene()->FlyGroup_Player1->GetObjects()) {
					Character* c = dynamic_cast<Character*>(it);
					float distance = (c->Position - Position).Magnitude();
					if (distance < minDistance) {
						minDistance = distance;
						character = c;
					}
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
	Rotation = atan2(Velocity.y, Velocity.x) + ALLEGRO_PI / 4;
	Bullet::Update(deltaTime);
}
void StarArcher_Bullet::OnExplode(Character* character) {
	Target->lockedBullets.erase(lockedBulletIterator);
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
	getMainPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-3.png", 1, character->Position.x, character->Position.y));
}
