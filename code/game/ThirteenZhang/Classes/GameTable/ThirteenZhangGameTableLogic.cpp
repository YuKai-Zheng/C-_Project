#include "ThirteenZhangGameTableLogic.h"
#include "ThirteenZhangGameTableUICallback.h"
//#include "HNNetExport.h"
//#include "HNLobbyExport.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/ExtraReward.h"
//using namespace HN;

namespace ThirteenZhang {
const char* gCardShapeName[] = {
    "未知",
    "高牌",
    "一对",
    "两对",
    "三条",
    "顺子",
    "同花",
    "葫芦",
    "四条",
    "同花顺",
};

GameTableLogic::GameTableLogic(IGameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate)
    : HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, uiCallback)
    , _gameUICallback(uiCallback)
    , _bSuper(false)
    , _superStation(-1) {
}

GameTableLogic::~GameTableLogic() {

}

void GameTableLogic::loadDeskUsersUI() {

    _deskUserList->transform([&](UserInfoStruct* user, int Index) {
        if (nullptr != user) {
            bool self = (user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
            _gameUICallback->onAddPlayer(user->bDeskStation, user->dwUserID, self);
        }
    });
}

void GameTableLogic::clearDesk() {

}

void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
    switch(messageHead->bAssistantID) {
    //case S_C_NOTICE_ROB: //通知抢庄
    //{
    //    CHECK_SOCKET_DATA(S_C_TNoticeRobNt, objectSize, "S_C_TNoticeRobNt size error!");
    //    S_C_TNoticeRobNt* pTNoticeRobNt = (S_C_TNoticeRobNt*)(object);
    //
    //    //当前抢庄玩家
    //    TableData.m_byCurrRobUser = pTNoticeRobNt->byCurrRobDesk;
    //    //玩家状态
    //    memcpy(TableData.m_EnUserState, pTNoticeRobNt->iUserState, sizeof(TableData.m_EnUserState));
    //    //游戏状态
    //    _gameStatus = GS_ROB_NT;
    //
    //    bool self = (_mySeatNo == pTNoticeRobNt->byCurrRobDesk);
    //    _uiCallback->onGameNoticeRobNt(pTNoticeRobNt->byCurrRobDesk, self);
    //
    //} break;
    //case S_C_USER_ROB_RESULT://玩家抢庄结果
    //{
    //    CHECK_SOCKET_DATA(S_C_UserRobNt_Result, objectSize, "S_C_UserRobNt_Result size error!");
    //    S_C_UserRobNt_Result* pRobNtResult = (S_C_UserRobNt_Result*)(object);
    //
    //    bool self = (_mySeatNo == pRobNtResult->byDeskStation);
    //    _uiCallback->onGameRobNtResult(pRobNtResult->byDeskStation, pRobNtResult->byRobResult, self);
    //
    //} break;
//            case S_C_MAKE_SUER_NT: //确定庄家
//            {
//                CHECK_SOCKET_DATA(S_C_MakeSure_NtStation, objectSize, "S_C_MakeSure_NtStation size error!");
//                S_C_MakeSure_NtStation* pNtStation = (S_C_MakeSure_NtStation*)(object);
//
//                TableData.m_byNowNtStation = pNtStation->byNowNtStation;
//
//                bool self = (_mySeatNo == pNtStation->byNowNtStation);
//                _uiCallback->onGameMakeSureNt(TableData.m_byNowNtStation, self);
//            } break;
        case S_C_PLAY_BEGIN_ANIMATION: {
		    CHECK_SOCKET_DATA(S_C_PlayBeginAnimation, objectSize, "S_C_SendCard size error!");
			S_C_PlayBeginAnimation* pPlayBeginAni = (S_C_PlayBeginAnimation*)(object);
			bool isCanPlay = pPlayBeginAni->canPlay;
			_gameUICallback->onIsCanPlayBeginAni(isCanPlay);
        }
		break;
    case S_C_SEND_CARD: { //发牌消息
        CHECK_SOCKET_DATA(S_C_SendCard, objectSize, "S_C_SendCard size error!");
        S_C_SendCard* pSendCard = (S_C_SendCard*)(object);

        //游戏状态
        _gameStatus = GS_SEND_CARD;

        //当前庄家位置
//                TableData.m_byNowNtStation = pSendCard->byNowNtStation;
        //玩家牌数据
        memcpy(TableData.m_byUserCards[_mySeatNo], pSendCard->byUserCards, sizeof(TableData.m_byUserCards[_mySeatNo]));
        //玩家状态
        memcpy(TableData.m_EnUserState, pSendCard->iUserState, sizeof(TableData.m_EnUserState));
        //特殊牌型信息
        memcpy(TableData.m_iSpecialShape, pSendCard->iSpecialShape, sizeof(TableData.m_iSpecialShape));

        std::vector<int> cardsShape;
        for (size_t i = 0; i < PLAY_COUNT; i++) {
            cardsShape.push_back(pSendCard->iSpecialShape[i]);
        }
        std::vector<BYTE> cards;
        for (size_t i = 0; i < USER_CARD_COUNT; i++) {
            if (pSendCard->byUserCards[i] > 0x00) {
                cards.push_back(pSendCard->byUserCards[i]);
            }
        }
        if (!cards.empty()) {
            _gameUICallback->onGameHasSpecialCard(cardsShape, cards);
        }

    }
    break;
    case S_C_NOTICE_OPEN_CARD: { //通知开牌消息
        CHECK_SOCKET_DATA(S_C_NoticeOpenCard, objectSize, "S_C_NoticeOpenCard size error!");
        S_C_NoticeOpenCard* pNoticeOpenCard = (S_C_NoticeOpenCard*)(object);
        //游戏状态
        _gameStatus = GS_OPEN_CARD;
        //玩家状态
        memcpy(TableData.m_EnUserState, pNoticeOpenCard->iUserState, sizeof(TableData.m_EnUserState));

        //排序牌数据
        CUpGradeGameLogic gameLogic;
        gameLogic.SortCard(TableData.m_byUserCards[_mySeatNo], NULL, USER_CARD_COUNT);

        _gameUICallback->onGameNoticeOpenCard();
    }
    break;
    case S_C_USER_OPEN_CARD_RESULT: { //玩家开牌结果
        CHECK_SOCKET_DATA(C_S_UserOpenCard_Result, objectSize, "C_S_UserOpenCard_Result size error!");
        C_S_UserOpenCard_Result* pOpenCardResult = (C_S_UserOpenCard_Result*)(object);

        //玩家位置
        BYTE seatNo = pOpenCardResult->byDeskStation;
        //玩家状态
        memcpy(TableData.m_EnUserState, pOpenCardResult->iUserState, sizeof(TableData.m_EnUserState));
        //每一堆牌数据
        memcpy(TableData.m_byHeapCard[seatNo], pOpenCardResult->byHeapCard, sizeof(TableData.m_byHeapCard[seatNo]));
        //玩家的开牌状态（是否使用特殊牌开牌）
        memcpy(TableData.m_iUseSpecialShape, pOpenCardResult->iUseSpecialShape, sizeof(TableData.m_iUseSpecialShape));

        _gameUICallback->onGameOpenCardResult(seatNo);

    }
    break;
    case S_C_USER_MANUAL_OPEN_CARD_RESULT: {
        CHECK_SOCKET_DATA(C_S_UserOpenCard, objectSize, "C_S_UserOpenCard size error!");
        C_S_UserOpenCard* pOpenCardResult = (C_S_UserOpenCard*)(object);
        //BYTE seatNo = pOpenCardResult->byDeskStation;
        if (_mySeatNo == pOpenCardResult->byDeskStation) {
            _gameUICallback->onGameManualOpenCard(object);
        }
    }
    break;
    case S_C_COMPARE_CARD: { //比牌消息
        CHECK_SOCKET_DATA(S_C_CompareCard, objectSize, "S_C_CompareCard size error!");
        S_C_CompareCard* pCompareCard = (S_C_CompareCard*)(object);

        _gameStatus = GS_COMPARE_CARD;
        //每一堆牌数据
        memcpy(TableData.m_byHeapCard, pCompareCard->byHeapCard, sizeof(TableData.m_byHeapCard));
        //牌型
        memcpy(TableData.m_iHeapShape, pCompareCard->iHeapShape, sizeof(TableData.m_iHeapShape));
        //比牌结果
        memcpy(TableData.m_iComepareResult, pCompareCard->iComepareResult, sizeof(TableData.m_iComepareResult));
        //玩家状态
        memcpy(TableData.m_EnUserState, pCompareCard->iUserState, sizeof(TableData.m_EnUserState));
        for (int i = 0; i < PLAY_COUNT; i++) {
            TableData.m_iUseSpecialShape[i] = !pCompareCard->iCommonCompare[i];//如果玩家使用普通比牌，则不使用特殊牌
        }
        _gameUICallback->onGameCompareCard(TableData.m_byHeapCard);

    }
    break;
    case S_C_OPEN_FIRE: { //打枪消息
        CHECK_SOCKET_DATA(S_C_OpenFire, objectSize, "S_C_OpenFire size error!");
        S_C_OpenFire* pOpenFire = (S_C_OpenFire*)(object);

        _gameUICallback->onGameOpenFire(pOpenFire);

    }
    break;
    case S_C_OPEN_SPECIAL_SHAPE: {
        CHECK_SOCKET_DATA(S_C_OpenSpecialShape, objectSize, "S_C_OpenSpecialShape size error!");
        S_C_OpenSpecialShape* pSpecialShape = (S_C_OpenSpecialShape*)object;

        _gameUICallback->onGameOpenSpecialShape(pSpecialShape);
    }
    break;
    case S_C_GAME_END: { //结算消息
        CHECK_SOCKET_DATA(S_C_GameResult, objectSize, "S_C_GameResult size error!");
        S_C_GameResult* pGameResult = (S_C_GameResult*)(object);

        //保存数据
        memcpy(TableData.m_i64UserFen, pGameResult->i64UserFen, sizeof(TableData.m_i64UserFen));
        memcpy(TableData.m_i64UserMoney, pGameResult->i64UserMoney, sizeof(TableData.m_i64UserMoney));

        _gameUICallback->onGameResult(pGameResult);

    }
    break;
    case S_C_IS_SUPER_USER: {	//超端验证消息
        CHECK_SOCKET_DATA(S_C_SuperUser, objectSize, "S_C_SuperUser size error!");
        S_C_SuperUser* pSuperUser = (S_C_SuperUser*)(object);

        _bSuper = pSuperUser->bIsSuper;
        _superStation = pSuperUser->byDeskStation;

        break;
    }
    case S_C_FORCEEXIT_LESSMONEY: {
        CHECK_SOCKET_DATA(S_C_ForceExit_LessMonye, objectSize, "S_C_ForceExit_LessMonye size error!");
        S_C_ForceExit_LessMonye* forceExit = (S_C_ForceExit_LessMonye*)object;

        if (forceExit->bIsExit == 1) {
            //金币不足，弹窗并提醒退出
            _gameUICallback->IShowTipExit(forceExit->i64Money);
        }
    }
    break;
    case NN::S2C_NOTIFY_MSG: {
        CCAssert(sizeof(NN::tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
        if (sizeof(NN::tag_s2c_notify) != objectSize)	return;

        NN::tag_s2c_notify* msg = (NN::tag_s2c_notify*)object;
        _gameUICallback->onGameNotifyWuHuaNiu(msg);
    }
    break;
	case ASS_NOTIFYVIPUSER_WINLOSE: {
		CCAssert(sizeof(VipUserWinLoseRlt) == objectSize, "sizeof(VipUserWinLoseRlt) != objectSize");
		if (sizeof(VipUserWinLoseRlt) != objectSize)	return;

		VipUserWinLoseRlt* msg = (VipUserWinLoseRlt*)object;
		_gameUICallback->onGameResultTag(msg->nWinLose);
	}
	break;
	case ASS_NOTIFYVIPUSER_GAMELOG:
	{
		CCAssert(sizeof(VipRoomPlayerGameLogMsg) == objectSize, "sizeof(VipRoomPlayerGameLogMsg) != objectSize");
		if (sizeof(VipRoomPlayerGameLogMsg) != objectSize)	return;

		VipRoomPlayerGameLogMsg* msg = (VipRoomPlayerGameLogMsg*)object;
		_gameUICallback->onVipGameResultLog(msg->mUserLog);
	}
	break;
        case ASS_SHOW_LUCKYMONEY:
        {
            // 出现红包
            if (objectSize != sizeof(LuckyMoneyNotify))
            {
                CCASSERT(false, "sizeof(LuckyMoneyNotify) != objectSize");
                return;
            }
            
            auto msg = (LuckyMoneyNotify*)object;
            _gameUICallback->onLuckyMoneyNotify(msg);
        }
            break;
        case ASS_LUCKYMONEY_RESULT:
        {
            // 打开红包结果
            if (objectSize != sizeof(LuckyMoneyResult))
            {
                CCASSERT(false, "sizeof(LuckyMoneyResult) != objectSize");
                return;
            }
            
            auto msg = (LuckyMoneyResult*)object;
            _gameUICallback->onLuckyMoneyResult(msg);
        }
            break;
//    //年兽
//    case NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG :
//    {
//        //年兽出现 150
//        _uiCallback->onGameShowNianShou();
//    }
//    break;
//    case NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT:
//    {
//        //赶年兽 151
//        CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//        
//        NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//        _uiCallback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//    }
//    break;
//    case NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG:
//    {
//        //对局获得鞭炮 152
//        CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//        
//        NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//        _uiCallback->onGameMatchGainFirecraker(msg->nUserID);
//    }
//    break;
            
    default:
        HNLOG_ERROR("unknow game command.");
        break;
    }
}

void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree) {
    bool self = (_mySeatNo == agree->bDeskStation);
    _gameUICallback->onGameAgree(agree->bDeskStation, self);
}

void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    bool self = (_mySeatNo == userSit->bDeskStation);
    _gameUICallback->onAddPlayer(userSit->bDeskStation, userSit->dwUserID, self);
}

void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    bool self = (userSit->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
    if (self) {
        stop();
        _gameUICallback->forceLeaveTheGame();
    } else {
        _gameUICallback->onRemovePlayer(userSit->bDeskStation, userSit->dwUserID, self);
    }
}

void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) {
    HNGameLogicBase::dealGameInfoResp(pGameInfo);

}

void GameTableLogic::dealGameStationResp(void* object, INT objectSize) {
    switch(_gameStatus) {
    case GS_WAIT_SETGAME: //等待东家设置状态
    case GS_WAIT_AGREE:	//等待同意设置
    case GS_WAIT_NEXT: {	//等待下一盘开始
        CHECK_SOCKET_DATA(S_C_TGameStation_Free, objectSize, "S_C_TGameStation_Free size error!");
        S_C_TGameStation_Free* pGameStation = (S_C_TGameStation_Free*)(object);

        TableData.m_byBeginTime = pGameStation->byBeginTime;
        TableData.m_byRobTime = pGameStation->byRobTime;
        TableData.m_bySendCardTime = pGameStation->bySendCardTime;
        TableData.m_byThinkTime = pGameStation->byThinkTime;
        TableData.m_byCompareTime = pGameStation->byCompareTime;
        TableData.m_byCountTime = pGameStation->byCountTime;
        TableData.m_iBaseFen = pGameStation->iBaseFen;
        _gameUICallback->onGameDizhuValue(TableData.m_iBaseFen);

        for (size_t i = 0; i < PLAY_COUNT; i++) {
            bool ready = pGameStation->bUserReady[i];
            HNLOG_INFO("GS_WAIT_SETGAME || GS_WAIT_AGREE || GS_WAIT_NEXT UserReady = %d", ready);
        }

        _gameUICallback->onGSFree(pGameStation->bUserReady);
        HNLOG_ERROR("GS_WAIT_SETGAME || GS_WAIT_AGREE || GS_WAIT_NEXT");
    }
    break;
    //case GS_ROB_NT:		//抢庄状态
    //{
    //    CHECK_SOCKET_DATA(S_C_GameStation_RobNt, objectSize, "S_C_GameStation_RobNt size error!");
    //    S_C_GameStation_RobNt* pGameStation = (S_C_GameStation_RobNt*)(object);
    //
    //    TableData.m_byBeginTime = pGameStation->byBeginTime;
    //    TableData.m_byRobTime = pGameStation->byRobTime;
    //    TableData.m_bySendCardTime = pGameStation->bySendCardTime;
    //    TableData.m_byThinkTime = pGameStation->byThinkTime;
    //    TableData.m_byCompareTime = pGameStation->byCompareTime;
    //    TableData.m_byCountTime = pGameStation->byCountTime;
    //    TableData.m_iBaseFen = pGameStation->iBaseFen;
    //
    //    //剩余时间
    //    TableData.m_byRemaindTime = pGameStation->byRemaindTime;
    //    //当前操作玩家
    //    TableData.m_byCurrRobUser = pGameStation->byCurrRobUser;
    //
    //    // 玩家状态
    //    memcpy(TableData.m_EnUserState, pGameStation->iUserState, sizeof(TableData.m_EnUserState));
    //
    //    _uiCallback->onGSRobNt(pGameStation->byCurrRobUser);
    //    HNLOG_DEBUG("GS_ROB_NT");
    //} break;
    case GS_SEND_CARD: {	//发牌状态
        CHECK_SOCKET_DATA(S_C_GameStation_SendCard, objectSize, "S_C_GameStation_SendCard size error!");
        S_C_GameStation_SendCard* pGameStation = (S_C_GameStation_SendCard*)(object);

        TableData.m_byBeginTime = pGameStation->byBeginTime;
        TableData.m_byRobTime = pGameStation->byRobTime;
        TableData.m_bySendCardTime = pGameStation->bySendCardTime;
        TableData.m_byThinkTime = pGameStation->byThinkTime;
        TableData.m_byCompareTime = pGameStation->byCompareTime;
        TableData.m_byCountTime = pGameStation->byCountTime;
        TableData.m_iBaseFen = pGameStation->iBaseFen;
        _gameUICallback->onGameDizhuValue(TableData.m_iBaseFen);

        //庄家位置
//                TableData.m_byNowNtStation = pGameStation->byNowNtStation;
        //剩余时间
        TableData.m_byRemaindTime = pGameStation->byRemaindTime;

        //玩家状态
        memcpy(TableData.m_EnUserState, pGameStation->iUserState, sizeof(TableData.m_EnUserState));

        //正常的玩家就显示操作容器 中途加入的不能显示
        if (STATE_ERR != TableData.m_EnUserState[_mySeatNo]) {
            CUpGradeGameLogic gameLogic;
            gameLogic.SortCard(pGameStation->byUserCards, NULL, USER_CARD_COUNT);
        }

        //玩家的牌数据
        memcpy(TableData.m_byUserCards[_mySeatNo], pGameStation->byUserCards, sizeof(TableData.m_byUserCards[_mySeatNo]));

        std::vector<BYTE> cards;
        for (size_t i = 0; i < USER_CARD_COUNT; i++) {
            BYTE val = pGameStation->byUserCards[i];
            if (0 == val) continue;
            if (0xFF == val) continue;
            cards.push_back(val);
        }
        _gameUICallback->onGSSendCard(cards);
        HNLOG_DEBUG("GS_SEND_CARD");
    }
    break;
    case GS_OPEN_CARD: {	//摆牌状态
        CHECK_SOCKET_DATA(S_C_GameStation_OpenCard, objectSize, "S_C_GameStation_OpenCard size error!");
        S_C_GameStation_OpenCard* pGameStation = (S_C_GameStation_OpenCard*)(object);

        TableData.m_byBeginTime = pGameStation->byBeginTime;
        TableData.m_byRobTime = pGameStation->byRobTime;
        TableData.m_bySendCardTime = pGameStation->bySendCardTime;
        TableData.m_byThinkTime = pGameStation->byThinkTime;
        TableData.m_byCompareTime = pGameStation->byCompareTime;
        TableData.m_byCountTime = pGameStation->byCountTime;
        TableData.m_iBaseFen = pGameStation->iBaseFen;
        _gameUICallback->onGameDizhuValue(TableData.m_iBaseFen);

        //庄家位置
//                TableData.m_byNowNtStation = pGameStation->byNowNtStation;
        //剩余时间
        TableData.m_byRemaindTime = pGameStation->byRemaindTime;

        //玩家状态
        memcpy(TableData.m_EnUserState, pGameStation->iUserState, sizeof(TableData.m_EnUserState));
        //玩家的牌数据
        memcpy(TableData.m_byUserCards[_mySeatNo], pGameStation->byUserCards, sizeof(TableData.m_byUserCards[_mySeatNo]));
        //玩家摆牌数据
        memcpy(TableData.m_byHeapCard[_mySeatNo], pGameStation->byMyHeapCard, sizeof(TableData.m_byHeapCard[_mySeatNo]));

        std::vector<BYTE> cards;
        for (size_t i = 0; i < USER_CARD_COUNT; i++) {
            BYTE val = pGameStation->byUserCards[i];
            if (0 == val) continue;
            if (0xFF == val) continue;
            cards.push_back(val);
        }

        /*for (size_t j = 0; j < 3; j++)
         {
         for (size_t k = 0; k < 5; k++)
         {
         BYTE val = pGameStation->byMyHeapCard[j][k];
         HNLOG_INFO("GS_OPEN_CARD i = %d, j = %d, c = %02X", _mySeatNo, _mySeatNo, val);
         }
         }*/

        _gameUICallback->onGSOpenCard(cards, pGameStation->byMyHeapCard);
        HNLOG_DEBUG("GS_OPEN_CARD");
    }
    break;
    case GS_COMPARE_CARD: { //比牌阶段
        CHECK_SOCKET_DATA(S_C_GameStation_CompareCard, objectSize, "S_C_GameStation_CompareCard size error!");
        S_C_GameStation_CompareCard* pGameStation = (S_C_GameStation_CompareCard*)(object);

        TableData.m_byBeginTime = pGameStation->byBeginTime;
        TableData.m_byRobTime = pGameStation->byRobTime;
        TableData.m_bySendCardTime = pGameStation->bySendCardTime;
        TableData.m_byThinkTime = pGameStation->byThinkTime;
        TableData.m_byCompareTime = pGameStation->byCompareTime;
        TableData.m_byCountTime = pGameStation->byCountTime;
        TableData.m_iBaseFen = pGameStation->iBaseFen;
        _gameUICallback->onGameDizhuValue(TableData.m_iBaseFen);

        //庄家位置
//                TableData.m_byNowNtStation = pGameStation->byNowNtStation;
        //剩余时间
        TableData.m_byRemaindTime = pGameStation->byRemaindTime;

        //玩家状态
        memcpy(TableData.m_EnUserState, pGameStation->iUserState, sizeof(TableData.m_EnUserState));
        //玩家摆牌数据
        memcpy(TableData.m_byHeapCard, pGameStation->byHeapCard, sizeof(TableData.m_byHeapCard));
        //牌型
        memcpy(TableData.m_iHeapShape, pGameStation->iHeapShape, sizeof(TableData.m_iHeapShape));
        //比牌结果
        memcpy(TableData.m_iComepareResult, pGameStation->iComepareResult, sizeof(TableData.m_iComepareResult));

        //for (size_t i = 0; i < PLAY_COUNT; i++)
        //{
        //	for (size_t j = 0; j < 3; j++)
        //	{
        //		for (size_t k = 0; k < 5; k++)
        //		{
        //			BYTE val = pGameStation->byHeapCard[i][j][k];
        //			HNLOG_INFO("GS_COMPARE_CARD i = %d, j = %d, c = %02X", i, j, val);
        //		}
        //	}
        //}

        _gameUICallback->onGSCompareCard(TableData.m_byHeapCard);
        HNLOG_DEBUG("GS_COMPARE_CARD");
    }
    break;
    default:
        HNLOG_ERROR("unknow GS.");
        break;
    }
}

void GameTableLogic::dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk) {
    _gameUICallback->onUserChat(normalTalk->dwSendID, normalTalk->dwTargetID, std::string(normalTalk->szMessage));
}

void GameTableLogic::sendForceQuit(const ccExitCallback& callback) {
//    leaveTheGame();
}
    
//    void GameTableLogic::leaveTheGame() {
//        bool connected = RoomLogic()->connected();
//        if (connected) {
//            HNGameLogicBase::sendUserUp();
//        }
//        else {
//            HNGameLogicBase::sendForceQuit();
//        }
//    }

    void GameTableLogic::sendDissolveRoom(BYTE bDeskIndex) {
        MSG_GR_S_DissolveVipRoom_Ask dissolve;
        dissolve.bDeskIndex = bDeskIndex;
        HNRoomLogic::getInstance()->sendData(MDM_GR_USER_ACTION, ASS_GR_DISSOLVE_VIPROOM, &dissolve, sizeof(MSG_GR_S_DissolveVipRoom_Ask));
    }
    
void GameTableLogic::sendUserUp(const ccExitCallback& callback) {
    _exitCB = callback;

    bool connected = RoomLogic()->connected();
    if (connected) {
        HNGameLogicBase::sendUserUp();
    } else {
        _exitCB(connected);
    }
}

const char* GameTableLogic::getCardTypeName(BYTE type) {
    return gCardShapeName[type];
}

//    void GameTableLogic::sendUserRobNt(bool rob)
//    {
//        C_S_UserRobNt UserRobNt;
//        UserRobNt.byDeskStation = _mySeatNo;
//        UserRobNt.byRob = rob;
//        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_USER_ROB, &UserRobNt, sizeof(UserRobNt));
//    }

void GameTableLogic::sendOpenCard(INT heapCount[3], BYTE heapCard[3][5]) {
    C_S_UserOpenCard UserOpenCard;
    UserOpenCard.byDeskStation = _mySeatNo;
    memcpy(UserOpenCard.iHeapCount, heapCount, sizeof(UserOpenCard.iHeapCount));
    memcpy(UserOpenCard.byHeapCard, heapCard, sizeof(UserOpenCard.byHeapCard));
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_USER_OPEN_CARD, &UserOpenCard, sizeof(UserOpenCard));
}

void GameTableLogic::requestManualOpenCard() {
    C_S_MANUAL_UserOpenCard manualOpenCard;
    manualOpenCard.byDeskStation = _mySeatNo;
    manualOpenCard.isManualOpenCard = true;
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_USER_MANUAL_OPEN_CARD, &manualOpenCard, sizeof(manualOpenCard));
}

void GameTableLogic::requestOpenWithSpecialShape(int specialShape, bool useShape) {
    C_S_OpenSpecialCard iSpecialShape;
    iSpecialShape.byDeskStation = _mySeatNo;
    iSpecialShape.iSpecialShape = specialShape;
    iSpecialShape.iOpenBySpecialShape = useShape;
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_OPEN_SPECIAL_CARD, &iSpecialShape, sizeof(iSpecialShape));
}
    
    void GameTableLogic::sendDriveNianShou(int nUserID)
    {
//        NewYearActivityInGame::GanNianShouRequest msg;
//        msg.nUserID = nUserID;
//        //0活动1游戏
//        msg.bSceneType = 1;
//        
//        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, NewYearActivityInGame::C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
    }
    

}
