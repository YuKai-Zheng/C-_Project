#include "GameMenuV88.h"
#include "../globel.h"
#include "../state/PlatformState.h"
#include "../pattern/StateManager.h"
#include "HNLobby/GameHTTPRequestManager.h"


bool GameMenuV88::init() {
	if (!GameMenuOne::init()) {
		return false;
	}
	return true;
}

void GameMenuV88::loginResult(bool isSuccess, const std::string &message) {
	GameMenuOne::loginResult(isSuccess, message,0);
	if (isSuccess) {
        GameHTTPRequestManager::getInstance()->reqOpenFuncSocket();
		GameHTTPRequestManager::getInstance()->reqValidGames();
		GameHTTPRequestManager::getInstance()->setEnterLobbyCallback([](){
			PlatformState* state = PlatformState::create();
			state->setType(GAMELIST);
			StateManager::getInstance()->setState(state);
		});
	}
}
