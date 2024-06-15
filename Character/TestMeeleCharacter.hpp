#ifndef TESTMEELECHARACTER_HPP
#define TESTMEELECHARACTER_HPP
#include "Character.hpp"

class TestMeeleCharacter : public Character {
public:
	TestMeeleCharacter(int x, int y, int player);
	void CreateBullet() override;
};
#endif // TESTCHARACTER_HPP
