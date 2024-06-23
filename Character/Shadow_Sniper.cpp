#include <string>

#include "Shadow_Sniper.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/MissileBullet.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"

Shadow_Sniper::Shadow_Sniper(int x, int y, int player) : Character("play/enemy-4.png", x, y, 400, 30, 500, 3, 1, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "remote";
	type = REMOTE;
	index = 5;
}


void Shadow_Sniper::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(cos(Rotation), sin(Rotation));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 36, diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}