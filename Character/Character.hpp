#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <list>
#include <vector>
#include <string>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include <allegro5/base.h>
#include <list>
#include <string>
#include "Scene/MainPlayScene.hpp"
#include "Engine/Sprite.hpp"

class Bullet;
class MainPlayScene;
class Turret;

enum CharacterType {
	MEELE,REMOTE,FLY,TOWER,
};

class Character : public Engine::Sprite {
protected:
 	std::vector<Engine::Point> path;
	float speed;
	float hp;
	int money;
	float coolDown;
	float reload = 0;
	float rotateRadian = 2 * ALLEGRO_PI;
	float AttackRadius;
	MainPlayScene* getMainPlayScene();
	virtual void OnExplode();
public:
	int player;
	int index;
	float reachEndTime;
	std::list<Character*> lockedCharacters;
	std::list<Character*>::iterator lockedCharacterIterator; // for the one attaking this one.
	std::list<Bullet*> lockedBullets;
	Character(std::string img, float x, float y, float radius, float speed, float hp, int money, float coolDown, int player);
 	void Hit(float damage);
	void UpdatePath(const std::vector<std::vector<int>>& mapDistance, std::string player);
	void Update(float deltaTime) override;
	void Draw() const override;
	virtual void CreateBullet(Character* character) = 0;
	Character* Target = nullptr;
	std::string name;
	CharacterType type;
	int getMoney();
};
#endif // ENEMY_HPP
