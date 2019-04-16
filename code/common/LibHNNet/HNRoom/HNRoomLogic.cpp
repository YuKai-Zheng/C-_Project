#include "HNRoomLogic.h"
#include "HNCommon/HNConverCode.h"
#include "HNData/HNRoomInfoModule.h"
#include "HNData/HNUserInfoModule.h"
#include "HNGame/HNGameMessageDelegate.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNPlatform/HNPlatformExport.h"
#include "HNRoomMessageDelegate.h"
#include "HNSocket/HNSocketLogic.h"
#include "HNSocket/HNSocketProtocolData.h"
#include "HNLobby/pattern/CommandManager.h"

namespace HN {
static HNRoomLogic *sHNRoomLogic = nullptr;

#define MAX_HEARTBEAT_COUNT 2
#define HEARTBEAT_INTERVAL  5.0f
#define RECONNECT_TIME      10.0f

HNRoomLogic *HNRoomLogic::getInstance()
{
    if (nullptr == sHNRoomLogic) {
        sHNRoomLogic = new HNRoomLogic();
        sHNRoomLogic->init();
    }
    return sHNRoomLogic;
}

HNRoomLogic::HNRoomLogic(void)
    : _gameRoomRule(0)
	,_serverPort(-1)
	,_roomConnected(false)
	,_roomLogin(false)
	,_selectedRoom(nullptr)
	,isStartCheck(false)
	,isNeedReLogin(false)
	,_reconnect(false) 
{
    memset(&loginResult, 0x00, sizeof(loginResult));
    _socketLogic = new HNSocketLogic(this, "game");

    _gameNotifyQueue = new HNUIDelegateQueue<IGameMessageDelegate *>();
    _addGameNotifyQueue = new HNUIDelegateQueue<IGameMessageDelegate *>();
    _removeGameNotifyQueue = new HNUIDelegateQueue<IGameMessageDelegate *>();

    _roomNotifyQueue = new HNUIDelegateQueue<IRoomMessageDelegate *>();
    _addRoomNotifyQueue = new HNUIDelegateQueue<IRoomMessageDelegate *>();
    _removeRoomNotifyQueue = new HNUIDelegateQueue<IRoomMessageDelegate *>();
}

HNRoomLogic::~HNRoomLogic(void)
{
    HN_SAFE_DELETE(_addGameNotifyQueue);
    HN_SAFE_DELETE(_gameNotifyQueue);
    HN_SAFE_DELETE(_removeGameNotifyQueue);
    HN_SAFE_DELETE(_roomNotifyQueue);
    HN_SAFE_DELETE(_addRoomNotifyQueue);
    HN_SAFE_DELETE(_removeRoomNotifyQueue);
    HN_SAFE_DELETE(_socketLogic);
    stopHeartBeat();
}

bool HNRoomLogic::init()
{
    return true;
}

bool HNRoomLogic::connectRoom(const CHAR *ip, INT port)
{
    _serverIp = ip;
    _serverPort = port;
    return _socketLogic->openWithIp(_serverIp.c_str(), _serverPort);
}

bool HNRoomLogic::closeRoom(bool bReconnect)
{
    _roomConnected = false;
    isStartCheck = false;
    isNeedReLogin = false;
	_reconnect = bReconnect;
	if (!bReconnect) stopHeartBeat();
    return _socketLogic->close();
}

bool HNRoomLogic::connected() const
{
    return _roomConnected && _socketLogic->connected();
}

bool HNRoomLogic::login() const
{
    return _roomLogin;
}

INT HNRoomLogic::sendData(UINT MainID, UINT AssistantID, void *object, INT objectSize)
{
    if (!isNeedReLogin)
	{
        return _socketLogic->send(MainID, AssistantID,HNSocketProtocolData::GameCheckCode, object,objectSize);
    }
    return -1;
}
    
void HNRoomLogic::sendData(UINT MainID, UINT AssistantID, void *object, INT objectSize, SEL_SocketMessage selector)
{
    _socketLogic->send(MainID, AssistantID, HNSocketProtocolData::GameCheckCode,object, objectSize);

    _socketLogic->addEventSelector(MainID, AssistantID, selector);
}

void HNRoomLogic::addEventSelector(UINT MainID, UINT AssistantID,SEL_SocketMessage selector) 
{
    _socketLogic->addEventSelector(MainID, AssistantID, selector);
}

void HNRoomLogic::removeEventSelector(UINT MainID, UINT AssistantID)
{
    _socketLogic->removeEventSelector(MainID, AssistantID);
}

void HNRoomLogic::addObserver(IGameMessageDelegate *delegate)
{
    _addGameNotifyQueue->addObserver(delegate);
    //_gameNotifyQueue->addObserver(delegate);
}

void HNRoomLogic::removeObserver(IGameMessageDelegate *delegate)
{
    //_gameNotifyQueue->removeObserver(delegate);
    _removeGameNotifyQueue->addObserver(delegate);
}

void HNRoomLogic::addRoomObserver(IRoomMessageDelegate *delegate)
{
    //_roomNotifyQueue->addObserver(delegate);
    _addRoomNotifyQueue->addObserver(delegate);
}

void HNRoomLogic::removeRoomObserver(IRoomMessageDelegate *delegate)
{
    //_roomNotifyQueue->removeObserver(delegate);
    _removeRoomNotifyQueue->addObserver(delegate);
}

void HNRoomLogic::roomLogin(UINT uGameID)
{
    MSG_GR_S_RoomLogon data = {0};
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
    data.byFromPhone = 1;
#endif
    data.uNameID = uGameID;
    data.dwUserID = PlatformLogic()->loginResult.dwUserID;
    strcpy(data.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
    sendData(MDM_GR_LOGON, ASS_GR_LOGON_BY_ID, &data, sizeof(data));
}

//——————————————————————
void HNRoomLogic::onHearBeat()
{
    if(!isStartCheck) {
        isStartCheck = true;
    }
	if (isStartCheck)
	{
		receive_time = time(NULL);
	}    
    //  HNLog::logInfo("receive room heart beat");
}

void HNRoomLogic::onConnected(bool connect, emSocketStatus status) 
{
    HNLog::logInfoCn(StringUtils::format(
                         "=======================房间连接状态：%s，状态值：%d\n",
                         (connect ? ("已连接") : ("未连接")), status));
    _roomConnected = connect;

    //dispatchFrameMessage([status](IRoomMessageDelegate *delegate) -> bool {
    //    delegate->onConnectMessage(SocketStatus_STCCESS == status);
    //    return false;
    //});

	//连接状态
	if (SocketStatus_STCCESS == status)
	{
		if (connected() && _selectedRoom != nullptr)
			roomLogin(_selectedRoom->uNameID);
	}
	else
	{
		time_t currTime = time(NULL);
		if (_reconnect && difftime(currTime, _reconnect_time) > RECONNECT_TIME)
		{
			reconnectTimeOut();
		}
		else
		{
			reconnectRoom();
		}
	}
}

void HNRoomLogic::onDisConnect()
{
    HNLog::logInfoCn(StringUtils::format("=======================房间断线"));
    _roomLogin = false;

    _roomConnected = false;
    //stopHeartBeat();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	reconnect();
#endif
}

void HNRoomLogic::onSocketMessage(HNSocketMessage *socketMessage)
{
//    HNLog::logInfoCn(StringUtils::format(
//                         "=======================房间收到新消息-主类型：%d，辅助处理类型:%"
//                         "d，数据包处理代码：%d，消息内容：%s",
//                         socketMessage->messageHead.bMainID,
//                         socketMessage->messageHead.bAssistantID,
//                         socketMessage->messageHead.bHandleCode, (char *)socketMessage->object));
    // 心跳
    if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST_1 == socketMessage->messageHead.bAssistantID)
	{
        //        _socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST_1,
        //        HNSocketProtocolData::GameCheckCode);
        return;
    }

	HNLog::logInfoCn(StringUtils::format("======================= game-main:%d,sub:%d",socketMessage->messageHead.bMainID,
		socketMessage->messageHead.bAssistantID));

    switch (socketMessage->messageHead.bMainID)
	{
    // 连接成功
    case MDM_GR_CONNECT:
	{
		HNLog::logInfoCn(StringUtils::format("=======================连接房间成功消息"));
		startHeartBeat();
		onConnectMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
	break;
    // 登录消息
    case MDM_GR_LOGON:
	{
		HNLog::logInfoCn(StringUtils::format("=======================登录房间消息"));
		onLoginMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    // 用户列表
    case MDM_GR_USER_LIST:
	{
		HNLog::logInfoCn(StringUtils::format("=======================用户列表消息"));
		onUserListMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    // 用户动作
    case MDM_GR_USER_ACTION:
	{
		HNLog::logInfoCn(StringUtils::format("=======================用户动作消息"));
		onUserActionMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    // 房间消息
    case MDM_GR_ROOM:
	{
		HNLog::logInfoCn(StringUtils::format("=======================房间消息"));
		onRoomMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    // 框架消息
    case MDM_GM_GAME_FRAME:
	{
		HNLog::logInfoCn(StringUtils::format("=======================框架消息"));
		onGameServerFrameMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    // 游戏消息
    case MDM_GM_GAME_NOTIFY:
	{
		HNLog::logInfoCn(StringUtils::format("=======================游戏消息"));

		dispatchGameMessage([&socketMessage](IGameMessageDelegate *delegate) -> bool {
			delegate->onGameMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
			return true;
		});
	}
    break;
    case MDM_GR_MANAGE:
	{
		HNLog::logInfoCn(StringUtils::format("=======================封桌&解封桌消息"));

		onManageMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    case MDM_GR_MONEY:
	{
		HNLog::logInfoCn(StringUtils::format("=======================银行消息"));

		H_R_M_Bank_Message(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
	}
    break;
    default:
        break;
    }
    //HNLog::logInfoCn(StringUtils::format("\n"));
}

void HNRoomLogic::onGameServerFrameMessage(NetMessageHead *messageHead,void *object, INT objectSize) {
    //发送游戏信息(1)
    if (ASS_GM_GAME_INFO == messageHead->bAssistantID)
	{
        // 效验数据
        // CHECK_SOCKET_DATA(MSG_GM_S_GameInfo, objectSize, "MSG_GM_S_GameInfo size
        // is error.");

        MSG_GM_S_GameInfo *pGameInfo = (MSG_GM_S_GameInfo *)object;

        // 显示信息
        if (messageHead->uMessageSize >(sizeof(MSG_GM_S_GameInfo) - sizeof(pGameInfo->szMessage))) 
		{
            dispatchGameMessage([&pGameInfo](IGameMessageDelegate *delegate) -> bool {
                delegate->onGameInfoMessage(pGameInfo);
                return true;
            });
        }
    }
    // 发送游戏状态(2)
    else if (ASS_GM_GAME_STATION == messageHead->bAssistantID)
	{
        dispatchGameMessage(
        [&object, objectSize](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameStationMessage(object, objectSize);
            return true;
        });
    }
    // 普通聊天
    else if (ASS_GM_NORMAL_TALK == messageHead->bAssistantID)
	{
        dispatchGameMessage(
        [&object, objectSize](IGameMessageDelegate *delegate) -> bool {
            (dynamic_cast<IGameChartMessageDelegate *>(delegate))
            ->onUserChatMessage(object, objectSize);
            return true;
        });
    }
}

void HNRoomLogic::onRoomMessage(NetMessageHead *messageHead, void *object,INT objectSize)
{
    if (ASS_GR_GAME_BEGIN == messageHead->bAssistantID)
	{ // 游戏开始
        BYTE deskNo = (BYTE)messageHead->bHandleCode;
        // 更新用户状态
        UserInfoModule()->transform(deskNo, [](UserInfoStruct *user, INT index) {
            user->bUserState = USER_PLAY_GAME;
        });

        dispatchGameMessage([&messageHead](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameStartMessage((BYTE)messageHead->bHandleCode);
            return false;
        });

        dispatchFrameMessage(
        [&messageHead](IRoomMessageDelegate *delegate) -> bool {
            delegate->onDeskStateMessage(messageHead->bHandleCode);
            return false;
        });

    } 
	else if (ASS_GR_USER_POINT == messageHead->bAssistantID)// 游戏结算
	{ 
        if (10 == messageHead->bHandleCode)// 同步金币
		{    
            // 效验数据
            CHECK_SOCKET_DATA(MSG_GR_S_RefalshMoney, objectSize,"MSG_GR_S_RefalshMoney size is error.");

            MSG_GR_S_RefalshMoney *pReflashMoney = (MSG_GR_S_RefalshMoney *)object;

            // 处理数据
            UserInfoStruct *pUserItem = UserInfoModule()->findUser(pReflashMoney->dwUserID);
            if (pUserItem == nullptr)
                return;

            pUserItem->i64Money = pReflashMoney->i64Money;

            // 更新自己数据
            if (pReflashMoney->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
                RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
                PlatformLogic()->loginResult.i64Money += pReflashMoney->i64Money;
            }
        } 
		else if (11 == messageHead->bHandleCode)// 同步经验值
		{ 
            // 效验数据
            CHECK_SOCKET_DATA(MSG_GR_S_RefalshMoney, objectSize,"MSG_GR_S_RefalshMoney size is error.");

            MSG_GR_S_RefalshMoney *pReflashMoney = (MSG_GR_S_RefalshMoney *)object;

            // 处理数据
            UserInfoStruct *pUserItem = UserInfoModule()->findUser(pReflashMoney->dwUserID);
            if (pUserItem == nullptr)
                return;

            pUserItem->dwPoint = pReflashMoney->i64Money;

            // 更新自己数据
            if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
                RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
            }

        } 
		else if (0 == messageHead->bHandleCode)// 同步经验值
		{ 
            // 效验数据
            CHECK_SOCKET_DATA(MSG_GR_R_UserPoint, objectSize,"MSG_GR_R_UserPoint size is error.");

            MSG_GR_R_UserPoint *pUserPoint = (MSG_GR_R_UserPoint *)object;

            UserInfoStruct *pUserItem = UserInfoModule()->findUser(pUserPoint->dwUserID);
            if (pUserItem == nullptr)
                return;

            // 更新用户信息
            pUserItem->dwPoint += pUserPoint->dwPoint;  //用户经验值
            pUserItem->i64Money += pUserPoint->dwMoney; //用户金币
            // pUserItem->dwSend += pUserPoint->dwSend;
            // //赠送
            pUserItem->uWinCount += pUserPoint->bWinCount;   //胜局
            pUserItem->uLostCount += pUserPoint->bLostCount; //输局
            pUserItem->uMidCount += pUserPoint->bMidCount;   //平局
            pUserItem->uCutCount += pUserPoint->bCutCount;   //逃局

            // pUserItem->strAutoSendMoney += pUserPoint->strAutoSendMoney;
            // //添加自动赠送 2013-01-29 wangzhitu

            // 更新自己数据
            if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID) {
                RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
                PlatformLogic()->loginResult.i64Money += pUserPoint->dwMoney;
            }
        }
        dispatchFrameMessage(
        [&object, objectSize](IRoomMessageDelegate *delegate) -> bool {
            delegate->onGamePointMessage(object, objectSize);
            return false;
        });

        dispatchGameMessage(
        [&object, objectSize](IGameMessageDelegate *delegate) -> bool {
            delegate->onGamePointMessage(object, objectSize);
            return false;
        });
    } else if (ASS_GR_GAME_END == messageHead->bAssistantID) // 游戏结束
	{ 
        BYTE deskIndex = (BYTE)messageHead->bHandleCode;

        dispatchGameMessage([deskIndex](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameEndMessage(deskIndex);
            return false;
        });

        if ((_gameRoomRule & GRR_CONTEST) || (_gameRoomRule & GRR_QUEUE_GAME))
		{
            std::vector<UserInfoStruct *> users;
            UserInfoModule()->findDeskUsers(deskIndex, users);
            std::for_each(users.begin(), users.end(), [](UserInfoStruct *user) {
                user->bDeskStation = INVALID_DESKSTATION;
                user->bDeskNO = INVALID_DESKNO;
                user->bUserState = USER_LOOK_STATE;
            });
        }

        // 更新用户状态
        UserInfoModule()->transform(deskIndex, [](UserInfoStruct *user, INT index) {
            user->bUserState = USER_SITTING;
        });

        dispatchFrameMessage(
        [&messageHead](IRoomMessageDelegate *delegate) -> bool {
            delegate->onDeskStateMessage(messageHead->bHandleCode);
            return false;
        });

    }
	else if (ASS_GR_USER_AGREE == messageHead->bAssistantID) // 用户同意
	{ 
        // 效验数据
        CHECK_SOCKET_DATA(MSG_GR_R_UserAgree, objectSize,"MSG_GR_R_UserAgree size is error.");

        MSG_GR_R_UserAgree *pUserAgree = (MSG_GR_R_UserAgree *)object;

        UserInfoStruct *pUserItem = UserInfoModule()->findUser(pUserAgree->bDeskNO, pUserAgree->bDeskStation);
        if (nullptr != pUserItem) 
		{
            pUserItem->bUserState = USER_ARGEE;
        }

        dispatchGameMessage([&pUserAgree](IGameMessageDelegate *delegate) -> bool {
            delegate->onUserAgreeMessage(pUserAgree);
            return false;
        });
    } 
	else if (ASS_GR_INSTANT_UPDATE == messageHead->bAssistantID) // 即时更新分数金币
	{ 
        // 效验数据
        CHECK_SOCKET_DATA(MSG_GR_R_InstantUpdate, objectSize,"MSG_GR_R_InstantUpdate size is error.");

        MSG_GR_R_InstantUpdate *pInstantUpdate = (MSG_GR_R_InstantUpdate *)object;

        // 更新数据
        UserInfoStruct *pUserItem = UserInfoModule()->findUser(pInstantUpdate->dwUserID);
        if (nullptr != pUserItem)
		{
            pUserItem->i64Money += pInstantUpdate->dwMoney;
            pUserItem->dwPoint += pInstantUpdate->dwPoint;
        }

        // 更新自己数据
        if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
            RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
            PlatformLogic()->loginResult.i64Money = pInstantUpdate->dwMoney;
        }
    } 
	else if (ASS_GR_CONTEST_APPLYINFO == messageHead->bAssistantID)// 比赛信息广播
	{ 
        CHECK_SOCKET_DATA(MSG_GR_I_ContestInfo, objectSize,"MSG_GR_ContestChange size of error!");
        MSG_GR_I_ContestInfo *contestInfo = (MSG_GR_I_ContestInfo *)object; // NET_ROOM_CONTEST_CHANGE_RESULT

        dispatchFrameMessage([&](IRoomMessageDelegate *delegate) -> bool {
            delegate->onGameContestNotic(contestInfo);
            return false;
        });

        dispatchGameMessage([&](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameContestNotic(contestInfo);
            return false;
        });
    }
    else if (ASS_GR_USER_CONTEST == messageHead->bAssistantID)// 用户比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，排名有更改
	{ 
        CHECK_SOCKET_DATA(MSG_GR_ContestChange, objectSize,"MSG_GR_ContestChange size of error!");
        MSG_GR_ContestChange *contestChange = (MSG_GR_ContestChange *)object; // NET_ROOM_CONTEST_CHANGE_RESULT

        UserInfoStruct *user = UserInfoModule()->findUser(contestChange->dwUserID);
        if (nullptr != user)
		{
            user->iRankNum = contestChange->iRankNum;           // 排行名次
            user->iRemainPeople = contestChange->iRemainPeople; // 比赛中还剩下的人数
            user->i64ContestScore = contestChange->i64ContestScore; // 比赛分数
            user->iContestCount = contestChange->iContestCount + 1; // 比赛局数
        }

        if (contestChange->dwUserID != PlatformLogic()->loginResult.dwUserID)
            return;

        dispatchGameMessage(
        [contestChange](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameUserContset(contestChange);
            return false;
        });

    } 
	else if (ASS_GR_CONTEST_GAMEOVER == messageHead->bAssistantID) // 比赛结束
	{ 
        CHECK_SOCKET_DATA(MSG_GR_ContestAward, objectSize,"MSG_GR_ContestAward size of error!");
        MSG_GR_ContestAward *contestAward = (MSG_GR_ContestAward *)object;
        /*
                                UserInfoStruct* user =
           UserInfoModule()->findUser(loginResult.pUserInfoStruct.dwUserID);
                                if (nullptr != user)
                                {
                                        user->contestAward = *contestAward;
                                        loginResult.pUserInfoStruct.contestAward =
           *contestAward;
                                }
        */
        dispatchFrameMessage(
        [contestAward](IRoomMessageDelegate *delegate) -> bool {
            delegate->onGameContestOver(contestAward);
            return false;
        });

        dispatchGameMessage([contestAward](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameContestOver(contestAward);
            return false;
        });
    } 
	else if (ASS_GR_CONTEST_KICK == messageHead->bAssistantID) // 用户被淘汰
	{ 
        dispatchGameMessage([](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameContestKick();
            return false;
        });
    } 
	else if ( ASS_GR_CONTEST_WAIT_GAMEOVER == messageHead->bAssistantID) // 比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名
	{ 
        dispatchGameMessage([](IGameMessageDelegate *delegate) -> bool {
            delegate->onGameContestWaitOver();
            return false;
        });
    } 
	else if ( ASS_GR_INIT_CONTEST == messageHead->bAssistantID) // 比赛开始，初始化比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，初始化排名信息
	{ 
        CHECK_SOCKET_DATA(MSG_GR_ContestChange, objectSize,"MSG_GR_ContestChange size of error!");

        MSG_GR_ContestChange *contestChange = (MSG_GR_ContestChange *)object;

        UserInfoStruct *user = UserInfoModule()->findUser(contestChange->dwUserID);
        if (nullptr != user)
		{
            user->iRankNum = contestChange->iRankNum;           // 排行名次
            user->iRemainPeople = contestChange->iRemainPeople; // 比赛中还剩下的人数
            // user->i64ContestScore = contestChange->i64ContestScore; // 比赛分数
            user->iContestCount = contestChange->iContestCount + 1; // 比赛局数
        }
        /*dispatchGameMessage([contestChange](IGameMessageDelegate* delegate) ->
        bool
        {
                delegate->onGameContestInit(contestChange);
                return false;
        });	*/
    }
    //		else if (ASS_GR_USER_CONTEST == messageHead->bAssistantID)
    ////
    //用户比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，排名有更改
    //		{
    //			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_CHANGE_RESULT,
    // objectSize,
    //"NET_ROOM_CONTEST_CHANGE_RESULT size of error!");
    //			NET_ROOM_CONTEST_CHANGE_RESULT* contestChange =
    //(NET_ROOM_CONTEST_CHANGE_RESULT*)object;
    //
    //			UserInfoStruct* user =
    // UserInfoModule()->findUser(contestChange->dwUserID);
    //			if (nullptr != user)
    //			{
    //				user->iRankNum = contestChange->bRankNum;
    ////
    //排行名次
    //				user->iRemainPeople =
    // contestChange->bRemainPeople;
    ////
    //比赛中还剩下的人数
    //				user->i64ContestScore =
    // contestChange->llContestScore;
    ////
    //比赛分数
    //			}
    //
    //			dispatchGameMessage([contestChange](IGameMessageDelegate*
    // delegate) -> bool
    //			{
    //				delegate->onGameUserContset(contestChange);
    //				return false;
    //			});
    //		}
    //		else if (ASS_GR_CONTEST_GAMEOVER == messageHead->bAssistantID)
    ////
    //比赛结束
    //		{
    //			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_AWARD_RESULT,
    // objectSize,
    //"NET_ROOM_CONTEST_AWARD_RESULT size of error!");
    //			NET_ROOM_CONTEST_AWARD_RESULT* contestAward =
    //(NET_ROOM_CONTEST_AWARD_RESULT*)object;
    ///*
    //			UserInfoStruct* user =
    // UserInfoModule()->findUser(loginResult.pUserInfoStruct.dwUserID);
    //			if (nullptr != user)
    //			{
    //				user->contestAward = *contestAward;
    //				loginResult.pUserInfoStruct.contestAward =
    //*contestAward;
    //			}
    //*/
    //			dispatchGameMessage([contestAward](IGameMessageDelegate*
    // delegate) -> bool
    //			{
    //				delegate->onGameContestOver(contestAward);
    //				return false;
    //			});
    //		}
    //		else if (ASS_GR_CONTEST_KICK == messageHead->bAssistantID)
    ////
    //用户被淘汰
    //		{
    //			dispatchGameMessage([](IGameMessageDelegate* delegate)
    //->
    // bool
    //			{
    //				delegate->onGameContestKick();
    //				return false;
    //			});
    //		}
    //		else if (ASS_GR_CONTEST_WAIT_GAMEOVER ==
    // messageHead->bAssistantID)
    //// 比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名
    //		{
    //			dispatchGameMessage([](IGameMessageDelegate* delegate)
    //->
    // bool
    //			{
    //				delegate->onGameContestWaitOver();
    //				return false;
    //			});
    //		}
    // else if (ASS_GR_INIT_CONTEST == messageHead->bAssistantID)		//
    // 比赛开始，初始化比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，初始化排名信息
    //{
    //	CHECK_SOCKET_DATA(NET_ROOM_CONTEST_CHANGE_RESULT, objectSize,
    //"NET_ROOM_CONTEST_CHANGE_RESULT size of error!");

    //	NET_ROOM_CONTEST_CHANGE_RESULT* contestChange =
    //(NET_ROOM_CONTEST_CHANGE_RESULT*)object;

    //	dispatchGameMessage([contestChange](IGameMessageDelegate* delegate) ->
    // bool
    //	{
    //		delegate->onGameContestInit(contestChange);
    //		return false;
    //	});
    //}
    // else if (ASS_GR_CONTEST_PEOPLE == messageHead->bAssistantID)	 //
    // 获取报名数量，登陆完成后服务端主动发送NET_ROOM_CONTEST_PEOPLE_RESUL
    //{
    //	CHECK_SOCKET_DATA(NET_ROOM_CONTEST_PEOPLE_RESULT, objectSize,
    //"NET_ROOM_CONTEST_PEOPLE_RESULT size of error!");
    //	NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople =
    //(NET_ROOM_CONTEST_PEOPLE_RESULT*)object;

    //	// 分发房间数据
    //	dispatchFrameMessage([contestPeople](IRoomMessageDelegate* delegate) ->
    // bool
    //	{
    //		delegate->onGameContestPeople(contestPeople);
    //		return false;
    //	});
    //}
    // else if (ASS_GR_CONTEST_RECORD == messageHead->bAssistantID)	 //
    // 个人参赛纪录，登陆完成后服务端主动发送NET_ROOM_CONTEST_RECORD_RESULT
    //{
    //	CHECK_SOCKET_DATA(NET_ROOM_CONTEST_RECORD_RESULT, objectSize,
    //"NET_ROOM_CONTEST_RECORD_RESULT size of error!");
    //	NET_ROOM_CONTEST_RECORD_RESULT* contestRecord =
    //(NET_ROOM_CONTEST_RECORD_RESULT*)object;

    //	// 分发房间数据
    //	dispatchFrameMessage([contestRecord](IRoomMessageDelegate* delegate) ->
    // bool
    //	{
    //		delegate->onGameContestRecord(contestRecord);
    //		return false;
    //	});
    //}
    // else if (ASS_GR_CONTEST_AWARDS == messageHead->bAssistantID)	 //
    // 个人参赛纪录，登陆完成后服务端主动发送NET_ROOM_CONTEST_RECORD_RESULT
    //{
    //	UINT count = objectSize / sizeof(NET_ROOM_CONTEST_AWARD_RESULT);
    //	if (count > 0)
    //	{
    //		NET_ROOM_CONTEST_AWARD_RESULT* contestRecord =
    //(NET_ROOM_CONTEST_AWARD_RESULT*)object;

    //		std::vector<NET_ROOM_CONTEST_AWARD_RESULT*> results(count);
    //		while (count > 0)
    //		{
    //			NET_ROOM_CONTEST_AWARD_RESULT* tmp = contestRecord++;
    //			results[count - 1] = tmp;
    //			count--;
    //		}

    //		// 分发房间数据
    //		dispatchFrameMessage([&results](IRoomMessageDelegate* delegate)
    //->
    // bool
    //		{
    //			delegate->onGameContestAwards(&results);
    //			return false;
    //		});
    //	}
    /*	}*/
}

//辅助id传回桌子号，handlecode传回座位号。
void HNRoomLogic::onManageMessage(NetMessageHead *messageHead, void *object,
                                  INT objectSize) {
    if (messageHead->bHandleCode == 1) {
        //封桌
        dispatchFrameMessage(
        [&messageHead](IRoomMessageDelegate *delegate) -> bool {
            delegate->onLockDeskMessage(messageHead->bAssistantID);
            return true;
        });
    } else {
        dispatchFrameMessage(
        [&messageHead](IRoomMessageDelegate *delegate) -> bool {
            delegate->onUnLockDeskMessage(messageHead->bAssistantID);
            return true;
        });
    }
}

// 房间连接处理
void HNRoomLogic::onConnectMessage(NetMessageHead *messageHead, void *object,INT objectSize)
{
    // 参数校验
    if (messageHead->bAssistantID != ASS_GR_CONNECT_SUCCESS)
        return;

    CHECK_SOCKET_DATA(MSG_S_ConnectSuccess, objectSize,"MSG_S_ConnectSuccess size is error.");

    MSG_S_ConnectSuccess *pConnectSuccess = (MSG_S_ConnectSuccess *)object;

    HNSocketProtocolData::GameCheckCode = (UINT)(pConnectSuccess->i64CheckCode - getSecrectKey()) / 23;

    _roomConnected = true;

	HNLog::logInfoCn(StringUtils::format("HNSocketProtocolData::GameCheckCode >> [%d]", HNSocketProtocolData::GameCheckCode));

    // 分发事件
    dispatchFrameMessage([](IRoomMessageDelegate *delegate) -> bool {
        delegate->onConnectMessage(true);
        return false;
    });


    if (UserDefault::getInstance()->getBoolForKey("NEW_YEAR") || (_reconnect && connected())) {
        ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
        RoomLogic()->roomLogin(pRoomInfo->uNameID);
    }
}

// 房间登录处理
void HNRoomLogic::onLoginMessage(NetMessageHead *messageHead, void *object,
                                 INT objectSize) {
    bool isSucess = false;
    switch (messageHead->bAssistantID) {
    case ASS_GP_LOGON_ALLO_PART: { // 异地登录

    } break;
    case ASS_GP_LOGON_LOCK_VALID: { // 设备锁定
    } break;
    case ASS_GR_LOGON_SUCCESS: { // 房间登录成功  (1)
        // 数据校验
        CHECK_SOCKET_DATA(MSG_GR_R_LogonResult, objectSize,
                          "MSG_GR_R_LogonResult size is error.");

        MSG_GR_R_LogonResult *pLogonResult = (MSG_GR_R_LogonResult *)object;

        loginResult = *pLogonResult;
        _roomLogin = ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode;

        // 分发事件
        dispatchFrameMessage(
        [&messageHead](IRoomMessageDelegate *delegate) -> bool {
            delegate->onLoginMessage(true, messageHead->bHandleCode);
            return false;
        });

        if (UserDefault::getInstance()->getBoolForKey("NEW_YEAR") || (_reconnect && connected())) {
            isSucess = true;
            //MSG_GR_S_UserSit data;
            //data.bDeskIndex = 0;
            //data.bDeskStation = 0;
            //RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data, sizeof(data));



            ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
            if (nullptr == pRoomInfo) {
                //_callback->onRoomSitCallback(false, "room info null.", _roomID, deskNo, INVALID_DESKSTATION);
                return;
            }

            std::vector<UserInfoStruct*> deskUsers;
            UserInfoModule()->findDeskUsers(0, deskUsers);
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
            for (size_t i = 0; i < empty.size(); i++) {
                if (empty[i]) {
                    seatNo = i;
                    break;
                }
            }

            if (INVALID_DESKSTATION != seatNo) {
                //requestSit(deskNo, seatNo);
                MSG_GR_S_UserSit data;
                data.bDeskIndex = 0;
                data.bDeskStation = seatNo;
                data.nClientFlag = 0;
                RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &data, sizeof(data));
            } else {
                //_callback->onRoomSitCallback(false, "there is no empty seat.", _roomID, deskNo, seatNo);
            }

        }
    }
    break;
    case ASS_GR_LOGON_ERROR: { // 登录失败
        // 分发事件
        dispatchFrameMessage(
        [&messageHead](IRoomMessageDelegate *delegate) -> bool {
            delegate->onLoginMessage(false, messageHead->bHandleCode);
            return false;
        });
    }
    break;
    case ASS_GR_SEND_FINISH: { // 房间登录完成(6)
        // 分发事件
        dispatchFrameMessage([](IRoomMessageDelegate *delegate) -> bool {
            delegate->onLoginFinishMessage();
            return false;
        });
    }
    break;
    default:
        break;
    }
    if (!isSucess) {
        CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    }
}

// 用户列表处理
void HNRoomLogic::onUserListMessage(NetMessageHead *messageHead, void *object,
                                    INT objectSize) {
    switch (messageHead->bAssistantID) {
    case ASS_GR_ONLINE_USER:   // 在线用户信息(2)
    case ASS_GR_NETCUT_USER: { // 断线用户信息(3)
        UINT userCount = objectSize / sizeof(UserInfoStruct);
        UserInfoStruct *pUserInfo = (UserInfoStruct *)object;
        HNLog::logInfoCn(StringUtils::format(
                             "玩家（%d）：更新在线列表(%d)\n",
                             RoomLogic()->loginResult.pUserInfoStruct.dwUserID, userCount));
        while (userCount-- > 0) {
            UserInfoModule()->updateUser(pUserInfo++);
        }
    }
    break;
    case ASS_GR_DESK_STATION: { // 桌子信息(4)
        //  数据校验
        CHECK_SOCKET_DATA(MSG_GR_DeskStation, objectSize,
                          "MSG_GR_DeskStation size is error.");

        MSG_GR_DeskStation *pDeskStation = (MSG_GR_DeskStation *)object;
        deskStation = *pDeskStation;
    }
    break;
    default:
        break;
    }
}

// 用户动作处理
void HNRoomLogic::onUserActionMessage(NetMessageHead *messageHead, void *object, INT objectSize)
{
    switch (messageHead->bAssistantID)
	{
    case ASS_GR_QUEUE_USER_SIT://排队用户坐下;
	{
        auto queueData = (int *)object;
        auto deskNo = (BYTE)queueData[0];
        bool isFind = false;
        for (int i = 0; i < 3; i++) 
		{
            if (PlatformLogic()->loginResult.dwUserID == queueData[3 * i + 1])
			{
                isFind = true;
            }
        }
        std::vector<QUEUE_USER_SIT_RESULT *> queueUsers;
        if (isFind)
		{
            for (int i = 0; i < 3; i++) 
			{
                QUEUE_USER_SIT_RESULT *tmp = new QUEUE_USER_SIT_RESULT;
                tmp->dwUserID = queueData[3 * i + 1];
                tmp->bDeskStation = i;
                UserInfoStruct *userInfo = UserInfoModule()->findUser(tmp->dwUserID);

                if (nullptr != userInfo)
				{
                    userInfo->bDeskNO = deskNo;
                    userInfo->bDeskStation = tmp->bDeskStation;
                    userInfo->bUserState = USER_ARGEE;
                    userInfo->iRankNum = queueData[3 * i + 2];
                    userInfo->iRemainPeople = queueData[3 * i + 3];
                    // UserInfoModule()->addUser(userInfo);
                }

                queueUsers.push_back(tmp);
                // CC_SAFE_DELETE(tmp);
            }
        }

        dispatchFrameMessage(
        [&deskNo, &queueUsers](IRoomMessageDelegate *delegate) -> bool {
            delegate->onQueueUserSitMessage(deskNo, queueUsers);
            return false;
        });

        dispatchGameMessage(
        [&deskNo, &queueUsers](IGameMessageDelegate *delegate) -> bool {
            delegate->onQueueUserSitMessage(deskNo, queueUsers);
            return false;
        });

        for (auto sit : queueUsers)
		{
            CC_SAFE_DELETE(sit);
        }
        queueUsers.clear();
    }
    break;
    case ASS_GR_USER_COME://用户进入房间(5)
	{	
        // 数据校验
        CHECK_SOCKET_DATA(MSG_GR_R_UserCome, objectSize,"MSG_GR_R_UserCome size is error.");

        MSG_GR_R_UserCome *pUserCome = (MSG_GR_R_UserCome *)object;

        // GamePromptLayer::showPrompt(StringUtils::format("玩家(%d)\n进入",
        // pUserCome->pUserInfoStruct.dwUserID).c_str());
        HNLog::logInfo(StringUtils::format("玩家(%d)\n进入",pUserCome->pUserInfoStruct.dwUserID).c_str());

        UserInfoModule()->updateUser(&pUserCome->pUserInfoStruct);
        // 分发房间消息
        dispatchFrameMessage([&pUserCome](IRoomMessageDelegate *delegate) -> bool {
            delegate->onUserComeMessage(&pUserCome->pUserInfoStruct);
            return false;
        });

		if (_reconnect)
		{
			_reconnect = false;
			MSG_GM_S_ClientInfo ClientInfo;
			ClientInfo.bEnableWatch = 0;
			sendData(MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, &ClientInfo, sizeof(ClientInfo));
		}
    }
    break;
    case ASS_GR_USER_LEFT: //用户离开房间
	{ 
        // 数据校验
        CHECK_SOCKET_DATA(MSG_GR_R_UserLeft, objectSize,"MSG_GR_R_UserLeft size is error.");

        MSG_GR_R_UserLeft *pUserLeft = (MSG_GR_R_UserLeft *)object;

        UserInfoStruct *userInfo = UserInfoModule()->findUser(pUserLeft->dwUserID);

        // 分发房间消息
        dispatchFrameMessage([&userInfo](IRoomMessageDelegate *delegate) -> bool {
            delegate->onUserLeftMessage(userInfo);
            return false;
        });

        UserInfoModule()->removeUser(pUserLeft->dwUserID);
    }
    break;
    case ASS_GR_USER_CUT: //用户断线
	{	
        //数据校验
        CHECK_SOCKET_DATA(MSG_GR_R_UserCut, objectSize,"MSG_GR_R_UserCut size is error.");

        MSG_GR_R_UserCut *pUserCut = (MSG_GR_R_UserCut *)object;

        UserInfoStruct *userInfo = UserInfoModule()->findUser(pUserCut->dwUserID);
        if (nullptr != userInfo) 
		{
            //设置数据
            userInfo->bUserState = USER_CUT_GAME;
        }

        // 分发游戏消息
        dispatchGameMessage(
        [&pUserCut, &userInfo](IGameMessageDelegate *delegate) -> bool {
            delegate->onUserCutMessage(pUserCut->dwUserID, pUserCut->bDeskNO,
            pUserCut->bDeskStation);
            return false;
        });

        // 分发房间消息
        dispatchFrameMessage([&pUserCut](IRoomMessageDelegate *delegate) -> bool {
            delegate->onUserCutMessage(pUserCut->dwUserID, pUserCut->bDeskNO,
            pUserCut->bDeskStation);
            return false;
        });
    }
    break;
    case ASS_GR_USER_UP:    // 用户站起
    case ASS_GR_WATCH_UP:	// 旁观站起
	{ 
        if (ERR_GR_SIT_SUCCESS == messageHead->bHandleCode)
		{
            // 数据校验
            CHECK_SOCKET_DATA(MSG_GR_R_UserUp, objectSize,"MSG_GR_R_UserUp size is error.");

            MSG_GR_R_UserUp *pUserUp = (MSG_GR_R_UserUp *)object;

            // 更新自己信息
            if (pUserUp->dwUserID == loginResult.pUserInfoStruct.dwUserID)
			{
                loginResult.pUserInfoStruct.bDeskNO = pUserUp->bDeskIndex;
                loginResult.pUserInfoStruct.bDeskStation = pUserUp->bDeskStation;
                loginResult.pUserInfoStruct.bUserState = pUserUp->bUserState;
            }

            UserInfoStruct *userInfo = UserInfoModule()->findUser(pUserUp->dwUserID);
            // 更新用户数据
            if (nullptr != userInfo)
			{
                userInfo->bDeskNO = INVALID_DESKNO;
                userInfo->bDeskStation = INVALID_DESKSTATION;
                userInfo->bUserState = USER_LOOK_STATE;
            }
            // 分发游戏消息
            dispatchGameMessage(
            [&pUserUp, &userInfo](IGameMessageDelegate *delegate) -> bool {
                delegate->onUserUpMessage(pUserUp, userInfo);
                return false;
            });

            // 分发房间消息
            dispatchFrameMessage(
            [&pUserUp, &userInfo](IRoomMessageDelegate *delegate) -> bool {
                delegate->onUserUpMessage(pUserUp, userInfo);
                return false;
            });
        } 
		else 
		{
            printfSitError(messageHead);
        }
    }
    break;
    case ASS_GR_SIT_ERROR: 
	{
        // 分发游戏消息
        dispatchGameMessage([](IGameMessageDelegate *delegate) -> bool {
            delegate->onUserSitMessage(nullptr, nullptr);
            return false;
        });

        // 分发房间消息
        dispatchFrameMessage([](IRoomMessageDelegate *delegate) -> bool {
            delegate->onUserSitMessage(nullptr, nullptr);
            return false;
        });
        printfSitError(messageHead);
    }
    break;
    case ASS_GR_USER_SIT:// 用户坐下
//        {
//        CCLOG("====  on UserSit  ASS_GR_USER_SIT		2 ");
    // dispatchFrameMessage([&pUserSit, &userInfo](IRoomMessageDelegate*
    // delegate) -> bool {
    //	delegate->onUserSitMessage(pUserSit, userInfo);
//    }
    case ASS_GR_WATCH_SIT: // 旁观坐下
	{ 
        if (ERR_GR_SIT_SUCCESS == messageHead->bHandleCode)
		{
            // 数据校验
            CHECK_SOCKET_DATA(MSG_GR_R_UserSit, objectSize,"MSG_GR_R_UserSit size is error.");

            MSG_GR_R_UserSit *pUserSit = (MSG_GR_R_UserSit *)object;

            HNLog::logInfoCn(StringUtils::format("玩家（%d）：玩家(%d)坐下:%d,%d,%d",
										RoomLogic()->loginResult.pUserInfoStruct.dwUserID,
										pUserSit->dwUserID,pUserSit->bDeskIndex,
										pUserSit->bDeskStation,pUserSit->bUserState));
            // 更新自己信息
            if (pUserSit->dwUserID == loginResult.pUserInfoStruct.dwUserID) 
			{
                loginResult.pUserInfoStruct.bDeskNO = pUserSit->bDeskIndex;
                loginResult.pUserInfoStruct.bDeskStation = pUserSit->bDeskStation;
                loginResult.pUserInfoStruct.bUserState = pUserSit->bUserState;
            }

            UserInfoStruct *userInfo = UserInfoModule()->findUser(pUserSit->dwUserID);
            // 更新用户数据
            if (nullptr != userInfo) 
			{
                userInfo->bDeskNO = pUserSit->bDeskIndex;
                userInfo->bDeskStation = pUserSit->bDeskStation;
                userInfo->bUserState = pUserSit->bUserState;
                UserInfoModule()->updateUser(userInfo);
            }

            // 分发游戏消息
            dispatchGameMessage(
            [&pUserSit, &userInfo](IGameMessageDelegate *delegate) -> bool {
                delegate->onUserSitMessage(pUserSit, userInfo);
                return false;
            });

            // 分发房间消息
            dispatchFrameMessage(
            [&pUserSit, &userInfo](IRoomMessageDelegate *delegate) -> bool {
                delegate->onUserSitMessage(pUserSit, userInfo);
                return false;
            });

            if (UserDefault::getInstance()->getBoolForKey("NEW_YEAR") || (_reconnect && connected())) 
			{
                if (nullptr == pUserSit || nullptr == userInfo) 
				{
                    if (pUserSit == nullptr) 
					{
                        return;
                    }
                    if (pUserSit->dwUserID != PlatformLogic()->loginResult.dwUserID)
					{
                        return;
                    }
                }

                // 断线重连进来
                if (pUserSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
				{
                    if (INVALID_DESKNO != pUserSit->bDeskIndex && INVALID_DESKSTATION != pUserSit->bDeskStation)
					{
                        int deskNo = pUserSit->bDeskIndex;
                        int seatNo = pUserSit->bDeskStation;
                        if (INVALID_DESKNO != deskNo && INVALID_DESKSTATION != seatNo) 
						{
                            // 启动游戏
                            bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
                            UserDefault::getInstance()->setBoolForKey("NEW_YEAR",false);
                        }
                    }
                }
            }
        }
    }
    break;
    default:
        break;
    }
}

//——————————————————————
// 错误输出
void HNRoomLogic::printfSitError(NetMessageHead *messageHead) {
    //显示错误信息
    std::string strMessage;
    switch (messageHead->bHandleCode) {
    case ERR_GR_BEGIN_GAME: {
        strMessage = "坐下此位置失败,游戏已经开始了!";
        break;
    }
    case ERR_GR_ALREAD_USER: {
        strMessage = "坐下此位置失败,下次动作快一点喔!";
        break;
    }
    case ERR_GR_PASS_ERROR: {
        strMessage = "游戏桌密码错误,请在游戏设置中重新设置您的携带密码!";
        break;
    }
    case ERR_GR_IP_SAME: {
        strMessage = "同桌玩家不允许有相同 IP 地址的玩家一起进行游戏!";
        break;
    }
    case ERR_GR_IP_SAME_3: {
        strMessage = "同桌玩家不允许有 IP 地址前３位相同的玩家一起进行游戏!";
        break;
    }
    case ERR_GR_IP_SAME_4: {
        strMessage = "同桌玩家不允许有IP 地址前４位相同的玩家一起进行游戏!";
        break;
    }
    case ERR_GR_CUT_HIGH: {
        strMessage = "同桌玩家认为您的逃跑率太高,不愿意和您游戏!";
        break;
    }
    case ERR_GR_POINT_LOW: {
        strMessage = "本桌玩家设置的进入条件，您不符合本桌进入条件!";
        break;
    }
    case ERR_GR_POINT_HIGH: {
        strMessage = "本桌玩家设置的进入条件，您不符合本桌进入条件!";
        break;
    }
    case ERR_GR_NO_FRIEND: {
        strMessage = "此桌有您不欢迎的玩家!";
        break;
    }
    case ERR_GR_POINT_LIMIT: {
        char buf[128];
        sprintf(buf, "此游戏桌需要至少 %u 的游戏积分,您的积分不够,不能游戏!",
                loginResult.uLessPoint);
        strMessage = buf;
        break;
    }
    case ERR_GR_CAN_NOT_LEFT: {
        strMessage = "您正在游戏中...";
        break;
    }
    case ERR_GR_NOT_FIX_STATION: {
        strMessage = "您不能加入此游戏桌游戏!";
        break;
    }
    case ERR_GR_MATCH_FINISH: {
        strMessage = "您的比赛已经结束了,不能继续参加比赛!";
        break;
    }
    case ERR_GR_MATCH_WAIT: { //比赛场排队中
        break;
    }
    case ERR_GR_UNENABLE_WATCH: {
        strMessage = "不允许旁观游戏!";
        break;
    }
    default:
        strMessage = "抱歉,服务器网络繁忙,请重新进入游戏房间后选择游戏桌位置!";
        break;
    }

    CHAR buf[512];
    sprintf(buf, ("坐下失败 >> %s >> errorCode = [%u]"), strMessage.c_str(),
            messageHead->bHandleCode);
    HNLOG_ERROR("%s", GBKToUtf8(buf));
}

bool HNRoomLogic::dispatchFrameMessage(const RoomFrameMessageFun &func) {
    auto iterA = _addRoomNotifyQueue->begin();
    while (iterA != _addRoomNotifyQueue->end()) {
        auto next = iterA;
        next++;
        _roomNotifyQueue->addObserver(iterA->second);
        _addRoomNotifyQueue->removeObserver(iterA->second);
        iterA = next;
    }

    auto iterR = _removeRoomNotifyQueue->begin();
    while (iterR != _removeRoomNotifyQueue->end()) {
        auto next = iterR;
        next++;
        _roomNotifyQueue->removeObserver(iterR->second);
        _removeRoomNotifyQueue->removeObserver(iterR->second);
        iterR = next;
    }

    for (auto iter = _roomNotifyQueue->begin(); iter != _roomNotifyQueue->end();
            ++iter) {
        bool bRemoved = false;
        for (auto iterR = _removeRoomNotifyQueue->begin();
                iterR != _removeRoomNotifyQueue->end(); iterR++) {
            if (iterR->second == iter->second) {
                bRemoved = true;
                break;
            }
        }
        if (!bRemoved) {
            func(iter->second);
        }
    }
    return true;
}

bool HNRoomLogic::dispatchGameMessage(const GameMessageFun &func) {
    auto iterA = _addGameNotifyQueue->begin();
    while (iterA != _addGameNotifyQueue->end()) {
        auto next = iterA;
        next++;
        _gameNotifyQueue->addObserver(iterA->second);
        _addGameNotifyQueue->removeObserver(iterA->second);
        iterA = next;
    }

    auto iterR = _removeGameNotifyQueue->begin();
    while (iterR != _removeGameNotifyQueue->end()) {
        auto next = iterR;
        next++;
        _gameNotifyQueue->removeObserver(iterR->second);
        _removeGameNotifyQueue->removeObserver(iterR->second);
        iterR = next;
    }

    for (auto iter = _gameNotifyQueue->begin(); iter != _gameNotifyQueue->end();
            ++iter) {
        bool bRemoved = false;
        for (auto iterR = _removeGameNotifyQueue->begin();
                iterR != _removeGameNotifyQueue->end(); iterR++) {
            if (iterR->second == iter->second) {
                bRemoved = true;
                break;
            }
        }
        if (!bRemoved) {
            func(iter->second);
        }
    }
    return true;
}

void HNRoomLogic::H_R_M_Bank_Message(const NetMessageHead* messageHead, void* object, INT objectSize) 
{
    if (ASS_GR_CHECK_IN == messageHead->bAssistantID || ASS_GR_CHECK_OUT == messageHead->bAssistantID)
	{
        // 数据校验
        CHECK_SOCKET_DATA(MSG_GR_Check_money, objectSize, "MSG_GR_Check_money size is error.");

        MSG_GR_Check_money * pUserSit = (MSG_GR_Check_money*)object;

        UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserSit->dwUserID);

        // 更新用户数据

        if (nullptr == userInfo)
            return;

        if (ASS_GR_CHECK_IN == messageHead->bAssistantID) {
            userInfo->i64Money -= pUserSit->i64CheckOut;//存款时减去身上的钱
            userInfo->i64Bank += pUserSit->i64CheckOut;
        } else {
            userInfo->i64Money += pUserSit->i64CheckOut;//取款时身上金币增加
            userInfo->i64Bank -= pUserSit->i64CheckOut;
        }

        UserInfoModule()->updateUser(userInfo);
    }
}

void HNRoomLogic::startHeartBeat()
{
    isNeedReLogin = false;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(HNRoomLogic::heartBeat), this, 2.0f, kRepeatForever, 0.0f, false);
}

void HNRoomLogic::stopHeartBeat() {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNRoomLogic::heartBeat), this);
}

void HNRoomLogic::heartBeat(float dt) {
    if (isNeedReLogin) {
        onDisConnect();
        return;
    }

    if(isStartCheck) {
        time_t currTime = time(NULL);
        if (difftime(currTime ,receive_time) > HEARTBEAT_INTERVAL) {
            //if (_socketLogic->getGameHeartBeatCount() > MAX_HEARTBEAT_COUNT) {
                //_socketLogic->resumeGameHearBeatCount();
                //stopHeartBeat();

				reconnect();

                return;
            //}

            //if (connected()) {
            //    _socketLogic->addGameHearBeatCount();
            //}
        }
        else {
            if (connected()) {
                _socketLogic->resumeGameHearBeatCount();
            }
        }
    }
}

}

void HNRoomLogic::reconnect(bool bPlatfrom)
{
	stopHeartBeat();
	if (!_reconnect) _reconnect_time = time(NULL);
	if(PlatformLogic()->connected() && bPlatfrom){
		_reconnect_time = time(NULL);
		closeRoom(true);
		reconnectRoom();
	}
	else
	{
		PlatformLogic()->reconnect();
	}
	_reconnect = true;

	time_t currTime = time(NULL);
	if (difftime(currTime, _reconnect_time) > RECONNECT_TIME*2)
	{
		reconnectTimeOut();
	}
	else
	{
		CommandManager::getInstance()->execute(ADD_LOADER_COM);
	}
}

bool HNRoomLogic::reconnectRoom()
{
	return _socketLogic->openWithIp(_serverIp.c_str(), _serverPort);
}

void HN::HNRoomLogic::reconnectTimeOut()
{
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	// 分发消息到房间
	dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool {
		delegate->onDisConnect();
		return false;
	});

	// 分发消息到游戏
	dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool {
		delegate->onDisConnect();
		return false;
	});

	PlatformLogic()->closePlatform();
}


