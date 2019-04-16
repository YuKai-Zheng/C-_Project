#include "StateManager.h"

enum STATE_TYPE {
    //初始状态
    INIT_STATE,
    //登录状态
    LOGIN_STATE,
    //大厅状态
    PLATFORM_STATE,
    //房间状态
    ROOM_STATE,
    //桌子状态
    DESK_STATE
};

StateManager::StateManager() {
}

void StateManager::start(BaseState* state) {
    curState = state;
    state->enter();
    state->execute();
    Director::getInstance()->runWithScene(state);
}

void StateManager::setState(BaseState* state,bool isAnimated) {
    if (curState) {
        curState->exit();
    }

    curState = state;
    if (state) {
        state->enter();
        state->execute();

        if (isAnimated) {
            Director::getInstance()->replaceScene(TransitionCrossFade::create(0.3f, state));
        } else {
            Director::getInstance()->replaceScene(state);
        }
    }
}

//在此处初始化
StateManager* StateManager::manager = new StateManager();
StateManager* StateManager::getInstance() {
    return manager;
}
