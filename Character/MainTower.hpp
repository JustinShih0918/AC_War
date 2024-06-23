#ifndef MainTower_HPP
#define MainTower_HPP
#include "Character.hpp"
#include <iostream>
class MainTower : public Character {
public:
	MainTower(int x, int y, int player, std::string img);
	void CreateBullet(Character* character) override;
};
#endif // StarArcher_HPP
