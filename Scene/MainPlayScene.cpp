#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "CharacterSelectScene.hpp"
#include "MainPlayScene.hpp"
//#include "Character/TestTowerCharacter.cpp"
#include <queue>
using namespace std;
bool MainPlayScene::DebugMode = false;
const std::vector<Engine::Point> MainPlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int MainPlayScene::MapWidth = 15, MainPlayScene::MapHeight = 13;
const int MainPlayScene::BlockSize = 64;
Engine::Point MainPlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void MainPlayScene::Initialize() {
	SpeedMult = 1;
	ticks = 0;
	money1 = 0, money2 = 0;
	mapState.clear();
	SpeedMult = 1;
	player1 = Engine::Point(7,4);
	player2 = Engine::Point(7,10);
	imgTarget1.first = new Engine::Image("mainPlay/target.png", player1.x * BlockSize + 320, player1.y * BlockSize);
	imgTarget2.first = new Engine::Image("mainPlay/target.png", player2.x * BlockSize, player2.y * BlockSize);
	imgTarget1.second = new Engine::Image("mainPlay/target_green.png", player1.x * BlockSize + 320, player1.y * BlockSize - 50);
	imgTarget2.second = new Engine::Image("mainPlay/target_orange.png", player2.x * BlockSize, player2.y * BlockSize - 50);

	// TowerPoint_1.push_back(Engine::Point(3,3));
	// TowerPoint_1.push_back(Engine::Point(1,1));
	// TowerPoint_1.push_back(Engine::Point(4,4));

	// TowerPoint_2.push_back(Engine::Point(10,10));
	// TowerPoint_2.push_back(Engine::Point(11,11));
	// TowerPoint_2.push_back(Engine::Point(12,12));

	AddNewObject(new Engine::Image("mainPlay/Background.png", 0, 0, 0, 0, 0.0, 0.0));
	Engine::Label *lab;
    lab = new Engine::Label("Player1","OpenSans-Regular.ttf", 60, 38 + 150, 27 + 30, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(lab);
    lab = new Engine::Label("Player2","OpenSans-Regular.ttf", 60, 1239 + 150, 27 + 30, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(lab);
	AddNewObject(TileMapGroup = new Group());
	ReadMap();
	DrawEmptyMoney();
	UpdateMoney();
	mapDistance = CalculateBFSDistance();
	AddNewControlObject(UIGroup = new Group());
	UIGroup->AddNewObject(imgTarget1.first);
	UIGroup->AddNewObject(imgTarget1.second);
	UIGroup->AddNewObject(imgTarget2.first);
	UIGroup->AddNewObject(imgTarget2.second);

	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	AddNewObject(GroundGroup_Player1 = new Group());
	AddNewObject(GroundGroup_Player2 = new Group());
	AddNewObject(FlyGroup_Player1 = new Group());
	AddNewObject(FlyGroup_Player2 = new Group());
	AddNewObject(TowerGroup_Player1 = new Group());
	AddNewObject(TowerGroup_Player2 = new Group());
	// for(int i = 0;i<3;i++) TowerGroup_Player1->AddNewObject(new TestTowerCharacter(TowerPoint_1[i].x * BlockSize + BlockSize / 2, TowerPoint_1[i].y * BlockSize + BlockSize / 2, 1));
	// for(int i = 0;i<3;i++) TowerGroup_Player2->AddNewObject(new TestTowerCharacter(TowerPoint_2[i].x * BlockSize + BlockSize / 2, TowerPoint_2[i].y * BlockSize + BlockSize / 2, 1));

}

void MainPlayScene::Terminate() {
	IScene::Terminate();
}

void MainPlayScene::Update(float deltatime){
	IScene::Update(deltatime);
	ticks += deltatime;
	if(ticks >= 1.1){
		UpdateMoney();
		ticks = 0;
	}
}

void MainPlayScene::Draw() const{
	IScene::Draw();
	if(DebugMode){
		int initX = 320;
		cout << "Debug call\n";
		for (int i = 0; i < MapHeight; i++) {
			for (int j = 0; j < MapWidth; j++) {
				if (mapDistance[i][j] != -1) {
					// Not elegant nor efficient, but it's quite enough for debugging.
					Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize + initX, (i + 0.5) * BlockSize);
					label.Anchor = Engine::Point(0.5, 0.5);
					label.Draw();
				}
			}
		}
	}
}
void MainPlayScene::DrawEmptyMoney(){
	int initX_1 = 228;
	int initX_2 = 1308;
	int initY = 750;
	int detY = 65;
	Engine::Image* img;
	for(int i = 0;i<10;i++){
		img = new Engine::Image("mainPlay/empty.png", initX_1, initY - detY * i, 0, 0, 0.0, 0.0);
		AddNewObject(img);
	}
	for(int i = 0;i<10;i++){
		img = new Engine::Image("mainPlay/empty.png", initX_2, initY - detY * i, 0, 0, 0.0, 0.0);
		AddNewObject(img);
	}
}
void MainPlayScene::UpdateMoney(){
	int initX_1 = 228;
	int initX_2 = 1308;
	int initY = 750;
	int detY = 65;
	for(int i = 0;i<Mon1.size();i++){
		RemoveObject(Mon1[i]->GetObjectIterator());
	}

	for(int i = 0;i<Mon2.size();i++){
		RemoveObject(Mon2[i]->GetObjectIterator());
	}

	Mon1.clear();
	Mon2.clear();

	if(money1 < 10) money1++;
	if(money2 < 10) money2++;
	Engine::Image* img;
	for(int i = 0;i < money1;i++){
		img = new Engine::Image("mainPlay/money.png", initX_1, initY - detY * i, 0, 0, 0.0, 0.0);
		Mon1.push_back(img);
		AddNewObject(img);
	}
	for(int i = 0;i < money2;i++){
		img = new Engine::Image("mainPlay/money.png", initX_2, initY - detY * i, 0, 0, 0.0, 0.0);
		Mon2.push_back(img);
		AddNewObject(img);
	}
}

void MainPlayScene::ReadMap(){
	std::string filename = string("Resource/map3.txt");
	char c;
	std::vector<bool> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '0': mapData.push_back(false); break;
		case '1': mapData.push_back(true); break;
		case '\n':
		case '\r':
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.");
			break;
		default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();

	int initX = 320;
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.");
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			if (num)
				TileMapGroup->AddNewObject(new Engine::Image("mainPlay/floor.png", initX + j * BlockSize, i * BlockSize, BlockSize, BlockSize));
			else
				TileMapGroup->AddNewObject(new Engine::Image("mainPlay/dirt.png", initX + j * BlockSize, i * BlockSize, BlockSize, BlockSize));
		}
	}
}

std::vector<std::vector<int>> MainPlayScene::CalculateBFSDistance() {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	std::queue<Engine::Point> que;
	// Push end point.
	// BFS from end point.
	// if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
	// 	return map;
	que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
	map[MapHeight - 1][MapWidth - 1] = 0;

	while (!que.empty()) {
		Engine::Point p = que.front();
		que.pop();
		// TODO: [BFS PathFinding] (1/1): Implement a BFS starting from the most right-bottom block in the map.
		//               For each step you should assign the corresponding distance to the most right-bottom block.
		//               mapState[y][x] is TILE_DIRT if it is empty.
		std::vector<Engine::Point> directions = {Engine::Point(1,0), Engine::Point(-1,0), Engine::Point(0,1), Engine::Point(0,-1)};
		for(auto it : directions){
			int newX = p.x + it.x;
			int newY = p.y + it.y;
			if(newX >= 0 && newX < MapWidth && newY >= 0 && newY < MapHeight){
				if(map[newY][newX] == -1 && mapState[newY][newX] == TILE_DIRT){
					map[newY][newX] = map[p.y][p.x] + 1;
					que.push(Engine::Point(newX,newY));
				}
			}
		}
	}
	// for(int i = 0;i<map.size();i++){
	// 	for(int j = 0;j<map[i].size();j++){
	// 		cout << map[i][j] << " ";
	// 	}
	// 	cout << "\n";
	// }
	return map;
}

void MainPlayScene::OnKeyDown(int keyCode){
	IScene::OnKeyDown(keyCode);
	if(keyCode == ALLEGRO_KEY_TAB){
		cout << "debug mode open\n";
		DebugMode = !DebugMode;
	}
	else if(keyCode == ALLEGRO_KEY_W || keyCode == ALLEGRO_KEY_S || keyCode == ALLEGRO_KEY_A || keyCode == ALLEGRO_KEY_D){
		if(keyCode == ALLEGRO_KEY_W) player1.y--;
		else if(keyCode == ALLEGRO_KEY_S) player1.y++;
		else if(keyCode == ALLEGRO_KEY_A) player1.x--;
		else if(keyCode == ALLEGRO_KEY_D) player1.x++;
		UpdateTarget(1);
	}
	else if(keyCode == ALLEGRO_KEY_UP || keyCode == ALLEGRO_KEY_DOWN || keyCode == ALLEGRO_KEY_LEFT || keyCode == ALLEGRO_KEY_RIGHT){
		if(keyCode == ALLEGRO_KEY_UP) player2.y--;
		else if(keyCode == ALLEGRO_KEY_DOWN) player2.y++;
		else if(keyCode == ALLEGRO_KEY_LEFT) player2.x--;
		else if(keyCode == ALLEGRO_KEY_RIGHT) player2.x++;
		UpdateTarget(2);
	}
}

bool MainPlayScene::CheckPosition(int mode, int input){
	if(mode == 1){
		if(input < 0 || input >= MapWidth) return false;
	}
	else if(mode == 2){
		if(input < 0 || input >= MapHeight) return false;
	}

	return true;
}

void MainPlayScene::UpdateTarget(int player){
	if(player == 1){
		if(player1.x < 0 || player1.x >= MapWidth || player1.y < 0 || player1.y >= MapHeight){
			if(player1.x < 0) player1.x = 0;
			else if(player1.x >= MapWidth) player1.x = MapWidth-1;

			if(player1.y < 0) player1.y = 0;
			else if(player1.y >= MapHeight) player1.y = MapHeight-1; 
			return;
		}
		imgTarget1.first->Position.x = player1.x * BlockSize + 320;
		imgTarget1.first->Position.y = player1.y * BlockSize;
		imgTarget1.second->Position.x = player1.x * BlockSize + 320;
		imgTarget1.second->Position.y = player1.y * BlockSize - 50;
	}
	else if(player == 2){
		if(player2.x < 0 || player2.x >= MapWidth || player2.y < 0 || player2.y >= MapHeight){
			if(player2.x < 0) player2.x = 0;
			else if(player2.x >= MapWidth) player2.x = MapWidth-1;

			if(player2.y < 0) player2.y = 0;
			else if(player2.y >= MapHeight) player2.y = MapHeight-1; 
			return;
		}
		imgTarget2.first->Position.x = player2.x * BlockSize + 320;
		imgTarget2.first->Position.y = player2.y * BlockSize;
		imgTarget2.second->Position.x = player2.x * BlockSize + 320;
		imgTarget2.second->Position.y = player2.y * BlockSize - 50;
	}
}