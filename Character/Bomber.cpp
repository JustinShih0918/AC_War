#include <string>

#include "Bomber.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/Bomber_Bullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

Bomber::Bomber(int x, int y, int player) : Character("play/enemy-2.png", x, y, 200, 20, 200, 5, 1, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "fly";
	type = FLY;
	index = 3;
}


void Bomber::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(cos(Rotation ), sin(Rotation ));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new Bomber_Bullet(Position + normalized * 36, diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}