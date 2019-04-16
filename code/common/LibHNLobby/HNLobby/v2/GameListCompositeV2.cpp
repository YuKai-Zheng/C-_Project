#include "GameListCompositeV2.h"
#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
USING_NS_CC;

GameListCompositeV2::GameListCompositeV2() {
}

bool GameListCompositeV2::init() {
    if (!Node::init()) {
        return false;
    }

    gamesLayer = GameLists::create();
    gamesLayer->setName("gamesLayer");
    gamesLayer->setPosition(Vec2(0.0f, 0.0f));
    gamesLayer->setOpacity(0);
    gamesLayer->runAction(FadeIn::create(0.3f));
    addChild(gamesLayer, GAMELIST_LAYER_ZORDER, CHILD_LAYER_GAMES_TAG);

    gamesLayer->onEnterGameCallBack = [this]() {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _gameListLogic->stop();
        platformDelegate->setLayerType(ROOMLIST);
        this->removeFromParent();
    };

    gamesLayer->onCloseCallBack = [this]() {
        platformDelegate->setLayerType(GAMETYPE);
        this->removeFromParent();
    };

    //起
    gamesLayer->onRunActionCallBack = [this]() {
    };

    return true;
}

void GameListCompositeV2::closeGameList() {
    gamesLayer->closeGameList();
}

void GameListCompositeV2::updateGameUserCount(UINT Id, UINT count) {
    if (gamesLayer) {
        gamesLayer->updateGamePeopleCount(Id, count);
    }

}
