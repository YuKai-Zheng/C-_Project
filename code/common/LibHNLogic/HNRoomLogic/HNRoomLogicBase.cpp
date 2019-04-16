#include "HNRoomLogic/HNRoomLogicBase.h"
#include "HNNetExport.h"
#include "../../LibHNLobby/HNLobby/GamePrompt.h"
#include "HNLobby/pattern/CommandManager.h"

namespace HN {
void HNRoomLogicBase::start() {
    RoomLogic()->addRoomObserver(this);
}

void HNRoomLogicBase::stop() {
    RoomLogic()->removeRoomObserver(this);
}

void HNRoomLogicBase::requestLogin(UINT roomID) {
    ComRoomInfo* pRoomInfo = RoomInfoModule()->getByRoomID(roomID);
    if(nullptr == pRoomInfo) {
        _callback->onRoomLoginCallback(false, "invalid room id", _roomID);
    }
	else 
	{
        if (PlatformLogic()->loginResult.i64Money < pRoomInfo->iLessPoint) {
            //char buffer[128];
            //string str = StringUtils::format(GetText("LACK_OF_MONEY"), pRoomInfo->iLessPoint);
            string str("");
			//str.append(GetText("LACK_OF_MONEY1"));
			//str.append(GetText("LACK_OF_MONEY2"));
			//str.append(StringUtils::format("%u", pRoomInfo->iLessPoint));
   //         //str.append(StringUtils::format("金币不足,进入该场次至少需携带\n%u金币", pRoomInfo->iLessPoint));
			//str.append(GetText("LACK_OF_MONEY3"));
			//str.append(StringUtils::format("At least %u coins are required to\n enter the room", pRoomInfo->iLessPoint));
			
			str.append(GetText("LACK_OF_MONEY1"));
			str.append("\n");
			str.append(GetText("LACK_OF_MONEY2"));
			str.append(StringUtils::format("%u", pRoomInfo->iLessPoint));
			//str.append(StringUtils::format("金币不足,进入该场次至少需携带\n%u金币", pRoomInfo->iLessPoint));
			str.append(GetText("LACK_OF_MONEY3"));
            //<string name = "LACK_OF_MONEY">您的金币小于%u, 不能进入房间, 请先充值或取款。< / string>

            _callback->onRoomLoginCallback(false, str, _roomID);
        } 
		else if(pRoomInfo->iMaxPoint != 0 && (PlatformLogic()->loginResult.i64Money > pRoomInfo->iMaxPoint))
		{
            char buffer[128];
            //string str = StringUtils::format(GetText("MONEY_MORE_THAN"), pRoomInfo->iMaxPoint);
            string str;
            str.append(GetText("MONEY_MORE_THAN1"));
            str.append(StringUtils::format("%u", pRoomInfo->iMaxPoint));
            str.append(GetText("MONEY_MORE_THAN2"));
            _callback->onRoomLoginCallback(false, str, _roomID);
        } 
		else 
		{
            RoomLogic()->setSelectedRoom(pRoomInfo);
            RoomLogic()->setRoomRule(pRoomInfo->dwRoomRule);
            _roomID = roomID;

            if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) 
			{
                // 排队机
                _callback->onRoomLoginCallback(false, GetText("DEV_WAITING_MODE"), _roomID);
            } 
			else {
                // 金币场不扣积分
                // 连接房间服务器
                UserInfoModule()->clear();
                RoomLogic()->connectRoom(pRoomInfo->szServiceIP, pRoomInfo->uServicePort);
            }
        }
    }
}

void HNRoomLogicBase::requestSit(BYTE deskNo) {

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

void HNRoomLogicBase::requestSit(BYTE deskNo, BYTE seatNo) {
    MSG_GR_S_UserSit data;
    data.bDeskIndex   = deskNo;
    data.bDeskStation = seatNo;
    data.nClientFlag = 0;
    RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data ,sizeof(data));
}

void HNRoomLogicBase::requestJoinQueue() {
    RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
}

void HNRoomLogicBase::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    if (nullptr == userSit ||  nullptr == user) {
        if(userSit == nullptr) {
            _callback->onRoomSitCallback(false, GBKToUtf8("坐下未知错误"), _roomID, INVALID_DESKNO, INVALID_DESKSTATION);
            return;
        }
        if(userSit->dwUserID != PlatformLogic()->loginResult.dwUserID) {
            return;
        }
    }

    // 断线重连进来
    if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID) {
        if (INVALID_DESKNO != userSit->bDeskIndex && INVALID_DESKSTATION != userSit->bDeskStation) {
            _callback->onRoomSitCallback(true, "sit success", _roomID, userSit->bDeskIndex, userSit->bDeskStation);
        }
    }
}

void HNRoomLogicBase::onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users) {
    if(INVALID_DESKNO == deskNo) {
        _callback->onRoomQueueSitCallback(false, "invalid deskNo", _roomID);
        return;
    }

    bool find = false;
    for (auto iter = users.begin(); iter != users.end(); ++iter) {
        if((*iter)->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            find = true;
            break;
        }
    }
    if (find) {
        _callback->onRoomQueueSitCallback(true, "queue sit success", _roomID);
    } else {
        _callback->onRoomQueueSitCallback(false,"not find login user.", _roomID);
    }
}

void HNRoomLogicBase::onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) {
    if(INVALID_DESKNO == userUp->bDeskIndex) {
        _callback->onRoomSitUpCallback(false, "sitUp faile", userUp->bDeskIndex, userUp->bDeskStation);
    } else {
        _callback->onRoomSitUpCallback(true, "sitUp success", userUp->bDeskIndex, userUp->bDeskStation);
    }
}

void HNRoomLogicBase::onDisConnect() {
    auto prompt = GamePromptLayer::create();
    prompt->showPrompt(GetText("NET_DISCONNECT"));
    prompt->setColorLayerTouchEnabled(false);
    prompt->setOkayCallBack([this]() {
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
    });
}

void HNRoomLogicBase::onConnectMessage(bool result) {
    if(!result) {
        _callback->onRoomLoginCallback(false, "网络连接已断开,请重新登陆", _roomID);
    } else {
        ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
        if(pRoomInfo == nullptr) {
            _callback->onRoomLoginCallback(false, "room info is null.", _roomID);
        } else {
            RoomLogic()->roomLogin(pRoomInfo->uNameID);
        }
    }
}

void HNRoomLogicBase::onLoginMessage(bool result, UINT dwErrorCode) {
    if(!result) {
        char message[100] = {0};
        switch (dwErrorCode) {
        case ERR_GR_USER_PASS_ERROR: {
            sprintf(message, "用户密码错误。(%d)", ERR_GR_USER_PASS_ERROR);
        }
        break;
        case ERR_GR_CONTEST_TIMEOUT: {
            sprintf(message, "连接超时。(%d)", ERR_GR_CONTEST_TIMEOUT);
        }
        break;
        case ERR_GR_IN_OTHER_ROOM: {
            sprintf(message, "正在其他房间。(%d)", ERR_GR_IN_OTHER_ROOM);
        }
        break;
        case ERR_GR_ACCOUNTS_IN_USE: {
            sprintf(message, "帐号正在使用。(%d)", ERR_GR_ACCOUNTS_IN_USE);
        }
        break;
        case ERR_GR_STOP_LOGON: {
            sprintf(message, "网络不畅，请重新登陆。(%d)", ERR_GR_STOP_LOGON);
        }
        break;
        case ERR_GR_PEOPLE_FULL: {
            sprintf(message, "房间人数已经满。(%d)", ERR_GR_PEOPLE_FULL);
        }
        break;
        default:
            break;
        }
        _callback->onRoomLoginCallback(false, message, _roomID);
    } else {
        ;
    }
}

void HNRoomLogicBase::onLoginFinishMessage() {
    UserInfoStruct info = RoomLogic()->loginResult.pUserInfoStruct;

	auto roomId = UserDefault::getInstance()->getIntegerForKey("QUICK_JOIN_ROOM_ID");
	auto gameId = UserDefault::getInstance()->getIntegerForKey("Enter_Game_ID");
	if (gameId == 11118888)
		_roomID = roomId;


    if (INVALID_DESKNO != info.bDeskNO && INVALID_DESKSTATION != info.bDeskStation) {
        _callback->onRoomSitCallback(true, "sit success", _roomID, info.bDeskNO, info.bDeskStation);
    } else {
        _callback->onRoomLoginCallback(true, "connect success", _roomID);
    }
}

void HNRoomLogicBase::onUserComeMessage(UserInfoStruct* user) {
    _callback->onRoomUserCome(user->dwUserID);
}

void HNRoomLogicBase::onUserLeftMessage(UserInfoStruct* user) {
    if (user == nullptr) {
        return;
    }
    _callback->onRoomUserLeft(user->dwUserID);
}

bool HNRoomLogicBase::onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
    return true;
}

void HNRoomLogicBase::onGamePointMessage(void * object, INT objectSize) {

}

HNRoomLogicBase::HNRoomLogicBase(IHNRoomLogicBase* callback) {
    CCAssert(callback != nullptr, "callback is null.");
    _callback = callback;
}

HNRoomLogicBase::~HNRoomLogicBase() {
    _callback = nullptr;
}
}
