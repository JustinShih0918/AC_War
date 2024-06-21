#ifndef TESTCHARACTER_HPP
#define TESTCHARACTER_HPP
#include "Character.hpp"

class TestCharacter : public Character {
public:
	TestCharacter(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // TESTCHARACTER_HPP
