#include "HNGameLogicBase.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNNetExport.h"
#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/globel.h"
#include "base/ChangeRootViewController.h"
#include "base/WeChatAndroid.h"

namespace HN {
void HNGameLogicBase::start() {
    RoomLogic()->addObserver(this);
    PlatformLogic()->addObserver(this);
}

void HNGameLogicBase::stop() {
    RoomLogic()->removeObserver(this);
    PlatformLogic()->removeObserver(this);
}

HNGameLogicBase::HNGameLogicBase(BYTE deskNo, INT maxPlayers, bool autoCreate,
                                 IHNGameLogicBase *callback)
    : _deskNo(deskNo), _maxPlayers(maxPlayers), _autoCreate(autoCreate) {
    CCAssert(callback != nullptr, "callback is null.");

    _callback = callback;
    initParams();

    _deskUserList = new HNGameTableUsersData(deskNo);
    loadDeskUsers();
    start();
}

HNGameLogicBase::~HNGameLogicBase() {
    stop();
    HN_SAFE_DELETE(_deskUserList);
}

//////////////////////////////////////////////////////////////////////////
void HNGameLogicBase::onDisConnect() {
    auto prompt = GamePromptLayer::create();
    prompt->setColorLayerTouchEnabled(false);
    prompt->showPrompt(GetText("NET_DISCONNECT"));
    prompt->setOkayCallBack([this]() {
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
        
        //竖屏游戏网络断开后需要切换到横屏
        if (UserDefault::getInstance()->getBoolForKey(ROOT_VIEW_IS_VERTICAL)) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ChangeRootViewController::changeRootViewControllerH();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            WeChatAndroid::getInstance()->changeScreenDirection(0);
#endif
        }
        
        PlatformConfig::getInstance()->setFullClose(false);
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
    });
}

void HNGameLogicBase::onUserAgreeMessage(MSG_GR_R_UserAgree *agree) {
    if (agree->bDeskNO != _deskNo)
        return;

    dealUserAgreeResp(agree);
}

void HNGameLogicBase::onGameStartMessage(BYTE bDeskNO) {
    if (bDeskNO != _deskNo)
        return;

    dealGameStartResp(bDeskNO);
}

void HNGameLogicBase::onGameEndMessage(BYTE bDeskNO) {
    if (bDeskNO != _deskNo)
        return;

    dealGameEndResp(bDeskNO);

    if ((RoomLogic()->getRoomRule() & GRR_CONTEST) ||
            (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)) {
        _deskUserList->clear();
    }
}

void HNGameLogicBase::onUserSitMessage(MSG_GR_R_UserSit *userSit,
                                       UserInfoStruct *user) {
    if (nullptr == userSit)
        return;
    if (nullptr == user)
        return;

	if (userSit->bDeskIndex != _deskNo && userSit->dwUserID != PlatformLogic()->loginResult.dwUserID)
		return;

    if (userSit->bUserState != USER_WATCH_GAME) {
        if (_maxPlayers > 0) {
            _existPlayer[userSit->bDeskStation] = true;
        }

        if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            _deskNo = userSit->bDeskIndex;
            _mySeatNo = userSit->bDeskStation;
        }
        dealUserSitResp(userSit, user);
    }
}

// 排队用户坐下
void HNGameLogicBase::onQueueUserSitMessage(
    BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT *> &users) {
    BYTE mySeatNo = INVALID_DESKNO;

    for (auto iter = users.begin(); iter != users.end(); ++iter) {
        if ((*iter)->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            mySeatNo = (*iter)->bDeskStation;
            break;
        }
    }

    if (INVALID_DESKNO == mySeatNo)
        return;

    _deskNo = deskNo;
    _mySeatNo = mySeatNo;

    _seatOffset = -_mySeatNo;

    _deskUserList->update(_deskNo);
    dealQueueUserSitMessage(deskNo, users);
}

void HNGameLogicBase::onUserUpMessage(MSG_GR_R_UserSit *userSit,
                                      UserInfoStruct *user) {
    if (userSit->bDeskIndex != _deskNo)
        return;
    if (userSit->bUserState != USER_WATCH_GAME) {
        if (_maxPlayers > 0) {
            _existPlayer[userSit->bDeskStation] = false;
        }

        if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            _mySeatNo = INVALID_DESKSTATION;
            _deskNo = INVALID_DESKNO;
        }

        dealUserUpResp(userSit, user);
    }
}

void HNGameLogicBase::onGameInfoMessage(MSG_GM_S_GameInfo *pGameInfo) {
    _gameStatus = pGameInfo->bGameStation;
    _waitTime = pGameInfo->bWaitTime;
    _watchOther = pGameInfo->bWatchOther;

    dealGameInfoResp(pGameInfo);
}

void HNGameLogicBase::onGameStationMessage(void *object, INT objectSize) {
    dealGameStationResp(object, objectSize);
}

bool HNGameLogicBase::onGameMessage(NetMessageHead *messageHead, void *object,
                                    INT objectSize) {
    dealGameMessage(messageHead, object, objectSize);
    return true;
}

void HNGameLogicBase::onUserCutMessage(INT dwUserID, BYTE bDeskNO,
                                       BYTE bDeskStation) {
    if (bDeskNO != _deskNo)
        return;

    dealUserCutMessageResp(dwUserID, bDeskStation);
}

// 结算消息
void HNGameLogicBase::onGamePointMessage(void *object, INT objectSize) {
    dealGamePointResp(object, objectSize);
}

//////////////////////////////////////////////////////////////////////////
// 比赛初始化
void HNGameLogicBase::onGameContestInit(
    NET_ROOM_CONTEST_CHANGE_RESULT *contestChange) {
    dealGameContestInit(contestChange);
}

//比赛信息广播
void HNGameLogicBase::onGameContestNotic(MSG_GR_I_ContestInfo *contestInfo) {
    dealGameContestNotic(contestInfo);
}

// 用户比赛信息
void HNGameLogicBase::onGameUserContset(MSG_GR_ContestChange *contestChange) {
    dealGameUserContset(contestChange);
}

// 比赛结束
void HNGameLogicBase::onGameContestOver(MSG_GR_ContestAward *contestAward) {
    dealGameContestOver(contestAward);
}

// 比赛淘汰
void HNGameLogicBase::onGameContestKick() {
    dealGameContestKick();
}

// 等待比赛结束
void HNGameLogicBase::onGameContestWaitOver() {
    dealGameContestWaitOver();
}

//////////////////////////////////////////////////////////////////////////
void HNGameLogicBase::onUserChatMessage(void *object, INT objectSize) {
    MSG_GR_RS_NormalTalk *pData = (MSG_GR_RS_NormalTalk *)object;
    dealUserChatMessage(pData);
}

//////////////////////////////////////////////////////////////////////////
void HNGameLogicBase::dealUserAgreeResp(MSG_GR_R_UserAgree *agree) {
    HNLOG("dealUserAgreeResp");
}

void HNGameLogicBase::dealGameStartResp(BYTE bDeskNO) {
    HNLOG("dealGameStartResp");
}

void HNGameLogicBase::dealGameEndResp(BYTE bDeskNO) {
    HNLOG("dealGameEndResp");
}

void HNGameLogicBase::dealUserSitResp(MSG_GR_R_UserSit *userSit,
                                      UserInfoStruct *user) {
    HNLOG("dealUserSitResp");
}

void HNGameLogicBase::dealQueueUserSitMessage(
    BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT *> &users) {
    HNLOG("dealQueueUserSitMessage");
}

void HNGameLogicBase::dealUserUpResp(MSG_GR_R_UserSit *userSit,
                                     UserInfoStruct *user) {
    HNLOG("dealUserUpResp");
}

void HNGameLogicBase::dealGameInfoResp(MSG_GM_S_GameInfo *pGameInfo) {
    HNLOG("dealGameInfoResp");
}

void HNGameLogicBase::dealUserCutMessageResp(INT userId, BYTE seatNo) {
    HNLOG("dealUserCutMessageResp");
}

void HNGameLogicBase::dealGamePointResp(void *object, INT objectSize) {
    HNLOG("dealGamePointResp");
}

void HNGameLogicBase::dealUserChatMessage(MSG_GR_RS_NormalTalk *normalTalk) {
    HNLOG("dealUserChatMessage");
}

//////////////////////////////////////////////////////////////////////////
// 比赛接口
//////////////////////////////////////////////////////////////////////////
// 比赛初始化
void HNGameLogicBase::dealGameContestInit(
    NET_ROOM_CONTEST_CHANGE_RESULT *contestChange) {
    HNLOG("dealGameContestInit");
    CCAssert(false, "subclasses must override");
}

void HNGameLogicBase::dealGameContestNotic(MSG_GR_I_ContestInfo *contestInfo) {
    HNLOG("dealGameContestNotic");
    CCAssert(false, "subclasses must override");
}

// 用户比赛信息
void HNGameLogicBase::dealGameUserContset(MSG_GR_ContestChange *contestChange) {
    HNLOG("dealGameUserContset");
    CCAssert(false, "subclasses must override");
}

// 比赛结束
void HNGameLogicBase::dealGameContestOver(MSG_GR_ContestAward *contestAward) {
    HNLOG("dealGameContestOver");
    CCAssert(false, "subclasses must override");
}

// 比赛淘汰
void HNGameLogicBase::dealGameContestKick() {
    HNLOG("dealGameContestKick");
    CCAssert(false, "subclasses must override");
}

// 等待比赛结束
void HNGameLogicBase::dealGameContestWaitOver() {
    HNLOG("dealGameContestWaitOver");
    CCAssert(false, "subclasses must override");
}

// 比赛奖励
void HNGameLogicBase::dealGameContestAwards(
    NET_ROOM_CONTEST_AWARD_RESULT *contestAward) {
    HNLOG("dealGameContestAwards");
}

// 报名数量
void HNGameLogicBase::dealGameContestPeople(
    NET_ROOM_CONTEST_PEOPLE_RESULT *contestPeople) {
    HNLOG("dealGameContestPeople");
}

// 个人参赛纪录
void HNGameLogicBase::dealGameContestRecord(
    NET_ROOM_CONTEST_RECORD_RESULT *ContestRecord) {
    HNLOG("dealGameContestRecord");
}

void HNGameLogicBase::sendGameInfo() {
    HNLOG("sendGameInfo");
    MSG_GM_S_ClientInfo ClientInfo;
    ClientInfo.bEnableWatch = 0;
    RoomLogic()->sendData(MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, &ClientInfo,
                          sizeof(ClientInfo));
}

void HNGameLogicBase::sendUserUp() {
    RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
}

void HNGameLogicBase::sendUserSit(BYTE lSeatNo) {
    if (INVALID_DESKSTATION == lSeatNo) {
        return;
    }

    UserInfoStruct *userInfo = _deskUserList->getUserByDeskStation(lSeatNo);
    if (nullptr != userInfo) {
        BYTE tmpSeatNo = lSeatNo;
        do {
            tmpSeatNo = (tmpSeatNo + 1) % _maxPlayers;
            if (_deskUserList->getUserByDeskStation(lSeatNo) == nullptr) {
                break;
            }
        } while (tmpSeatNo != lSeatNo);
        if (tmpSeatNo == lSeatNo) {
            HNLOG("no empty sit.");
            return;
        } else {
            lSeatNo = tmpSeatNo;
        }
    }

    MSG_GR_S_UserSit data;
    data.bDeskIndex = _deskNo;
    data.bDeskStation = lSeatNo;
    data.nClientFlag = 0;
    RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data,
                          sizeof(data));
}

void HNGameLogicBase::sendForceQuit() {
    RoomLogic()->sendData(MDM_GM_GAME_FRAME, ASS_GM_FORCE_QUIT);
}

void HNGameLogicBase::sendAgreeGame() {
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME);
}

void HNGameLogicBase::sendChatMsg(const std::string &msg) {
    MSG_GR_RS_NormalTalk data;
    data.dwSendID = RoomLogic()->loginResult.pUserInfoStruct.dwUserID;
    data.iLength = msg.size();
    sprintf(data.szMessage, "%s", Utf8ToGB(msg.c_str()));
    RoomLogic()->sendData(MDM_GM_GAME_FRAME, ASS_GM_NORMAL_TALK, (void *)&data,
                          sizeof(MSG_GR_RS_NormalTalk));
}

//////////////////////////////////////////////////////////////////////////

void HNGameLogicBase::loadDeskUsers() {
    BYTE seatNo = INVALID_DESKNO;
    for (int i = 0; i < _maxPlayers; i++) {
        UserInfoStruct *pUser = _deskUserList->getUserByDeskStation(i);
        if (nullptr != pUser) {
            if (pUser->dwUserID ==
                    RoomLogic()->loginResult.pUserInfoStruct.dwUserID) {
                _mySeatNo = i;
                _seatOffset = -_mySeatNo;
            }
            if (_maxPlayers > 0) {
                _existPlayer[i] = true;
            }
        }
    }
    if (-1 == _maxPlayers) {
        UserInfoStruct *pUser = _deskUserList->getUserByUserID(
                                    RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
        if (nullptr != pUser) {
            _mySeatNo = pUser->bDeskStation;
        }
    }
}

void HNGameLogicBase::clearDesk() {}

//////////////////////////////////////////////////////////////////////////
INT HNGameLogicBase::getUserId(BYTE lSeatNo) {
    if (INVALID_DESKSTATION == lSeatNo) {
        return INVALID_USER_ID;
    }

    UserInfoStruct *userInfo = _deskUserList->getUserByDeskStation(lSeatNo);
    return (nullptr != userInfo) ? userInfo->dwUserID : INVALID_USER_ID;
}

UserInfoStruct *HNGameLogicBase::getUserByUserID(INT userID) {
    return _deskUserList->getUserByUserID(userID);
}

UserInfoStruct *HNGameLogicBase::getUserBySeatNo(BYTE lSeatNo) {
    return _deskUserList->getUserByDeskStation(lSeatNo);
}

bool HNGameLogicBase::getUserIsBoy(BYTE lSeatNo) {
    UserInfoStruct *userInfo = _deskUserList->getUserByDeskStation(lSeatNo);
    return (nullptr != userInfo) ? userInfo->bBoy : true;
}

BYTE HNGameLogicBase::getMySeatNo() const {
    return _mySeatNo;
}
//////////////////////////////////////////////////////////////////////////

void HNGameLogicBase::initParams() {
    _gameStatus = 0;
    _mySeatNo = INVALID_DESKSTATION;
    _waitTime = 0;
    _seatOffset = 0;

    if (_maxPlayers > 0) {
        _existPlayer.resize(_maxPlayers, false);
        _playing.resize(_maxPlayers, false);
    }
}

void HNGameLogicBase::refreshParams() {}

BYTE HNGameLogicBase::getNextUserSeatNo(BYTE lSeatNo) {
    if (_maxPlayers <= 0) {
        return INVALID_DESKSTATION;
    }

    BYTE nextSeatNo = lSeatNo;
    int count = 0;
    while (count < _maxPlayers) {
        nextSeatNo = (nextSeatNo + 1 + _maxPlayers) % _maxPlayers;
        UserInfoStruct *pUser = _deskUserList->getUserByDeskStation(nextSeatNo);
        if (pUser != nullptr && _playing[nextSeatNo]) {
            return nextSeatNo;
        }
        count++;
    }
    return INVALID_DESKSTATION;
}

BYTE HNGameLogicBase::getLastUserSeatNo(BYTE lSeatNo) {
    if (_maxPlayers <= 0) {
        return INVALID_DESKSTATION;
    }

    BYTE lastSeatNo = lSeatNo;
    int count = 0;
    while (count < _maxPlayers) {
        lastSeatNo = (lastSeatNo - 1 + _maxPlayers) % _maxPlayers;
        UserInfoStruct *pUser = _deskUserList->getUserByDeskStation(lastSeatNo);
        if (pUser != nullptr && _playing[lastSeatNo]) {
            return lastSeatNo;
        }
        count++;
    }
    return INVALID_DESKSTATION;
}

BYTE HNGameLogicBase::viewToLogicSeatNo(BYTE vSeatNO) {
    return (vSeatNO - _seatOffset + _maxPlayers) % _maxPlayers;
}

BYTE HNGameLogicBase::logicToViewSeatNo(BYTE lSeatNO) {
    return (lSeatNO + _seatOffset + _maxPlayers) % _maxPlayers;
}

void HNGameLogicBase::onNewsMessage(MSG_GR_RS_NormalTalk *pData) {
	//数量负数
	if (pData->dwTargetID < 0)
	{
		pData->dwTargetID = -pData->dwTargetID;
	}
	_callback->onHandleNewMessage(pData->dwSendID, pData->szMessage, pData->dwTargetID);
}
}
