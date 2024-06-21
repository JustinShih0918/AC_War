// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/CharacterSelectScene.hpp"
#include "Scene/ScoreBoard.hpp"
#include "Scene/MainPlayScene.hpp"
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO: [HACKATHON-1-SCENE] (3/4): Register Scenes here
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("scoreboard-scene", new ScoreBoard());
	game.AddNewScene("character-select",new CharacterSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("MainPlay", new MainPlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());

    // TODO: [HACKATHON-1-SCENE] (4/4): Change the start scene
	game.Start("start", 60, 1600, 832);
	// game.Start("settings", 60, 1600, 832);
	return 0;
}
