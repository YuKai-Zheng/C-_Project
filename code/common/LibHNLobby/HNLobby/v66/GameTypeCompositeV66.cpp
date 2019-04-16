#include "GameTypeCompositeV66.h"

#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
#include "../PlatformDefine.h"
//#include "../v99/PlatformResV99.h"
#include "../GameType.h"
#include "HNLobby/FontConfig.h"

USING_NS_CC;

bool GameTypeCompositeV66::init() {
    if (!Node::init()) {
        return false;
    }
    createTypeListLayer();
    return true;
}

//创建类型主列表
void GameTypeCompositeV66::createTypeListLayer() {
    auto gameTypeLayer = GameType::create();
    gameTypeLayer->setName("gameTypeLayer");
    gameTypeLayer->setPosition(Vec2(0.0f, 0.0f));
    gameTypeLayer->setOpacity(0);
    gameTypeLayer->runAction(FadeIn::create(0.3f));
    addChild(gameTypeLayer, 102, 103);

    gameTypeLayer->onEnterGameCallBack = [this]() {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN", -1);
        platformDelegate->setLayerType(GAMELIST);
        this->removeFromParent();
    };

    gameTypeLayer->quickEnterGameCallBack = [this]() {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN", 0);
        platformDelegate->setLayerType(ROOMLIST);
        this->removeFromParent();
    };

    gameTypeLayer->onCloseCallBack = [this]() {
    };
}
