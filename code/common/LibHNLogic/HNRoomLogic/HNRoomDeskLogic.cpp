#include "HNRoomDeskLogic.h"

namespace HN {
void HNRoomDeskLogic::start() {
    RoomLogic()->addRoomObserver(this);
}

void HNRoomDeskLogic::stop() {
    RoomLogic()->removeRoomObserver(this);
}

void HNRoomDeskLogic::requestSit(BYTE deskNo) {
    ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
    if(nullptr == pRoomInfo) {
        _callback->onRoomSitCallback(false, "room info null.", _roomID, deskNo, INVALID_DESKSTATION);
        return;
    }

    std::vector<UserInfoStruct*> deskUsers;
    UserInfoModule()->findDeskUsers(deskNo, deskUsers);

    std::vector<bool> empty(pRoomInfo->uDeskPeople, true);

    // 查找坐下的用户座位号
    for (auto user : deskUsers) {
        if (USER_SITTING == user->bUserState		    // 坐下
                || USER_PLAY_GAME == user->bUserState		// 游戏中
                || USER_CUT_GAME == user->bUserState		// 断线
                || USER_ARGEE == user->bUserState) {		// 同意游戏
            empty[user->bDeskStation] = false;
        }
    }

    BYTE seatNo = INVALID_DESKSTATION;
    for(size_t i = 0; i < empty.size(); i++) {
        if(empty[i]) {
            seatNo = i;
            break;
        }
    }

    if(INVALID_DESKSTATION != seatNo) {
        requestSit(deskNo, seatNo);
    } else {
        _callback->onRoomSitCallback(false, "there is no empty seat.", _roomID, deskNo, seatNo);
    }
}

void HNRoomDeskLogic::requestSit(BYTE deskNo, BYTE seatNo) {
    MSG_GR_S_UserSit data;
    data.bDeskIndex   = deskNo;
    data.bDeskStation = seatNo;
    data.nClientFlag = 0;
    RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data ,sizeof(data));
}

HNRoomDeskLogic::HNRoomDeskLogic(IHNRoomDeskLogic* callback) {
    CCAssert(callback != nullptr, "callback is null.");
    _callback = callback;
    ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
    CCAssert(pRoomInfo != nullptr, "room info is null.");
    _roomID = pRoomInfo->uRoomID;
}

HNRoomDeskLogic::~HNRoomDeskLogic() {
    _callback = nullptr;
}

void HNRoomDeskLogic::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    if (nullptr == userSit) {
        return;
    }

    if(userSit->dwUserID != PlatformLogic()->loginResult.dwUserID) {
        _callback->onRoomDeskUserCountChanged(userSit);
        return;
    }

    // 断线重连进来
    else { // (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
        if (INVALID_DESKNO != userSit->bDeskIndex && INVALID_DESKSTATION != userSit->bDeskStation) {
            _callback->onRoomSitCallback(true, "sit success", _roomID, userSit->bDeskIndex, userSit->bDeskStation);
        } else {
            _callback->onRoomSitCallback(false, GBKToUtf8("坐下失败，请重试。"), _roomID, userSit->bDeskIndex, userSit->bDeskStation);

        }
    }
}

void HNRoomDeskLogic::onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) {
    _callback->onRoomDeskUserCountChanged(userUp);
}

void HNRoomDeskLogic::onDeskStateMessage(MSG_GR_DeskStation* pDate) {
	_callback->onDeskState(pDate);
}

void HNRoomDeskLogic::onDeskStateMessage(BYTE bDeskIndex) {
	_callback->onDeskState(bDeskIndex);
}

void HNRoomDeskLogic::onLockDeskMessage(BYTE deskNo) {
    _callback-> onRoomDeskLock(deskNo);
}

void HNRoomDeskLogic::onUnLockDeskMessage(BYTE deskNo) {
    _callback->onRoomDeskUnLock(deskNo);
}

void HNRoomDeskLogic::onUserComeMessage(UserInfoStruct* user) {
    _callback->onRoomUserCome(user->dwUserID);
}

void HNRoomDeskLogic::onUserLeftMessage(UserInfoStruct* user) {
    if(user){
        _callback->onRoomUserLeft(user->dwUserID);
    }
}
}
