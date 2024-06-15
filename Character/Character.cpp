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
#include "Scene/PlayScene.hpp"
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
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"

PlayScene* Character::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
	
}
void Character::OnExplode() {
	getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
	for (int i = 0; i < 10; i++) {
		// Random add 10 dirty effects.
		getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
	}
}
 Character::Character(std::string img, float x, float y, float radius, float speed, float hp, int money, float coolDown, int player) :
	Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money), coolDown(coolDown), player(player) {
	CollisionRadius = 10;
	AttackRadius = radius;
	reachEndTime = 0;
}
void Character::UpdatePath(const std::vector<std::vector<int>>& mapDistance, std::string player) {
	int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
	int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
	if (x < 0) x = 0;
	if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
	if (y < 0) y = 0;
	if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;
	Engine::Point pos(x, y);
	int num = mapDistance[y][x];
	if (num == -1) {
		num = 0;
		Engine::LOG(Engine::ERROR) << "Enemy path finding error";
	}
	path = std::vector<Engine::Point>(num + 1);
	while (num != 0) {
		std::vector<Engine::Point> nextHops;
		for (auto& dir : PlayScene::directions) {
			int x = pos.x + dir.x;
			int y = pos.y + dir.y;
			if (x < 0 || x >= PlayScene::MapWidth || y < 0 || y >= PlayScene::MapHeight || mapDistance[y][x] != num - 1)
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
	if(player == "Player1")
		path[0] = PlayScene::EndGridPoint;
	else if(player == "Player2")
		path[0] = PlayScene::SpawnGridPoint;
	else
		path[0] = PlayScene::EndGridPoint;
}
void Character::Update(float deltaTime) {
	// Pre-calculate the velocity.
	if (Target) {
		Engine::Point diff = Target->Position - Position;
		if (diff.Magnitude() > CollisionRadius) {
			//Target->lockedTurrets.erase(lockedTurretIterator);
			Target = nullptr;
			//lockedTurretIterator = std::list<Turret*>::iterator();
		}
	}
	if (!Target) {
		// Lock first seen target.
		// Can be improved by Spatial Hash, Quad Tree, ...
		// However simply loop through all enemies is enough for this program.
		if (player == 1){
			for (auto& it : getPlayScene()->CharacterGroup_Player2->GetObjects()) {
				Engine::Point diff = it->Position - Position;
				if (diff.Magnitude() <= AttackRadius) {
					Target = dynamic_cast<Character*>(it);
					//Target->lockedTurrets.push_back(this);
					//lockedTurretIterator = std::prev(Target->lockedTurrets.end());
					break;
				}
			}
		}
		else if (player == 2){
			for (auto& it : getPlayScene()->CharacterGroup_Player1->GetObjects()) {
				Engine::Point diff = it->Position - Position;
				if (diff.Magnitude() <= AttackRadius) {
					Target = dynamic_cast<Character*>(it);
					//Target->lockedTurrets.push_back(this);
					//lockedTurretIterator = std::prev(Target->lockedTurrets.end());
					break;
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
		Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI / 2;
		// Shoot reload.
		reload -= deltaTime;
		if (reload <= 0) {
			// shoot.
			reload = coolDown;
			CreateBullet();
		}
	}
	float remainSpeed = speed * deltaTime;
	while (remainSpeed != 0) {
		if (path.empty()) {
			// Reach end point.
			Hit(hp);
			getPlayScene()->Hit();
			reachEndTime = 0;
			return;
		}
		Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
		Engine::Point vec = target - Position;
		// Add up the distances:
		// 1. to path.back()
		// 2. path.back() to border
		// 3. All intermediate block size
		// 4. to end point
		reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
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
	}
	//Rotation = atan2(Velocity.y, Velocity.x);
	Sprite::Update(deltaTime);
}

void Character::Hit(float damage) {
	hp -= damage;
	if(damage == 0.5){
		if(speed - 10 >= 20) speed -= 10;
	}
	int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
	int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
	if (hp <= 0) {
		OnExplode();
		// Remove all turret's reference to target.
		for (auto& it: lockedCharacters)
			it->Target = nullptr;
		for (auto& it: lockedBullets)
			it->Target = nullptr;
		if(money == 100){
			getPlayScene()->RangeExplode(Position.x,Position.y);
		}
		getPlayScene()->EarnMoney(money);
		getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("explosion.wav");
	}
}



void Character::Draw() const {
	Sprite::Draw();
	if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
	}
}
