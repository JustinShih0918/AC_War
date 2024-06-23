#ifndef MainPlayScene_HPP
#define MainPlayScene_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include <vector>
#include <iostream>
#include <deque>

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
	int Win;
	static bool DebugMode;
	string character_img[7];
	Engine::Image* selectImg_1[3];
	Engine::Image* selectImg_2[3];
	deque<int> selected_1;
	deque<int> selected_2;
	vector<Engine::Image*> Mon1;
	vector<Engine::Image*> Mon2;
	Engine::Point player1;
	Engine::Point player2;
	string playerName_1;
	string playerName_2;
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
	std::vector<std::vector<int>> mapDistance_Player1_Middle;
	std::vector<std::vector<int>> mapDistance_Player1_Left;
	std::vector<std::vector<int>> mapDistance_Player1_Right;
	std::vector<std::vector<int>> mapDistance_Player2_Middle;
	std::vector<std::vector<int>> mapDistance_Player2_Left;
	std::vector<std::vector<int>> mapDistance_Player2_Right;
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
	void DoSelect(int player, int pos);
	void GetTransmitData();
	void UpdateSelected(int player, int pos);
	void UpdateSelectedImg();
	std::vector<std::vector<int>> CalculateBFSDistance(Engine::Point distination);
};

#endif // MainPlayScene_HPP