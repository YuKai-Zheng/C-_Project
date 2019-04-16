#include "BRXJGameLogic.h"
#include "platform/CCPlatformMacros.h"
#include "HNLobby/v66/ExtraReward.h"

namespace BRXJ {
// 构造函数
GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool autoCreate)
    :HNGameLogicBase(deskNo, PLAY_COUNT, autoCreate, uiCallback) {
    _gameUICallback = uiCallback;
}

// 析构函数
GameTableLogic::~GameTableLogic() {
    _gameUICallback = nullptr;
}
// 同意开始
void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree) {
    CCLOG("=========dealUserAgreeResp!!!!!!!!!!!=============");

}

// 游戏开始
void GameTableLogic::dealGameStartResp(BYTE bDeskNO) {
    CCLOG("=========dealGameStartResp!!!!!!!!!!!=============");


}

// 玩家坐下
void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, const UserInfoStruct* user) {
    CCLOG("=========dealUserSitResp!!!!!!!!!!!=============");
}

// 玩家站起
void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    CCLOG("=========dealUserUpResp!!!!!!!!!!!=============");
    if (_mySeatNo == user->bDeskNO) {
        stop();
        _gameUICallback->levelGame();
    }
}

// 游戏信息
void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) {
    _gameStatus = pGameInfo->bGameStation;
}

// 游戏状态
void GameTableLogic::dealGameStationResp(void* object, INT objectSize) {
    CCLOG("=========dealGameStationResp!!!!!!!!!!!=============");
    TGameStationBase* pData = (TGameStationBase*)object;
    _i64UserMaxNote = pData->i64UserMaxNote;
    //状态

    _gameUICallback->grayEffectBet(false);

    switch (_gameStatus) {
    case GS_WAIT_SETGAME:
    case GS_WAIT_AGREE:
    case GS_WAIT_NEXT: {	//等待下盘开始
        CCLOG("GS_WAIT_NEXT_C = %d  Server = %d  ", sizeof(TGameStationBase), objectSize);
        CCAssert(sizeof(TGameStationBase) == objectSize, "size error.");
        TGameStationBase* pData = (TGameStationBase*)object;
        //时间段
        _iFreeTime = pData->iFreeTime;
        _iKaiPaiTime = pData->iKaiPaiTime;
        _iXiaZhuTime = pData->iXiaZhuTime;
        _iShowWinTime = pData->iShowWinTime;

        _i64ShangZhuangLimit = pData->i64ShangZhuangLimit;
        _i64UserMaxNote = pData->i64UserMaxNote;
        //庄
        _iNtStation = pData->iNtStation;
        _iNowNtStation = pData->iNtStation;
        _iNtPlayCount = pData->iNtPlayCount;
        _i64NtWinMoney = pData->i64NtWinMoney;
        _i64NtMoney = pData->i64NtMoney;

        //筹码
        memcpy(_i64ChouMaValues, pData->i64ChouMaValues, sizeof(_i64ChouMaValues));
        //自己
        _i64UserWin = pData->i64UserWin;
        _i64MyMoney = pData->i64MyMoney;
        _i64RemainBetNum = pData->i64RemainBetNum;
        _i64RemainBetNumTwo = pData->i64RemainBetNum;


        memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));
        memcpy(_tGameLuzi, pData->tGameLuzi, sizeof(_tGameLuzi));

        _gameUICallback->clearTable(false);

        //开计时器
        if (_iNtStation != -1)
            _gameUICallback->starTime(_iFreeTime);





        break;
    }
    case GS_NOTE_STATE: {	//下注
        CCAssert(sizeof(TGameStationBase_Note) == objectSize, "size error.");
        TGameStationBase_Note* pData = (TGameStationBase_Note*)object;
        //时间段
        _iFreeTime = pData->iFreeTime;
        _iKaiPaiTime = pData->iKaiPaiTime;
        _iXiaZhuTime = pData->iXiaZhuTime;
        _iShowWinTime = pData->iShowWinTime;

        _i64ShangZhuangLimit = pData->i64ShangZhuangLimit;
        _i64UserMaxNote = pData->i64UserMaxNote;

        //庄
        _iNtStation = pData->iNtStation;
        _iNowNtStation = pData->iNtStation;
        _iNtPlayCount = pData->iNtPlayCount;
        _i64NtWinMoney = pData->i64NtWinMoney;
        _i64NtMoney = pData->i64NtMoney;

        //筹码
        memcpy(_i64ChouMaValues, pData->i64ChouMaValues, sizeof(_i64ChouMaValues));

        //自己
        _i64UserWin = pData->i64UserWin;
        _i64MyMoney = pData->i64MyMoney;
        _i64RemainBetNum = pData->i64RemainBetNum;
        _i64RemainBetNumTwo = pData->i64RemainBetNum;

        memcpy(_i64QuYuZhuTrue, pData->i64QuYuZhuTrue, sizeof(_i64QuYuZhuTrue));
        memcpy(_i64QuYuZhu, pData->i64QuYuZhu, sizeof(_i64QuYuZhu));
        memcpy(_i64UserXiaZhu, pData->i64UserXiaZhu, sizeof(_i64UserXiaZhu));

        memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));
        memcpy(_tGameLuzi, pData->tGameLuzi, sizeof(_tGameLuzi));

        //开计时器
        _gameUICallback->starTime(_iXiaZhuTime);

        _gameUICallback->clearTable(true);

        _gameUICallback->upDataAreaMoney(_i64UserXiaZhu);
        _gameUICallback->upDataAreaTotleMoney(_i64QuYuZhu);

        _gameUICallback->upDataNTInfo(_i64NtMoney, _i64NtWinMoney);
        _gameUICallback->upDataPlayerInfo(_i64MyMoney, _i64UserWin);

        _gameUICallback->grayEffectBet(true);

        break;
    }
    case GS_SEND_CARD: {	//发牌
        CCAssert(sizeof(TGameStationBase_SendCard) == objectSize, "size error.");
        TGameStationBase_SendCard* pData = (TGameStationBase_SendCard*)object;

        TGameStationBase_SendCard	TGameStation;
        _iFreeTime = pData->iFreeTime;
        _iKaiPaiTime = pData->iKaiPaiTime;
        _iXiaZhuTime = pData->iXiaZhuTime;
        _iShowWinTime = pData->iShowWinTime;
        _i64ShangZhuangLimit = pData->i64ShangZhuangLimit;
        _i64UserMaxNote = pData->i64UserMaxNote;

        _iNtStation = pData->iNtStation;
        _iNowNtStation = pData->iNtStation;
        _iNtPlayCount = pData->iNtPlayCount;
        _i64NtWinMoney = pData->i64NtWinMoney;
        memcpy(_i64ChouMaValues, pData->i64ChouMaValues, sizeof(_i64ChouMaValues));

        //非空闲状态，或者五庄等待状态，铁定应该有庄家的
        _i64NtMoney = pData->i64NtMoney;
        //自己的输赢情况
        _i64UserWin = pData->i64UserWin;
        //自己的金币数
        _i64MyMoney = pData->i64MyMoney;

        _i64RemainBetNum = pData->i64RemainBetNum;
        _i64RemainBetNumTwo = pData->i64RemainBetNum;

        //下注信息
        memcpy(_i64QuYuZhuTrue, pData->i64QuYuZhuTrue, sizeof(TGameStation.i64QuYuZhuTrue));
        memcpy(_i64QuYuZhu, pData->i64QuYuZhu, sizeof(_i64QuYuZhu));
        memcpy(_i64UserXiaZhu, pData->i64UserXiaZhu, sizeof(_i64UserXiaZhu));

        //牌数据
        memcpy(_byCardCount, pData->byCardCount, sizeof(_byCardCount));
        memcpy(_byUserCard, pData->byUserCard, sizeof(_byUserCard));
        memcpy(_iCardShape, pData->iCardShape, sizeof(_iCardShape));

        //上庄列表
        memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));
        //路子信息
        memcpy(_tGameLuzi, pData->tGameLuzi, sizeof(_tGameLuzi));

        //开计时器
        _gameUICallback->starTime(_iKaiPaiTime);

        _gameUICallback->clearTable(true);

        _gameUICallback->upDataAreaMoney(_i64UserXiaZhu);
        _gameUICallback->upDataAreaTotleMoney(_i64QuYuZhu);

        _gameUICallback->upDataNTInfo(_i64NtMoney, _i64NtWinMoney);
        _gameUICallback->upDataPlayerInfo(_i64MyMoney, _i64UserWin);

        _gameUICallback->grayEffectBet(false);


        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 4; j++) {
                //获取牌值
                _gameUICallback->initCardValue(pData->byUserCard[i][j], i, j);
            }
        }

        _gameUICallback->ISendCard(false);

        break;
    }
    case GS_SHOW_WIN: {	//显示中奖区域
        CCAssert(sizeof(TGameStationBase_ShowWin) == objectSize, "size error.");
        TGameStationBase_ShowWin* pData = (TGameStationBase_ShowWin*)object;

        _iFreeTime = pData->iFreeTime;
        _iKaiPaiTime = pData->iKaiPaiTime;
        _iXiaZhuTime = pData->iXiaZhuTime;
        _iShowWinTime = pData->iShowWinTime;
        _i64ShangZhuangLimit = pData->i64ShangZhuangLimit;
        _i64UserMaxNote = pData->i64UserMaxNote;

        _iNtStation = pData->iNtStation;
        _iNowNtStation = pData->iNtStation;
        _iNtPlayCount = pData->iNtPlayCount;
        _i64NtWinMoney = pData->i64NtWinMoney;
        memcpy(_i64ChouMaValues, pData->i64ChouMaValues, sizeof(_i64ChouMaValues));
        //非空闲状态，或者五庄等待状态，铁定应该有庄家的
        _i64NtMoney = pData->i64NtMoney;
        //自己的输赢情况
        _i64UserWin = pData->i64UserWin;
        //自己的金币数
        _i64MyMoney = pData->i64MyMoney;

        //下注信息
        memcpy(_i64QuYuZhuTrue, pData->i64QuYuZhuTrue, sizeof(_i64QuYuZhuTrue));
        memcpy(_i64QuYuZhu, pData->i64QuYuZhu, sizeof(_i64QuYuZhu));
        memcpy(_i64UserXiaZhu, pData->i64UserXiaZhu, sizeof(_i64UserXiaZhu));

        //牌数据
        memcpy(_byCardCount, pData->byCardCount, sizeof(_byCardCount));
        memcpy(_byUserCard, pData->byUserCard, sizeof(_byUserCard));
        memcpy(_iCardShape, pData->iCardShape, sizeof(_iCardShape));
        //上庄列表
        memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));
        //路子信息
        memcpy(_tGameLuzi, pData->tGameLuzi, sizeof(_tGameLuzi));

        //中奖区域信息
        memcpy(_iCompareResult, pData->iCompareResult, sizeof(_iCompareResult));

        _i64RemainBetNum = pData->i64RemainBetNum;
        _i64RemainBetNumTwo = pData->i64RemainBetNum;

        //开计时器
        _gameUICallback->starTime(_iShowWinTime);

        _gameUICallback->clearTable(true);

        _gameUICallback->upDataAreaMoney(_i64UserXiaZhu);
        _gameUICallback->upDataAreaTotleMoney(_i64QuYuZhu);

        _gameUICallback->upDataNTInfo(_i64NtMoney, _i64NtWinMoney);
        _gameUICallback->upDataPlayerInfo(_i64MyMoney, _i64UserWin);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                //获取牌值
                _gameUICallback->initCardValue(pData->byUserCard[i][j], i, j);
            }
        }

        _gameUICallback->ISendCard(false);


        for (int i = 0; i < 4; i++) {
            if (pData->iCompareResult[i] == 1) //刷中奖位置
                _gameUICallback->showWinArea(i);
            _gameUICallback->showCardShap(i, _iCardShape[i]);
        }

        break;
    }
    default:
        break;
    }


    _gameUICallback->upDataLuzi(pData->tGameLuzi);

    _gameUICallback->playEffectOnStation();

    //更新游戏状态
    _gameUICallback->gameStation(_gameStatus);
    if (_iNtStation != -1) {
        auto user = _deskUserList->getUserByDeskStation(_iNtStation);
        if (user) {
			_gameUICallback->upDataNTInfo(user->dwUserID,user->bLogoID, user->nickName, user->i64Money, _i64NtWinMoney, _iNtPlayCount);
            if (_mySeatNo == _iNtStation || isInNTList(_mySeatNo) == true) {
                _gameUICallback->IApplyNT(true);
            } else {
                _gameUICallback->IApplyNT(false);
            }
        }
    }
    auto me = getUserBySeatNo(_mySeatNo);
    if (me)
        _gameUICallback->upDataPlayerInfo(me->nickName, me->i64Money, _i64UserWin);
}

// 游戏消息
void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
    CCLOG("=========dealGameMessage!!!!!!!!!!!=============");
    switch (messageHead->bAssistantID) {
    case S_C_APPLY_ZHUANG_RESULT:	// 申请上庄结果 91
        HNLOG("dealApplyNTResp");
        dealApplyNTResp(object, objectSize);
        break;
    case S_C_XIA_ZHU_RESULT:	    // 下注结果 93
        HNLOG("dealBetResultResp");
        dealBetResultResp(object, objectSize);
        break;
    case S_C_NO_NT_WAITE:									// 无庄等待 100
        HNLOG("dealNoNTWaitResp");
        dealNoNTWaitResp(object, objectSize);
        break;
    case S_C_NOTICE_XIA_ZHU:							    // 游戏开始开始下注 101
        HNLOG("dealNoticeBetResp");
        dealNoticeBetResp(object, objectSize);
        break;
    case S_C_SEND_CARD:										// 开始发牌 102
        HNLOG("dealSendCardResp");
        dealSendCardResp(object, objectSize);
        break;
    case S_C_SHOW_WINAREA:								// 显示中奖区域 104
        HNLOG("dealShowWinAreaResp");
        dealShowWinAreaResp(object, objectSize);
        break;
    case S_C_GAME_END:										// 游戏结束结算消息 105
        HNLOG("dealGameEndResp");
        dealGameEndResp(object, objectSize);
        break;
    case S2C_TIP_EXIT_MSG:
        dealUserCutEnd(object, objectSize);
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
    //case S_C_TIP_EXIT_MSG:
    //{
    //						 dealGameTipExit(object, objectSize);
    //}
    case NN::S2C_NOTIFY_MSG: {
        CCAssert(sizeof(NN::tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
        if (sizeof(NN::tag_s2c_notify) != objectSize)	return;

        NN::tag_s2c_notify* msg = (NN::tag_s2c_notify*)object;
        _gameUICallback->onGameNotify(msg);
        break;
    }

//    //年兽
//    case NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG : {
//        //年兽出现 150
//        _tableUIcallBack->onGameShowNianShou();
//    }
//    break;
//    case NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT: {
//        //赶年兽 151
//        CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//
//        NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//        _tableUIcallBack->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//    }
//    break;
//    case NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG: {
//        //对局获得鞭炮 152
//        CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//
//        NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//        _tableUIcallBack->onGameMatchGainFirecraker(msg->nUserID);
//    }
//    break;

    default:
        HNLOG("AssistID:%u", messageHead->bAssistantID);
    }
}

/*-------------publish--------------------*/
// 请求站起
void GameTableLogic::requestStandUp() {
    sendUserUp();
}

// 请求下注
void GameTableLogic::requestBet(int betType, int betArea) {
    if (_mySeatNo != INVALID_DESKSTATION && _mySeatNo == _iNowNtStation) {
        _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("庄家不能下注,闲家才能下注!"));
        return;
    }

    if (_gameStatus != GS_NOTE_STATE) {
        _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("未到下注时间！"));
        return;
    } else if (betType < 0 || betType > 5) { //筹码5个
        _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("请选择下注类型！"));
        return;
    }

    LLONG betValue[] = { 1000, 10000, 100000, 1000000, 5000000,};
    if (_i64MyMoney < betValue[betType]) {
        _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("金币不足！"));
        return;
    }

    LLONG totleMoney = 0;
    for (int i = 0; i < 3; i++) {
        totleMoney += _i64QuYuZhu[i];
    }

    auto ban = (unsigned int)_i64NtMoney / 2;
    if (totleMoney >=(unsigned int)(ban / 1000) * 1000) {
        _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("超过本局最大下注!"));
        return;
    }

    if (betValue[betType] > _i64RemainBetNum) {
        _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("本次下注超过当区域剩余最大下注!"));
        return;
    }

    sendBet(_deskNo, betType, betArea);
}

void GameTableLogic::sendBet(BYTE deskNo, int betType, int betArea) {
    C_S_UserNote data;
    data.byDeskStation = deskNo;
    data.iChouMaType = betType;
    data.iNoteArea = betArea;

    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_XIA_ZHU, &data, sizeof(C_S_UserNote));
}

// 请求上庄、下庄
void GameTableLogic::requestBeNT(bool isNT) {
    char str[128] = { 0 };

    if (_i64MyMoney < _i64ShangZhuangLimit) {
        sprintf(str, GetText("BJL_Request_Banker_Failure"), _i64ShangZhuangLimit);
        _gameUICallback->IShowNoticeMessage(str);
        return;
    }

    // 申请上庄
    if (isNT) {
        if (_mySeatNo != INVALID_DESKSTATION && _mySeatNo == _iNtStation) {
            _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("您已经是庄家"));
            return;
        }

        for (int i = 0; i < PLAY_COUNT; i++) {
            if (_mySeatNo == byZhuangList[i]) {
                _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("您已在庄家列表中，请耐心等待"));
                return;
            }
        }

        sendBeNT(true);
    }

    // 申请下庄
    else {
        sendBeNT(false);
    }
}

// 申请断线重连，或者刚刚进入房间时申请恢复桌子数据包
void GameTableLogic::requestGameData() {
    CCLOG("=========requestGameData!!!!!!!!!!!=============");
}

// 请求庄家列表
vector<string> GameTableLogic::requestNTList() {
    std::vector<std::string> ntList;

    for (int i = 0; i < PLAY_COUNT; i++) {
        if (byZhuangList[i] == INVALID_DESKSTATION) {
            break;
        }

        auto user = _deskUserList->getUserByDeskStation(byZhuangList[i]);
        if (user == nullptr) {
            continue;
        }

        ntList.push_back(ForceGBKToUtf8(user->nickName));
    }

    return ntList;
}

/*------------------------public-------------------*/
// 申请上庄
void GameTableLogic::dealApplyNTResp(void* object, INT objectSize) {
    CCLOG("=========dealApplyNTResp!!!!!!!!!!!=============");

    CCAssert(sizeof (S_C_ApplyZhuangResult) == objectSize, "size error.");

    S_C_ApplyZhuangResult* pData = (S_C_ApplyZhuangResult*)object;
    memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));

    pData->bIsSuccess;//0表示申请成功，1表示已经在庄家列表中，2表示金币不够

    _iNowNtStation = pData->iNowNtStation;
    _i64NtMoney = pData->i64NtMoney;
    _iNtPlayCount = pData->iZhuangBaShu;
    _i64NtWinMoney = pData->i64NtWin;
    memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));

    // 更改上下庄显示
    if (_iNowNtStation == _mySeatNo || isInNTList(_mySeatNo) ==true) {
        _gameUICallback->IApplyNT(true);
    } else {
        _gameUICallback->IApplyNT(false);
    }

    // 下庄成功
    if (pData->bXiaZhuang && _mySeatNo == pData->byDeskStation) {
        if (_iNowNtStation == _mySeatNo)
            _gameUICallback->IShowNoticeMessage(ForceGBKToUtf8("将在下一局游戏退出庄家位置"));
        _gameUICallback->IApplyNT(false);
    }

    // 更新庄家列表
    _gameUICallback->IUpdateNTList();
}

// 无庄等待
void GameTableLogic::dealNoNTWaitResp(void* object, INT objectSize) {
    // 更改上下庄显示
    _gameUICallback->IApplyNT(false);
    //隐藏庄家
    _gameUICallback->hideNtInfo(false);
    //更新游戏状态提醒
    _gameUICallback->gameStation(GS_WAIT_SETGAME);
    //开启计时
    _gameUICallback->killTime();
    _gameUICallback->grayEffectBet(false);
}

// 游戏开始下注
void GameTableLogic::dealNoticeBetResp(void* object, INT objectSize) {
    CCAssert(sizeof (S_C_BeginData) == objectSize, "size error.");
    S_C_BeginData* pData = (S_C_BeginData*)object;

    _gameStatus = GS_NOTE_STATE;

    _iNowNtStation = pData->iNowNtStation;
    _i64NtMoney = pData->i64NtMoney;
    _i64NtWinMoney = pData->i64NtWinMoney;		//庄家的成绩
    _iNtPlayCount = pData->iNtPlayCount;
    _i64RemainBetNum = _i64RemainBetNumTwo;
    //更新游戏状态提醒
    _gameUICallback->gameStation(_gameStatus);
    //开启计时
    _gameUICallback->starTime(_iXiaZhuTime);

    //上庄列表
    memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));

    auto user = getUserBySeatNo(_iNowNtStation);
    if (user) {
        _gameUICallback->upDataNTInfo(user->dwUserID,user->bLogoID,user->nickName, _i64NtMoney, _i64NtWinMoney, _iNtPlayCount);
        if (_iNtStation != _iNowNtStation) {
            memset(_i64QuYuZhu, 0, sizeof(_i64QuYuZhu));
        }
    }
    // 更改上下庄显示
    if (_mySeatNo == _iNowNtStation)
        _gameUICallback->IApplyNT(true);
    // 更新庄家列表
    _gameUICallback->IUpdateNTList();
    //恢复桌面
    _gameUICallback->clearTable(true);
    _gameUICallback->grayEffectBet(true);

    _gameUICallback->repeatNote();

    _gameUICallback->playEffectOnStation();
}

//void GameTableLogic::dealUserNoContinueEnd(void* object, INT objectSize){}		// 91
void GameTableLogic::dealUserCutEnd(void* object, INT objectSize) {
    CCAssert(sizeof (tag_s2c_tip_exit) == objectSize, "size error.");
    tag_s2c_tip_exit* pData = (tag_s2c_tip_exit*)object;
    _gameUICallback->onGameTipExit(pData);
}// 92

// 游戏下注结果
void GameTableLogic::dealBetResultResp(void* object, INT objectSize) {
    CCAssert(sizeof (C_S_UserNoteResult) == objectSize, "size error.");
    C_S_UserNoteResult* pData = (C_S_UserNoteResult*)object;

    _i64Money = pData->i64Money;
    _i64RemainBetNum = pData->i64RemainBetNum;
    memcpy(_i64QuYuZhu, pData->i64QuYuZhu, sizeof(_i64QuYuZhu));
    memcpy(_i64QuYuZhuTrue, pData->i64QuYuZhuTrue, sizeof(_i64QuYuZhuTrue));
    memcpy(_i64UserXiaZhuData, pData->i64UserXiaZhuData, sizeof(_i64UserXiaZhuData));

    //更新区域总压注
    _gameUICallback->upDataAreaTotleMoney(_i64QuYuZhu);
    //更新区域压注
    _gameUICallback->upDataAreaMoney(_i64UserXiaZhuData);

    // 下注动画
    int betType[] = { 100, 1000, 10000, 100000, 1000000,5000000};
	//int playMusicCount = 0
    for (int i = 0; i < 5; i++) {
        if (pData->i64Money == betType[i]) {
            _gameUICallback->chipAnimation(true, betType[i], pData->iArea);
            break;
        }
    }
}

// 开始发牌
void GameTableLogic::dealSendCardResp(void* object, INT objectSize) {
    CCAssert(sizeof (S_C_SendCard) == objectSize, "size error.");
    S_C_SendCard* pData = (S_C_SendCard*)object;

    _gameStatus = GS_SEND_CARD;

    memcpy(_byUserCard, pData->byUserCard, sizeof(_byUserCard));
    memcpy(_byCardCount, pData->byCardCount, sizeof(_byCardCount));
    memcpy(_iCardShape, pData->iCardShape, sizeof(_iCardShape));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            //获取牌值
            _gameUICallback->initCardValue(pData->byUserCard[i][j], i, j);
            CCLOG("card  %d   i = %d  j = %d", pData->byUserCard[i][j], i, j);
        }
    }
    _iKaiPaiTime = pData->iKaiPaiTime;

    _gameUICallback->grayEffectBet(false);

    //更新游戏状态提醒
    _gameUICallback->gameStation(_gameStatus);
    //开计时器
    _gameUICallback->starTime(_iKaiPaiTime);
    //发牌
    _gameUICallback->ISendCard(true);
}

// 显示中奖区域
void GameTableLogic::dealShowWinAreaResp(void* object, INT objectSize) {
    CCLOG("=========dealShowWinAreaResp!!!!!!!!!!!=============");
    CCAssert(sizeof (S_C_ShowWinAreas) == objectSize, "size error.");
    S_C_ShowWinAreas* pData = (S_C_ShowWinAreas*)object;
    pData->iCompareResult;
    _gameStatus = GS_SHOW_WIN;
    //更新游戏状态提醒
    _gameUICallback->gameStation(_gameStatus);
    //开计时器
    _gameUICallback->starTime(_iShowWinTime);
    int winCout = 0;
    int loseCount = 0;
    for (int i = 0; i < 4; i++) {
        if (pData->iCompareResult[i] == 1 && i >= 1) { //刷中奖位置
            winCout++;
        }
        if (pData->iCompareResult[i] == -1 && i >= 1)
            loseCount++;

     //   _tableUIcallBack->showCardShap(i, _iCardShape[i]);
        if (pData->iCompareResult[i] == 1)
            _gameUICallback->showWinArea(i);
    }
    if (winCout == 3)
        _gameUICallback->allAnimation(true);
    if (loseCount == 3)
        _gameUICallback->allAnimation(false);
}

// 游戏结束
void GameTableLogic::dealGameEndResp(void* object, INT objectSize) {
    CCAssert(sizeof (S_C_GameResult) == objectSize, "size error.");
    S_C_GameResult* pData = (S_C_GameResult*)object;

    _gameStatus = GS_WAIT_NEXT;

    _i64NtMoney = pData->i64NtMoney;

    memcpy(byZhuangList, pData->byZhuangList, sizeof(byZhuangList));

    //更新游戏状态提醒
    _gameUICallback->gameStation(GS_WAIT_NEXT);
    //开计时器
    _gameUICallback->starTime(_iFreeTime);
    //玩家金币更新
    _gameUICallback->upDataPlayerInfo(pData->i64UserMoney, pData->i64UserWin);
    // 更新庄家列表
    _gameUICallback->IUpdateNTList();
    //更新庄家（庄列表有人直接更新 ）
    auto user = getUserBySeatNo(_iNowNtStation);
    if (user) {
        if (_mySeatNo == _iNowNtStation) {
            _gameUICallback->IApplyNT(true);
            _gameUICallback->openRepeatNote(false);

        }
        _gameUICallback->upDataNTInfo(user->dwUserID,user->bLogoID,user->nickName, _i64NtMoney, _i64NtWinMoney, _iNtPlayCount);
    }


    //更新路子把数
    _gameUICallback->upDataLuzi(pData->tGameLuzi);

    _gameUICallback->nextLevelGame();
    //结算面板
    _gameUICallback->showSettlementDialog(pData);

    _gameUICallback->grayEffectBet(false);

    //清理桌面
    _gameUICallback->clearTable(false);

	_i64NtMoney = 0;


}

//钱少提醒退出
void GameTableLogic::dealGameTipExit(void* object, INT objectSize) {
    CCLOG("=========dealGameTipExit!!!!!!!!!!!=============");
}

BYTE GameTableLogic::getGameStatus() {
    return _gameStatus;
}

bool GameTableLogic::isInNTList(BYTE seatNo) {
    bool isNT = (seatNo != INVALID_DESKSTATION && seatNo == _iNtStation);

    if (!isNT) {
        for (int i = 0; i < PLAY_COUNT; i++) {
            if (byZhuangList[i] == INVALID_DESKSTATION) {
                break;
            }

            if (byZhuangList[i] == seatNo) {
                isNT = true;
                break;
            }
        }
    }

    return isNT;
}

/*  ----------请求---------- */
void GameTableLogic::sendBeNT(bool isNT) {
    C_S_ApplyShangZhuang data;
    data.bShangZhuang = isNT;
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_APPLY_ZHUANG, &data, sizeof(data));
}


void GameTableLogic::sendDriveNianShou(int nUserID) {
//    NewYearActivityInGame::GanNianShouRequest msg;
//    msg.nUserID = nUserID;
//    //0活动1游戏
//    msg.bSceneType = 1;
//
//    HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, NewYearActivityInGame::C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
}


}
