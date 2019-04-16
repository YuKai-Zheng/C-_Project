#include "GameTypeCompositeV3.h"
#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
#include "PlatformResV3.h"
#include "../PlatformDefine.h"
#include "../GameType.h"
#include "../GameTypeV3.h"
USING_NS_CC;

#define fish_type_tag "fishType"
#define nn_type_tag "nnType"
#define happy_type_tag "happyType"
#define multi_type_tag "multiType"

GameTypeCompositeV3::GameTypeCompositeV3() {
}

bool GameTypeCompositeV3::init() {
    if (!Node::init()) {
        return false;
    }

    createTypeListLayer();
    return true;
}

//创建类型主列表
void GameTypeCompositeV3::createTypeListLayer() {
    auto gameTypeLayer = GameTypeV3::create();
    gameTypeLayer->setName("gameTypeLayer");
    gameTypeLayer->setPosition(Vec2(0.0f, 0.0f));
    gameTypeLayer->setOpacity(0);
    gameTypeLayer->runAction(FadeIn::create(0.3f));
    addChild(gameTypeLayer, 102, 103);

    gameTypeLayer->onEnterGameCallBack = [&]() {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN", -1);
        platformDelegate->setLayerType(GAMELIST);
        this->removeFromParent();
    };

    gameTypeLayer->quickEnterGameCallBack = [this]() {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        platformDelegate->setLayerType(ROOMLIST);
        UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN", 0);
    };

    gameTypeLayer->onCloseCallBack = [this]() {
    };
}