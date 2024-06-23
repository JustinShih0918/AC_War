#ifndef CS_Student_HPP
#define CS_Student_HPP
#include "Character.hpp"

class CS_Student : public Character {
public:
	CS_Student(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // StarArcher_HPP
