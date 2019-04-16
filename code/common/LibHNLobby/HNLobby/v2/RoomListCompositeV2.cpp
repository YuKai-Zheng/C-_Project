#include "RoomListCompositeV2.h"
#include "HNLobby/GameNotice.h"
#include "../pattern/CommandManager.h"


RoomListCompositeV2::RoomListCompositeV2() {
}

bool RoomListCompositeV2::init() {
    if (!Node::init()) {
        return false;
    }

    gameRoomLayer = GameRoom::create();
    addChild(gameRoomLayer, ROOM_LAYER_ZORDER, CHILD_LAYER_ROOM_TAG);
    gameRoomLayer->setPosition(Vec2(0.0f, 0.0f));
    gameRoomLayer->setName("gameRoomLayer");
    gameRoomLayer->setOpacity(0);
    gameRoomLayer->runAction(Sequence::create(FadeIn::create(0.3f), CallFunc::create([&]() {
        auto gameRoomLayer =dynamic_cast<GameRoom *>(this->getChildByName("gameRoomLayer"));
        UINT roomID = gameRoomLayer->getTimeMatchRoomID();
        if (roomID != 255) {
            gameRoomLayer->fastEnterTimeMatch(roomID);
        }
    }),nullptr));
    gameRoomLayer->onCloseCallBack = [this]() {
        CommandManager::getInstance()->addLoaderCommand("");
        CommandManager::getInstance()->execute("CheckGiftCommand");

        if (UserDefault::getInstance()->getIntegerForKey("PLATFORM_GAMETYPE_RETURN") == -1) {
            _roomListLogic->stop();
            platformDelegate->setLayerType(GAMELIST);
        } else {
            platformDelegate->setLayerType(GAMETYPE);
        }
    };

    gameRoomLayer->onEnterDeskCallBack = [this](ComRoomInfo *roomInfo) {
        if (platformDelegate) {
            platformDelegate->setLayerType(DESKLIST);
        }
    };

    gameRoomLayer->onRunActionCallBack = [this]() {
    };

    isShowBank = (gameRoomLayer != nullptr);

    return true;
}

void RoomListCompositeV2::execute() {
    topMenudelegate->setShowBank(isShowBank);
    //topMenudelegate->setType(room_type);
}

void RoomListCompositeV2::closeRoomList() {
    gameRoomLayer->closeRoomList();
}

void RoomListCompositeV2::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (gameRoomLayer) {
        gameRoomLayer->updateRoomPeopleCount(roomID, userCount);
    }
}
