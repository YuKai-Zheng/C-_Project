#include "RoomListComposite.h"
#include "HNLobby/GameNotice.h"
#include "../pattern/CommandManager.h"


RoomListComposite::RoomListComposite() {
}

bool RoomListComposite::init() {
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

        _roomListLogic->stop();

        if (platformDelegate) {
            platformDelegate->setLayerType(GAMELIST);
        }
    };

    gameRoomLayer->onEnterDeskCallBack = [this](ComRoomInfo *roomInfo) {
        _roomListLogic->stop();

        if (platformDelegate) {
            platformDelegate->setLayerType(DESKLIST);
        }
    };

    gameRoomLayer->onRunActionCallBack = [this]() {
    };

    isShowBank = (gameRoomLayer != nullptr);

    return true;
}

void RoomListComposite::execute() {
    topMenudelegate->setShowBank(isShowBank);
    //topMenudelegate->setType(room_type);
}

void RoomListComposite::closeRoomList() {
    gameRoomLayer->closeRoomList();
}

void RoomListComposite::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (gameRoomLayer) {
        gameRoomLayer->updateRoomPeopleCount(roomID, userCount);
    }
}
