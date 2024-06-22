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
#include "Character/TestTowerCharacter.hpp"
#include "Character/TestMeeleCharacter.hpp"
#include "Character/TestFlyCharacter.hpp"
#include "Character/TestCharacter.hpp"
#include "UI/Animation/Warning.hpp"
#include <iostream>
#include <queue>

using namespace std;
bool MainPlayScene::DebugMode = false;
const std::vector<Engine::Point> MainPlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int MainPlayScene::MapWidth = 15, MainPlayScene::MapHeight = 13;
const int MainPlayScene::BlockSize = 64;
Engine::Point MainPlayScene::TowerPoint_1[3] = {Engine::Point(7,1), Engine::Point(3,2), Engine::Point(11,2)};
Engine::Point MainPlayScene::TowerPoint_2[3] = {Engine::Point(7,11), Engine::Point(3,10), Engine::Point(11,10)};
Engine::Point MainPlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize + 320, MapHeight * BlockSize);
}
void MainPlayScene::Initialize() {
	AddNewObject(new Engine::Image("mainPlay/Background.png", 0, 0, 0, 0, 0.0, 0.0));
	SpeedMult = 1;
	int initX = 320;
	ticks = 0;
	Mon1.clear();
	Mon2.clear();
	Win = 0;
	money1 = 0, money2 = 0;
	mapState.clear();
	SpeedMult = 1;
	player1 = Engine::Point(7,4);
	player2 = Engine::Point(7,10);
	GetTransmitData();
	imgTarget1.first = new Engine::Image("mainPlay/target.png", player1.x * BlockSize + 320, player1.y * BlockSize);
	imgTarget2.first = new Engine::Image("mainPlay/target.png", player2.x * BlockSize, player2.y * BlockSize);
	imgTarget1.second = new Engine::Image("mainPlay/target_green.png", player1.x * BlockSize + 320, player1.y * BlockSize - 50);
	imgTarget2.second = new Engine::Image("mainPlay/target_orange.png", player2.x * BlockSize, player2.y * BlockSize - 50);

	Engine::Label *lab;
    lab = new Engine::Label(playerName_1,"OpenSans-Regular.ttf", 60, 38 + 120, 27 + 30, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(lab);
    lab = new Engine::Label(playerName_2,"OpenSans-Regular.ttf", 60, 1239 + 180, 27 + 30, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(lab);

	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	AddNewObject(GroundGroup_Player1 = new Group());
	AddNewObject(GroundGroup_Player2 = new Group());
	AddNewObject(FlyGroup_Player1 = new Group());
	AddNewObject(FlyGroup_Player2 = new Group());
	AddNewObject(TowerGroup_Player1 = new Group());
	AddNewObject(TowerGroup_Player2 = new Group());
	ReadMap();
	mapDistance_Player1_Middle = CalculateBFSDistance(TowerPoint_2[0]);
	mapDistance_Player1_Left = CalculateBFSDistance(TowerPoint_2[1]);
	mapDistance_Player1_Right = CalculateBFSDistance(TowerPoint_2[2]);
	mapDistance_Player2_Middle = CalculateBFSDistance(TowerPoint_1[0]);
	mapDistance_Player2_Left = CalculateBFSDistance(TowerPoint_1[1]);
	mapDistance_Player2_Right = CalculateBFSDistance(TowerPoint_1[2]);
	// mapDistance_Player"X" means X need to call the function.

	for(int i = 0;i<3;i++) {
		TestTowerCharacter* character = new TestTowerCharacter(TowerPoint_1[i].x * BlockSize + BlockSize / 2 + initX, TowerPoint_1[i].y * BlockSize + BlockSize / 2, 1);
		character->UpdatePath(mapDistance_Player1_Middle, "Player1");
		TowerGroup_Player1->AddNewObject(character);
	}
	for(int i = 0;i<3;i++) {
		TestTowerCharacter* character = new TestTowerCharacter(TowerPoint_2[i].x * BlockSize + BlockSize / 2 + initX, TowerPoint_2[i].y * BlockSize + BlockSize / 2, 2);
		character->UpdatePath(mapDistance_Player2_Middle, "Player2");
		TowerGroup_Player2->AddNewObject(character);
	}
	DrawEmptyMoney();
	UpdateMoney();
	AddNewControlObject(UIGroup = new Group());
	
	UIGroup->AddNewObject(imgTarget1.first);
	UIGroup->AddNewObject(imgTarget1.second);
	UIGroup->AddNewObject(imgTarget2.first);
	UIGroup->AddNewObject(imgTarget2.second);
}

void MainPlayScene::Terminate() {
	IScene::Terminate();
}

void MainPlayScene::Update(float deltatime){
	IScene::Update(deltatime);
	UpdateMoney();
	ticks += deltatime;
	if(Win){
		Engine::GameEngine::GetInstance().ChangeScene("win");
	}
	if(ticks >= 1.1){
		if (money1 < 10) money1++;
		if (money2 < 10) money2++;
		ticks = 0;
	}
}

void MainPlayScene::Draw() const{
	IScene::Draw();
	if(DebugMode){
		int initX = 320;
		for (int i = 0; i < MapHeight; i++) {
			for (int j = 0; j < MapWidth; j++) {
				if (mapDistance_Player2_Middle[i][j] != -1) {
					// Not elegant nor efficient, but it's quite enough for debugging.
					Engine::Label label(std::to_string(mapDistance_Player2_Middle[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize + initX, (i + 0.5) * BlockSize);
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
	else if(keyCode == ALLEGRO_KEY_1 || keyCode == ALLEGRO_KEY_2 || keyCode == ALLEGRO_KEY_3) DoSelect(1, keyCode - 28);
	else if(keyCode == ALLEGRO_KEY_8 || keyCode == ALLEGRO_KEY_9 || keyCode == ALLEGRO_KEY_7) DoSelect(2, keyCode - 34);
}

void MainPlayScene::DoSelect(int player, int pos){
	if(player == 1){
		if(selected_1[pos] == 1){
			TestMeeleCharacter *acter = new TestMeeleCharacter(player1.x * BlockSize + BlockSize / 2 + 320, player1.y * BlockSize, 1);
			if (money1 >= acter->getMoney() && mapState[player1.y][player1.x] == TILE_DIRT){
				if(player1.x > 7){
					cout << "Trace player2 right\n";
					acter->UpdatePath(mapDistance_Player1_Right, "Player1");
				}
				else if(player1.x <= 7)
					acter->UpdatePath(mapDistance_Player1_Left, "Player1");
				else
					cout << "path determine error\n";
				GroundGroup_Player1->AddNewObject(acter);
				money1 -= acter->getMoney();
				UpdateSelected(1,0);
			}
			else{
				if(money1 < acter->getMoney()) UIGroup->AddNewObject(new Warning(3,80,100, 24));
				else if(mapState[player1.y][player1.x] != TILE_DIRT) UIGroup->AddNewObject(new Warning(4,80,100, 24));
			}
		}
		else if(selected_1[pos] == 2){
			TestFlyCharacter *acter = new TestFlyCharacter(player1.x * BlockSize + BlockSize / 2 + 320, player1.y * BlockSize, 1);
			if (money1 >= acter->getMoney() && mapState[player1.y][player1.x] == TILE_DIRT){
				if(player1.x > 7)
					acter->UpdatePath(mapDistance_Player1_Right, "Player1");	
				else if(player1.x <= 7)
					acter->UpdatePath(mapDistance_Player1_Left, "Player1");
				else
					cout << "path determine error\n";
				FlyGroup_Player1->AddNewObject(acter);
				money1 -= acter->getMoney();
				UpdateSelected(1,1);
			}
			else{
				if(money1 < acter->getMoney()) UIGroup->AddNewObject(new Warning(3,80,100,24));
				else if(mapState[player1.y][player1.x] != TILE_DIRT) UIGroup->AddNewObject(new Warning(4,80,100,24));
			}
		}
		else if(selected_1[pos] == 3){
			TestCharacter *acter = new TestCharacter(player1.x * BlockSize + BlockSize / 2 + 320, player1.y * BlockSize, 1);
			if (money1 >= acter->getMoney() && mapState[player1.y][player1.x] == TILE_DIRT){
				if(player1.x > 7)
					acter->UpdatePath(mapDistance_Player1_Right, "Player1");
				else if(player1.x <= 7)
					acter->UpdatePath(mapDistance_Player1_Left, "Player1");
				else
					cout << "path determine error\n";
				GroundGroup_Player1->AddNewObject(acter);
				money1 -= acter->getMoney();
				UpdateSelected(1,2);
			}
			else{
				if(money1 < acter->getMoney()) UIGroup->AddNewObject(new Warning(3,80,100,24));
				else if(mapState[player1.y][player1.x] != TILE_DIRT) UIGroup->AddNewObject(new Warning(4,80,100,24));
			}
		}
	}
	else if(player == 2){
		if(selected_2[pos] == 1){
			cout << "Player2 x coordinate:" << player2.x << "  "<< MapWidth * BlockSize / 2 + 320 << "\n";
			TestMeeleCharacter *acter = new TestMeeleCharacter(player2.x * BlockSize + BlockSize / 2 + 320, player2.y * BlockSize, 2);
			if (money2 >= acter->getMoney() && mapState[player2.y][player2.x] == TILE_DIRT){
				if(player2.x > 7)
					acter->UpdatePath(mapDistance_Player2_Right, "Player2");
				else if(player2.x <= 7)
					acter->UpdatePath(mapDistance_Player2_Left, "Player2");
				else
					cout << "Path Determine Error\n";
				GroundGroup_Player2->AddNewObject(acter);
				money2 -= acter->getMoney();
				UpdateSelected(2,0);
			}
			else{
				if(money2 < acter->getMoney()) UIGroup->AddNewObject(new Warning(3,1400,100, 24));
				else if(mapState[player2.y][player2.x] != TILE_DIRT) UIGroup->AddNewObject(new Warning(4,1400,100, 24));
			}
		}
		else if(selected_2[pos] == 2){
			TestFlyCharacter *acter = new TestFlyCharacter(player2.x * BlockSize + BlockSize / 2 + 320, player2.y * BlockSize, 2);
			if (money2 >= acter->getMoney() && mapState[player2.y][player2.x] == TILE_DIRT){
				if(player2.x > 7)
					acter->UpdatePath(mapDistance_Player2_Right, "Player2");
				else if(player2.x <= 7)
					acter->UpdatePath(mapDistance_Player2_Left, "Player2");
				else
					cout << "Path Determine Error\n";
				FlyGroup_Player2->AddNewObject(acter);
				money2 -= acter->getMoney();
				UpdateSelected(2,1);
			}
			else{
				if(money2 < acter->getMoney()) UIGroup->AddNewObject(new Warning(3,1400,100, 24));
				else if(mapState[player2.y][player2.x] != TILE_DIRT) UIGroup->AddNewObject(new Warning(4,1400,100, 24));
			}
		}
		else if(selected_2[pos] == 3){
			TestCharacter *acter = new TestCharacter(player2.x * BlockSize + BlockSize / 2 + 320, player2.y * BlockSize, 2);
			if (money2 >= acter->getMoney() && mapState[player2.y][player2.x] == TILE_DIRT){
				if(player2.x > 7)
					acter->UpdatePath(mapDistance_Player2_Right, "Player2");
				else if(player2.x <= 7)
					acter->UpdatePath(mapDistance_Player2_Left, "Player2");
				else
					cout << "Path Determine Error\n";
				GroundGroup_Player2->AddNewObject(acter);
				money2 -= acter->getMoney();
				UpdateSelected(2,2);
			}
			else{
				if(money2 < acter->getMoney()) UIGroup->AddNewObject(new Warning(3,1400,100, 24));
				else if(mapState[player2.y][player2.x] != TILE_DIRT) UIGroup->AddNewObject(new Warning(4,1400,100, 24));
			}
		}
	}
	else cout << "error doing selecting\n";
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

std::vector<std::vector<int>> MainPlayScene::CalculateBFSDistance(Engine::Point distination) {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	std::queue<Engine::Point> que;
	// Push end point.
	// BFS from end point.
	que.push(distination);
	map[distination.y][distination.x] = 0;

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
	return map;
}

void MainPlayScene::GetTransmitData(){
	CharacterSelectScene* scene = dynamic_cast<CharacterSelectScene*>(Engine::GameEngine::GetInstance().GetScene("character-select"));
	for(int i = 0;i<5;i++) selected_1.push_back(scene->selected_1[i]);
	for(int i = 0;i<5;i++) selected_2.push_back(scene->selected_2[i]);

	for(int i = 0;i<scene->playerName_1.size();i++) playerName_1.push_back(scene->playerName_1[i]);
	for(int i = 0;i<scene->playerName_2.size();i++) playerName_2.push_back(scene->playerName_2[i]);
}

void MainPlayScene::UpdateSelected(int player, int pos){
	if(player == 1){
		selected_1.push_back(selected_1[pos]);
		deque<int>::iterator it = selected_1.begin() + pos;
		selected_1.erase(it);
		cout << "selected_1:";
		for(int i = 0;i<selected_1.size();i++) cout << " " << selected_1[i];
		cout << "\n";
	}
	else if(player == 2){
		selected_2.push_back(selected_2[pos]);
		deque<int>::iterator it = selected_2.begin() + pos;
		selected_2.erase(it);
		cout << "selected_2:";
		for(int i = 0;i<selected_2.size();i++) cout << " " << selected_2[i];
		cout << "\n";
	}
}
