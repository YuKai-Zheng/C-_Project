#include "RoomListCompositeV3.h"
#include "HNLobby/GameNotice.h"
#include "../pattern/CommandManager.h"


RoomListCompositeV3::RoomListCompositeV3() {
}

bool RoomListCompositeV3::init() {
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

        platformDelegate->setLayerType(GAMELIST);
        _roomListLogic->stop();
    };

    gameRoomLayer->onEnterDeskCallBack = [this](ComRoomInfo *roomInfo) {
        topMenudelegate->moveOut(true);
        bottomMenuDelegate->moveOut(true);

        if (platformDelegate) {
            platformDelegate->createDeskListLayer(roomInfo);
        }

        if (GameNotice::getNotic(this) != nullptr) {
            GameNotice::getNotic(this)->actionOut(0.0f);
        }
    };

    gameRoomLayer->onRunActionCallBack = [this]() {
    };

    isShowBank = (gameRoomLayer != nullptr);

    return true;
}

void RoomListCompositeV3::execute() {
    topMenudelegate->setShowBank(isShowBank);
    topMenudelegate->setType(room_type);
}

void RoomListCompositeV3::closeRoomList() {
    gameRoomLayer->closeRoomList();
}

void RoomListCompositeV3::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (gameRoomLayer) {
        gameRoomLayer->updateRoomPeopleCount(roomID, userCount);
    }
}
