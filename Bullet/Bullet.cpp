#include "Bullet.hpp"
#include "Engine/Collider.hpp"
#include "Enemy/Enemy.hpp"
#include "Character/Character.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include <iostream>

PlayScene* Bullet::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Bullet::OnExplode(Character* character) {
}
Bullet::Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Character* parent) :
	Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
	Velocity = forwardDirection.Normalize() * speed;
	Rotation = rotation;
	CollisionRadius = 4;
}
void Bullet::Update(float deltaTime) {
	Sprite::Update(deltaTime);
	PlayScene* scene = getPlayScene();
	// Can be improved by Spatial Hash, Quad Tree, ...
	// However simply loop through all enemies is enough for this program.
	if (parent->player == 1) {
		if (parent->type == "meele"){
			for (auto& it : scene->GroundGroup_Player2->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
			for (auto& it : scene->TowerGroup_Player2->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
		}
		else if (parent->type == "remote" || parent->type == "tower" || parent->type == "fly"){
			for (auto& it : scene->TowerGroup_Player2->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
			for (auto& it : scene->FlyGroup_Player2->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible){
						continue;
					}
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
			for (auto& it : scene->GroundGroup_Player2->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					std::cout<<"hi\n";
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
		}
	}
	else if(parent->player == 2) {
		if (parent->type == "meele"){
			for (auto& it : scene->GroundGroup_Player1->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
			for (auto& it : scene->TowerGroup_Player1->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
		}
		else if (parent->type == "remote" || parent->type == "tower" || parent->type == "fly"){
			for (auto& it : scene->TowerGroup_Player1->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
			for (auto& it : scene->FlyGroup_Player1->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
			for (auto& it : scene->GroundGroup_Player1->GetObjects()) {
				Character* character = dynamic_cast<Character*>(it);
				if (!character->Visible)
					continue;
				if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, character->Position, character->CollisionRadius)) {
					OnExplode(character);
					character->Hit(damage);
					getPlayScene()->BulletGroup->RemoveObject(objectIterator);
					return;
				}
			}
		}
	}
	
	// Check if out of boundary.
	if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
		getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}
