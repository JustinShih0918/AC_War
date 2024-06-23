#ifndef STONETITAN_HPP
#define STONETITAN_HPP
#include "Character.hpp"

class StoneTitan : public Character {
public:
	StoneTitan(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // StarArcher_HPP
