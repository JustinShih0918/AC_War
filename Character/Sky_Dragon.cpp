#include <string>

#include "Sky_Dragon.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/Sky_Dragon_Bullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

Sky_Dragon::Sky_Dragon(int x, int y, int player) : Character("play/enemy-2.png", x, y, 300, 30, 500, 5, 1, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "fly";
	type = FLY;
	index = 6;
}


void Sky_Dragon::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(cos(Rotation ), sin(Rotation ));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new Sky_Dragon_Bullet(Position + normalized * 36, diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}