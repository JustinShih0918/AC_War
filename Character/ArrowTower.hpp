#ifndef ARROWTOWER_HPP
#define ARROWTOWER_HPP
#include "Character.hpp"

class ArrowTower : public Character {
public:
	ArrowTower(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // StarArcher_HPP
