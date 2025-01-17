#include <string>

#include "Shadow_Sniper.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/Shadow_Sniper_Bullet.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"

Shadow_Sniper::Shadow_Sniper(int x, int y, int player) : Character("Mainplay/Shadow_Sniper.png", x, y, 360, 20, 200, 4, 1.5, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "remote";
	type = REMOTE;
	index = 5;
}


void Shadow_Sniper::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(0, 1);
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new Shadow_Sniper_Bullet(Engine::Point(character->Position.x,character->Position.y-100), diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}