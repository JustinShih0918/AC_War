#include <string>

#include "TestMeeleCharacter.hpp"
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/MissileBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

TestMeeleCharacter::TestMeeleCharacter(int x, int y, int player) : Character("play/enemy-3.png", x, y, 50, 50, 2000, 5, 1, player) {
	// TODO: [CUSTOM-TOOL] You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
	name = "melle";
	type = "meele";
}


void TestMeeleCharacter::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position + normalized * 36, diff, rotation, this));
	AudioHelper::PlayAudio("gun.wav");
}