#ifndef SHADOW_SNIPER_HPP
#define SHADOW_SNIPER_HPP
#include "Character.hpp"

class Shadow_Sniper : public Character {
public:
	Shadow_Sniper(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // Shadow_Sniper_HPP
