#include "GameListCompositeV3.h"
#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"

USING_NS_CC;

GameListCompositeV3::GameListCompositeV3() {
}

bool GameListCompositeV3::init() {
    if (!Node::init()) {
        return false;
    }

    gamesLayer = GameListsV3::create();
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

void GameListCompositeV3::closeGameList() {
    gamesLayer->closeGameList();
}

void GameListCompositeV3::updateGameUserCount(UINT Id, UINT count) {
    if (gamesLayer) {
        gamesLayer->updateGamePeopleCount(Id, count);
    }

}
