#ifndef MainPlayScene_HPP
#define MainPlayScene_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include <vector>
#include <iostream>
using namespace std;
class MainPlayScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
	};
protected:
	int SpeedMult;
public:
	float ticks;
	int money1;
	int money2;
	vector<Engine::Image*> Mon1;
	vector<Engine::Image*> Mon2;
	Group* TileMapGroup;
	std::vector<std::vector<TileType>> mapState;
	int MapId;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
    explicit MainPlayScene() = default;
	void Initialize() override;
	void Terminate() override;
	void ReadMap();
	void DrawEmptyMoney();
	void UpdateMoney();
	void Update(float deltatime) override;
};

#endif // MainPlayScene_HPP