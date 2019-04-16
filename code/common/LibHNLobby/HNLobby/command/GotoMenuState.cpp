#include "GotoMenuState.h"
#include "../pattern/StateManager.h"
#include "../state/MenuState.h"

void GotoMenuState::execute() {
    StateManager::getInstance()->setState(MenuState::create(),false);
}
