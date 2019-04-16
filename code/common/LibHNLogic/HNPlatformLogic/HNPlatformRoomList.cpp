#include "HNPlatformLogic/HNPlatformRoomList.h"

namespace HN {
HNPlatformRoomList::HNPlatformRoomList(PlatformDelegate* delegate) :HNPlatformLogicBase(delegate) {
    CCAssert(delegate != nullptr, "delegate is null.");
    m_delegate = delegate;
    name = "HNPlatformRoomList";
}

HNPlatformRoomList::~HNPlatformRoomList() {
    m_delegate = nullptr;
}

void HNPlatformRoomList::start() {
    HNPlatformLogicBase::start();
    RoomLogic()->addRoomObserver(this);
}

void HNPlatformRoomList::stop() {
    HNPlatformLogicBase::stop();
    RoomLogic()->removeRoomObserver(this);
}

void HNPlatformRoomList::requestRoomList() {
    UINT gameNameID = GameCreator()->getCurrentGameNameID();
    UINT gameKindID = GameCreator()->getCurrentGameKindID();

    if (0 == gameKindID || -1 == gameKindID) {
        //_callback->onPlatformRoomListCallback(false, "invalid NameID or KindID.");
        m_delegate->updateRoomList(false, "invalid NameID or KindID.");
        return;
    }

    PlatformLogic()->requestRoomListInfo(gameKindID, gameNameID);
}

void HNPlatformRoomList::onHandleRoomListMessage() {
    //_callback->onPlatformRoomListCallback(true, "request success.");
    m_delegate->updateRoomList(true, "request success.");
}

void HNPlatformRoomList::onHandleRoomUserCountMessage(UINT roomID, UINT peopleCount, UINT virtualCount) {
    //_callback->onPlatformRoomUserCountCallback(roomID, peopleCount);
    m_delegate->updateRoomUserCount(roomID, peopleCount);
}
}