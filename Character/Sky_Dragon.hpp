#ifndef Sky_DRAGON_HPP
#define Sky_DRAGON_HPP
#include "Character.hpp"

class Sky_Dragon : public Character {
public:
	Sky_Dragon(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // Sky_Dragon_HPP
