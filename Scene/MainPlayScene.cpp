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
using namespace std;
const int MainPlayScene::MapWidth = 15, MainPlayScene::MapHeight = 13;
const int MainPlayScene::BlockSize = 64;
void MainPlayScene::Initialize() {
	SpeedMult = 1;
	ticks = 0;
	money1 = 0, money2 = 0;
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
}

void MainPlayScene::Terminate() {
	IScene::Terminate();
}

void MainPlayScene::Update(float deltatime){
	cout << "Deltatime: " <<deltatime << "\n";
	IScene::Update(deltatime);
	ticks += deltatime;
	cout << "Ticks: "<<ticks << "\n";
	if(ticks >= 1.5){
		UpdateMoney();
		ticks = 0;
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
	cout << mapData.size() << "\n";
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