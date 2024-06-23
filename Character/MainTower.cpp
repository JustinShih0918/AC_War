#include <string>

#include "MainTower.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/MainTower_Bullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

MainTower::MainTower(int x, int y, int player, std::string img) : Character(img, x, y, 250, 0, 1500, 5, 1.5, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "tower";
	type = TOWER;
	index = 0;
}


void MainTower::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(Target->Position.x - Position.x, Target->Position.y - Position.y);
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	std::string a;
	if (player == 1) a = "mainPlay/Green_Tower_Bullet.png";
	else if (player == 2) a = "mainPlay/Red_Tower_Bullet.png";
	getMainPlayScene()->BulletGroup->AddNewObject(new MainTower_Bullet(Position + normalized * 36, diff, rotation, this, character, a));
	AudioHelper::PlayAudio("gun.wav");
}