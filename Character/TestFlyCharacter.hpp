#ifndef TESTFLYCHARACTER_HPP
#define TESTFLYCHARACTER_HPP
#include "Character.hpp"

class TestFlyCharacter : public Character {
public:
	TestFlyCharacter(int x, int y, int player);
	void CreateBullet() override;
};
#endif // TESTFlyCHARACTER_HPP
