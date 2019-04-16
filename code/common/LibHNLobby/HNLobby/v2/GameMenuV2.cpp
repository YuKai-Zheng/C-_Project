#include "GameMenuV2.h"
#include "../globel.h"
#include "../state/PlatformState.h"
#include "../pattern/StateManager.h"
#include "../model/one/GameMenuOne.h"


bool GameMenuV2::init() {
    if (!GameMenuOne::init()) {
        return false;
    }
    return true;
}

void GameMenuV2::loginResult(bool isSuccess, const std::string &message) {
    GameMenuOne::loginResult(isSuccess, message,0);
    if (isSuccess) {
        PlatformState* state = PlatformState::create();
        state->setType(GAMETYPE);
        StateManager::getInstance()->setState(state);
    }
}