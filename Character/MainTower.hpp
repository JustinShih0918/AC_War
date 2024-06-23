#ifndef MainTower_HPP
#define MainTower_HPP
#include "Character.hpp"

class MainTower : public Character {
public:
	MainTower(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // StarArcher_HPP
