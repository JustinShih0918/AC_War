#include <string>

#include "CS_Student.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/MissileBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/MainPlayScene.hpp"
#include "Engine/Point.hpp"

CS_Student::CS_Student(int x, int y, int player) : Character("play/enemy-3.png", x, y, 70, 50, 2000, 2, 1, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "melle";
	type = MEELE;
	index = 2;
}


void CS_Student::CreateBullet(Character* character) {
	Engine::Point diff = Engine::Point(cos(Rotation), sin(Rotation));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getMainPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 36, diff, rotation, this, character));
	AudioHelper::PlayAudio("gun.wav");
}