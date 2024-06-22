#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Engine/AudioHelper.hpp"
#include "Bullet/Bullet.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Character.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Turret/Turret.hpp"
#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>

#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include "Scene/MainPlayScene.hpp"

MainPlayScene* Character::getMainPlayScene() {
	return dynamic_cast<MainPlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Character::OnExplode() {
	getMainPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
	for (int i = 0; i < 10; i++) {
		// Random add 10 dirty effects.
		getMainPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
	}
}
 Character::Character(std::string img, float x, float y, float radius, float speed, float hp, int money, float coolDown, int player) :
	Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money), coolDown(coolDown),reload(coolDown) , player(player) {
	CollisionRadius = 10;
	AttackRadius = radius;
	reachEndTime = 0;
}
void Character::UpdatePath(const std::vector<std::vector<int>>& mapDistance, std::string player) {
	int x = static_cast<int>(floor((Position.x - 320 - MainPlayScene::BlockSize/2) / MainPlayScene::BlockSize));
	int y = static_cast<int>(floor((Position.y - MainPlayScene::BlockSize/2)/ MainPlayScene::BlockSize));
	if (x < 0) x = 0;
	if (x >= MainPlayScene::MapWidth) x = MainPlayScene::MapWidth - 1;
	if (y < 0) y = 0;
	if (y >= MainPlayScene::MapHeight) y = MainPlayScene::MapHeight - 1;
	Engine::Point pos(x, y);

	int num = mapDistance[y][x];
	if (num == -1) {
		num = 0;
		Engine::LOG(Engine::ERROR) << "Enemy path finding error";
	}
	path = std::vector<Engine::Point>(num + 1);
	while (num != 0) {
		std::vector<Engine::Point> nextHops;
		for (auto& dir : MainPlayScene::directions) {
			int x = pos.x + dir.x;
			int y = pos.y + dir.y;
			if (x < 0 || x >= MainPlayScene::MapWidth || y < 0 || y >= MainPlayScene::MapHeight || mapDistance[y][x] != num - 1)
				continue;
			nextHops.emplace_back(x, y);
		}
		// Choose arbitrary one.
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
		pos = nextHops[dist(rng)];
		path[num] = pos;
		num--;
	}
	if(player == "Player1") {
		if(mapDistance == getMainPlayScene()->mapDistance_Player1_Middle)
			path[0] = MainPlayScene::TowerPoint_2[0];
		else if(mapDistance == getMainPlayScene()->mapDistance_Player1_Left)
			path[0] = MainPlayScene::TowerPoint_2[1];
		else if(mapDistance == getMainPlayScene()->mapDistance_Player1_Right)
			path[0] = MainPlayScene::TowerPoint_2[2];
		else
			cout << "path[0] set error\n";
	}
	else if(player == "Player2")
		if(mapDistance == getMainPlayScene()->mapDistance_Player2_Middle)
			path[0] = MainPlayScene::TowerPoint_1[0];
		else if(mapDistance == getMainPlayScene()->mapDistance_Player2_Left)
			path[0] = MainPlayScene::TowerPoint_1[1];
		else if(mapDistance == getMainPlayScene()->mapDistance_Player2_Right)
			path[0] = MainPlayScene::TowerPoint_1[2];
		else
			cout << "path[0] set error\n";
	else {
		path[0] = Engine::Point(0,0);
		cout << "Player undefine\n";
	}
}
void Character::Update(float deltaTime) {
	Sprite::Update(deltaTime);
	// Pre-calculate the velocity.
	/*
	if(player == 1) {
		if(Position.x > MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
			UpdatePath(getMainPlayScene()->mapDistance_Player1_Right,"Player1");
		else if(Position.x <= MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
			UpdatePath(getMainPlayScene()->mapDistance_Player1_Left,"Player1");
		else
			cout << "path update error\n";
	}
	else if(player == 2) {
		if(Position.x > MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
			UpdatePath(getMainPlayScene()->mapDistance_Player2_Right,"Player2");
		else if(Position.x <= MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
			UpdatePath(getMainPlayScene()->mapDistance_Player2_Left,"Player2");
		else
			cout << "path update error\n";
	}
	*/
	if (Target) {
		Engine::Point diff = Target->Position - Position;
		if (diff.Magnitude() > CollisionRadius) {
			Target->lockedCharacters.erase(lockedCharacterIterator);
			Target = nullptr;
			lockedCharacterIterator = std::list<Character*>::iterator();
		}
	}
	if (!Target) {
		Rotation = atan2(Velocity.y, Velocity.x); // facing front
		// Lock first seen target.
		// Can be improved by Spatial Hash, Quad Tree, ...
		// However simply loop through all enemies is enough for this program.
		if (player == 1){
			if(type == MEELE) {
				for (auto& it : getMainPlayScene()->GroundGroup_Player2->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player2->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
			}
			else if(type == REMOTE || type == TOWER || type == FLY) {
				for (auto& it : getMainPlayScene()->GroundGroup_Player2->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player2->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
				for (auto& it : getMainPlayScene()->FlyGroup_Player2->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
			}
		}
		else if (player == 2){
			if(type == MEELE) {
				for (auto& it : getMainPlayScene()->GroundGroup_Player1->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player1->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
			}
			else if(type == REMOTE || type == TOWER || type == FLY) {
				for (auto& it : getMainPlayScene()->GroundGroup_Player1->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
				for (auto& it : getMainPlayScene()->TowerGroup_Player1->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
				for (auto& it : getMainPlayScene()->FlyGroup_Player1->GetObjects()) {
					Engine::Point diff = it->Position - Position;
					if (diff.Magnitude() <= AttackRadius) {
						Target = dynamic_cast<Character*>(it);
						Target->lockedCharacters.push_back(this);
						lockedCharacterIterator = std::prev(Target->lockedCharacters.end());
						break;
					}
				}
			}
		}
		else {
			std::cout << "character attack error\n";
		}
	}
	if (Target) {
		Engine::Point originRotation = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
		Engine::Point targetRotation = (Target->Position - Position).Normalize();
		float maxRotateRadian = rotateRadian * deltaTime;
		float cosTheta = originRotation.Dot(targetRotation);
		// Might have floating-point precision error.
		if (cosTheta > 1) cosTheta = 1;
		else if (cosTheta < -1) cosTheta = -1;
		float radian = acos(cosTheta);
		Engine::Point rotation;
		if (abs(radian) <= maxRotateRadian)
			rotation = targetRotation;
		else
			rotation = ((abs(radian) - maxRotateRadian) * originRotation + maxRotateRadian * targetRotation) / radian;
		// Add 90 degrees (PI/2 radian), since we assume the image is oriented upward.
		//Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI/2;
		Rotation = atan2(Target->Position.y - Position.y, Target->Position.x - Position.x);
		// Shoot reload.
		reload -= deltaTime;
		if (reload <= 0) {
			// shoot.
			reload = coolDown;
			CreateBullet(Target);
		}
	}
	float remainSpeed = speed * deltaTime;
	while (remainSpeed != 0 || type == TOWER) {
		Engine::Point target = path.back() * MainPlayScene::BlockSize + Engine::Point(MainPlayScene::BlockSize / 2 + 320, MainPlayScene::BlockSize / 2);
		Engine::Point vec = target - Position;
		// Add up the distances:
		// 1. to path.back()
		// 2. path.back() to border
		// 3. All intermediate block size
		// 4. to end point
		reachEndTime = (vec.Magnitude() + (path.size() - 1) * MainPlayScene::BlockSize - remainSpeed) / speed;
		Engine::Point normalized = vec.Normalize();
		if (remainSpeed - vec.Magnitude() > 0) {
			Position = target;
			path.pop_back();
			remainSpeed -= vec.Magnitude();
		}
		else {
			Velocity = normalized * remainSpeed / deltaTime;
			remainSpeed = 0;
		}
		if(Target)
			Velocity = normalized * 0;
		if(type == TOWER) break;
	}
	//Rotation = atan2(Velocity.y, Velocity.x);
}

void Character::Hit(float damage) {
	hp -= damage;
	if (hp <= 0) {
		OnExplode();
		// Remove all turret's reference to target.
		for (auto& it: lockedCharacters)
			it->Target = nullptr;
		for (auto& it: lockedBullets)
			it->Target = nullptr;
		if(player == 1) {
			if(type == MEELE || type == REMOTE)
				getMainPlayScene()->GroundGroup_Player1->RemoveObject(objectIterator);
			else if(type == FLY)
				getMainPlayScene()->FlyGroup_Player1->RemoveObject(objectIterator);
			else if(type == TOWER) {
				Engine::Point BlockPoint = Engine::Point((int)(Position.x-320)/MainPlayScene::BlockSize,(int)(Position.y)/MainPlayScene::BlockSize);
				if(BlockPoint == getMainPlayScene()->TowerPoint_1[1])
					getMainPlayScene()->mapDistance_Player2_Left = getMainPlayScene()->mapDistance_Player2_Middle;
				else if(BlockPoint == getMainPlayScene()->TowerPoint_1[2])
					getMainPlayScene()->mapDistance_Player2_Right = getMainPlayScene()->mapDistance_Player2_Middle;
				else if(BlockPoint == getMainPlayScene()->TowerPoint_1[0]){
					//Engine::GameEngine::GetInstance().ChangeScene("win");
				}
				else
					cout << "Tower destroy error\n";
				getMainPlayScene()->TowerGroup_Player1->RemoveObject(objectIterator);
				for (auto& it : getMainPlayScene()->GroundGroup_Player2->GetObjects()){
					if(it->Position.x > MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player2_Right,"Player2");
					else if(it->Position.x <= MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player2_Left,"Player2");
					else
						cout << "path update error\n";
				}
				for (auto& it : getMainPlayScene()->FlyGroup_Player2->GetObjects()){
					if(it->Position.x > MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player2_Right,"Player2");
					else if(it->Position.x <= MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player2_Left,"Player2");
					else
						cout << "path update error\n";
				}
			}
			else 
				cout << "die type error\n\n";
		}
		else if(player == 2) {
			if(type == MEELE || type == REMOTE)
				getMainPlayScene()->GroundGroup_Player2->RemoveObject(objectIterator);
			else if(type == FLY)
				getMainPlayScene()->FlyGroup_Player2->RemoveObject(objectIterator);
			else if(type == TOWER) {
				Engine::Point BlockPoint = Engine::Point((int)(Position.x-320)/MainPlayScene::BlockSize,(int)(Position.y)/MainPlayScene::BlockSize);
				if(BlockPoint == getMainPlayScene()->TowerPoint_2[1])
					getMainPlayScene()->mapDistance_Player1_Left = getMainPlayScene()->mapDistance_Player1_Middle;
				else if(BlockPoint == getMainPlayScene()->TowerPoint_2[2])
					getMainPlayScene()->mapDistance_Player1_Right = getMainPlayScene()->mapDistance_Player1_Middle;
				else if(BlockPoint == getMainPlayScene()->TowerPoint_2[0]){
					//Engine::GameEngine::GetInstance().ChangeScene("win");
				}
				else
					cout << "Tower destroy error\n";
				getMainPlayScene()->TowerGroup_Player2->RemoveObject(objectIterator);
				for (auto& it : getMainPlayScene()->GroundGroup_Player1->GetObjects()){
					if(it->Position.x > MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player1_Right,"Player1");
					else if(it->Position.x <= MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player1_Left,"Player1");
					else
						cout << "path update error\n";
				}
				for (auto& it : getMainPlayScene()->FlyGroup_Player1->GetObjects()){
					if(it->Position.x > MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player1_Right,"Player1");
					else if(it->Position.x <= MainPlayScene::MapWidth * MainPlayScene::BlockSize / 2 + 320)
						dynamic_cast<Character*>(it)->UpdatePath(getMainPlayScene()->mapDistance_Player1_Left,"Player1");
					else
						cout << "path update error\n";
				}
			}
			else 
				cout << "die type error\n\n";
			
		}
		else 
			cout << "die player error\n\n";
		AudioHelper::PlayAudio("explosion.wav");
	}
}
void Character::Draw() const {
	Sprite::Draw();
	if (MainPlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_filled_circle(Position.x, Position.y, AttackRadius, al_map_rgba(0, 255, 0, 50));
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
	}
}
