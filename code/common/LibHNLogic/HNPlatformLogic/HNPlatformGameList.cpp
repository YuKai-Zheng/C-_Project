#include "HNPlatformLogic/HNPlatformGameList.h"

namespace HN {
void HNPlatformGameList::requestGameList() {
    platformRequestGameList();
}

void HNPlatformGameList::onHandleGameListMessage() {
    //_callback->onPlatformGameListCallback(true, "request game list success.");
    m_delegate->updateGameList(true, "request game list success.");
}

void HNPlatformGameList::onHandleGameUserCountMessage(DL_GP_RoomListPeoCountStruct* userCount) {
    UINT count = userCount->uOnLineCount + userCount->uVirtualUser;
    //_callback->onPlatformGameUserCountCallback(userCount->uID, count);
    m_delegate->updateGameUserCount(userCount->uID, count);
}

void HNPlatformGameList::start() {
    HNPlatformLogicBase::start();
}

void HNPlatformGameList::stop() {
    HNPlatformLogicBase::stop();
}

//void HNPlatformGameList::onPlatformGameListCallback(bool success, const std::string& message) {
//    m_delegate->updateGameList(success,message);
//}

HNPlatformGameList::HNPlatformGameList(PlatformDelegate* delegate):HNPlatformLogicBase(delegate) {
    CCAssert(delegate != nullptr, "delegate is null.");
    m_delegate = delegate;
    name = "HNPlatformGameList";
}

HNPlatformGameList::~HNPlatformGameList() {
    m_delegate = nullptr;
}

void HNPlatformGameList::platformRequestGameList() {
    PlatformLogic()->sendData(MDM_GP_LIST, ASS_GP_LIST_KIND);
}
}