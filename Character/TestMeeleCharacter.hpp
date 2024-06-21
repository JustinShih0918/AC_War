#ifndef TESTMEELECHARACTER_HPP
#define TESTMEELECHARACTER_HPP
#include "Character.hpp"

class TestMeeleCharacter : public Character {
public:
	TestMeeleCharacter(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // TESTCHARACTER_HPP
