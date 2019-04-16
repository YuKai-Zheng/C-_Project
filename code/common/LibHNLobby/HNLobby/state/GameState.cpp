#include "GameState.h"
#include "HNPlatform/HNGameCreator.h"
#include "../../GoldenToad/Classes/GameMessage/GoldenToadMessageHead.h"

void GameState::enter() {
}

void GameState::exit() {
}

void GameState::execute() {
}

bool GameState::init(HNGameUIBase* game) {
    if (!HNScene::init()) {
        return false;
    }

    addChild(game);

    if (HNGameCreator::getInstance()->getCurrentGameKindID() == GoldenToad::GAME_LIKUI_NAME_ID || HNGameCreator::getInstance()->getCurrentGameKindID() == GoldenToad::GAME_TOAD_NAME_ID) {
        unscheduleReleaseScene();
    }

    return true;
}

GameState* GameState::create(HNGameUIBase* game) {
    GameState *pRet = new(std::nothrow) GameState();
    if (pRet && pRet->init(game)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}