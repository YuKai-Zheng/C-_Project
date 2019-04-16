#include "GameMenuV3.h"
#include "../globel.h"
#include "../state/PlatformState.h"
#include "../pattern/StateManager.h"


bool GameMenuV3::init() {
    if (!GameMenuOne::init()) {
        return false;
    }
    return true;
}

void GameMenuV3::loginResult(bool isSuccess, const std::string &message) {
    GameMenuOne::loginResult(isSuccess, message,0);
    if (isSuccess) {
        PlatformState* state = PlatformState::create();
        state->setType(GAMETYPE);
        StateManager::getInstance()->setState(state);
    }
}