#include "GameMenuV1.h"
#include "../state/PlatformState.h"
#include "../pattern/StateManager.h"

bool GameMenuV1::init() {
    if (!GameMenuOne::init()) {
        return false;
    }
    return true;
}
void GameMenuV1::loginResult(bool isSuccess, const std::string &message) {
    GameMenuOne::loginResult(isSuccess, message,0);
    if (isSuccess) {
        PlatformState* state = PlatformState::create();
        state->setType(GAMELIST);
        StateManager::getInstance()->setState(state);
    }
}
