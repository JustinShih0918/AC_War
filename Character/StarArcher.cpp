#include <string>

#include "StarArcher.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/Star_Archer_Bullet.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"

StarArcher::StarArcher(int x, int y, int player) : Character("Mainplay/archer.png", x, y, 255, 20, 240, 3, 0.5, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "remote";
	type = REMOTE;
	index = 1;
}


void StarArcher::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(cos(Rotation), sin(Rotation));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new StarArcher_Bullet(Position + normalized * 36, diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}