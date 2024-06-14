#ifndef TESTCHARACTER_HPP
#define TESTCHARACTER_HPP
#include "Character.hpp"

class TestCharacter : public Character {
public:
	TestCharacter(int x, int y, int player);
	void CreateBullet() override;
};
#endif // TESTCHARACTER_HPP
