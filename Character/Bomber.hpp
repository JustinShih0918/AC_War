#ifndef Bomber_HPP
#define Bomber_HPP
#include "Character.hpp"

class Bomber : public Character {
public:
	Bomber(int x, int y, int player);
	void CreateBullet(Character* character) override;
};
#endif // Bomber_HPP
