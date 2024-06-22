#include <string>

#include "TestTowerCharacter.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/MissileBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

TestTowerCharacter::TestTowerCharacter(int x, int y, int player) : Character("play/enemy-1.png", x, y, 300, 0, 1200, 5, 1, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "tower";
	type = TOWER;
}


void TestTowerCharacter::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(Target->Position.x - Position.x, Target->Position.y - Position.y);
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 36, diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}