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
	static const std::vector<Engine::Point> directions;
	static Engine::Point TowerPoint_1[3];
	static Engine::Point TowerPoint_2[3];
	float ticks;
	int money1;
	int money2;
	int MapId;
	static bool DebugMode;
	vector<Engine::Image*> Mon1;
	vector<Engine::Image*> Mon2;
	Engine::Point player1;
	Engine::Point player2;
	pair<Engine::Image*,Engine::Image*> imgTarget1;
	pair<Engine::Image*, Engine::Image*> imgTarget2;
	Group* UIGroup;
	Group* TileMapGroup;
	Group* GroundEffectGroup;
	Group* DebugIndicatorGroup;
	Group* BulletGroup;
	Group* TowerGroup;
	Group* EnemyGroup;
	Group* EffectGroup;
	Group* GroundGroup_Player1;
	Group* GroundGroup_Player2;
	Group* TowerGroup_Player1;
	Group* TowerGroup_Player2;
	Group* FlyGroup_Player1;
	Group* FlyGroup_Player2;
	std::vector<std::vector<TileType>> mapState;
	std::vector<std::vector<int>> mapDistance_Player1;
	std::vector<std::vector<int>> mapDistance_Player2;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static Engine::Point GetClientSize();
    explicit MainPlayScene() = default;
	void Initialize() override;
	void Terminate() override;
	void ReadMap();
    void DrawEmptyMoney();
    void UpdateMoney();
	void Draw() const override;
	void Update(float deltatime) override;
	void OnKeyDown(int keyCode) override;
	void UpdateTarget(int player);
	bool CheckPosition(int mode, int input);
	std::vector<std::vector<int>> CalculateBFSDistance_Player1();
	std::vector<std::vector<int>> CalculateBFSDistance_Player2();
};

#endif // MainPlayScene_HPP