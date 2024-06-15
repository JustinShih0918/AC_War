#ifndef TESTTOWERCHARACTER_HPP
#define TESTTOWERCHARACTER_HPP
#include "Character.hpp"

class TestTowerCharacter : public Character {
public:
	TestTowerCharacter(int x, int y, int player);
	void CreateBullet() override;
};
#endif // TESTCHARACTER_HPP
