#ifndef StarArcher_HPP
#define StarArcher_HPP
#include "Character.hpp"

class StarArcher : public Character {
public:
	StarArcher(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // StarArcher_HPP
