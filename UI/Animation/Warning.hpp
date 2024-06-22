#ifndef Warning_HPP
#define Warning_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>
#include "UI/Component/Label.hpp"
#include "Engine/Sprite.hpp"
class CharacterSelectScene;
class MainPlayScene;

class Warning : public Engine::Label {
protected:
    MainPlayScene* getMainPlayScene();
	CharacterSelectScene* getCharacterSelectScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 2;
public:
    std::string warning;
    int mode;
    int posX;
    int posY;
	Engine::Label* label;
	bool hasAdd;
	Warning(int m, int x, int y);
	void Update(float deltaTime) override;
};
#endif