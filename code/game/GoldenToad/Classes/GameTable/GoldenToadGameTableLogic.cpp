#include "GoldenToadGameTableLogic.h"
#include "GoldenToadGameTableUICallback.h"
#include "GoldenToadMessageHead.h"
//#include "HNNetExport.h"
#include "GoldenToadVersion.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"
#include "HNLobby/v66/ExtraReward.h"

namespace GoldenToad {

GameTableLogic::GameTableLogic(GameTableUICallback* tableUICallBack, BYTE deskNo, bool bAutoCreate)
    : HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, tableUICallBack)
    , _tableUICallBack(tableUICallBack)
//    , _roomRate(0.0f)
    , _baseBulletMul(100)
    , _userMoney(0)
    , _baseScoreValue(0)
    , _oldMoney(0)
    , _oldScore(0) {
    initParams();
}

GameTableLogic::~GameTableLogic() {
//        for (auto usr : _users) {

//        }
    _users.clear();
}

void GameTableLogic::enterGame() {
    _seatOffset = 0;
    if(_mySeatNo == INVALID_DESKSTATION && !_autoCreate) {
        for(int i = 0; i < PLAY_COUNT; i++) {
            if(!_existPlayer[i]) {
                sendUserSit(logicToViewSeatNo(i));
                break;
            }
        }
    } else {
        loadUsers();

        if(_mySeatNo != INVALID_DESKSTATION && _autoCreate) {
            sendGameInfo();
        }
    }
}

void GameTableLogic::loadUsers() {
    _MYSEATNO = _mySeatNo;
    for(int i = 0; i < PLAY_COUNT; i++) {
        //i为实际物理位置
        UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
        if(_existPlayer[i] && pUser != nullptr) {
            _playerSitted[i] = true;
            int viewNo = logicToViewSeatNo(i);
            _tableUICallBack->addUser(viewNo, i == _MYSEATNO);
            _tableUICallBack->dealSetUserMoney(i, pUser->i64Money);//玩家在没有进行上分操作之前，其积分一定为0，所以此处直接赋给初始值即可。
            /**
             *	设置玩家金币
             *	玩家金币仅设置、显示自己的即可，其他玩家仅可以看到其积分，看不到其金币情况
             */
            if (i == _MYSEATNO) {
                _tableUICallBack->dealSetUserCoin(i, pUser->i64Money);
            }
        }
    }
}

LLONG GameTableLogic::getUserMoney(BYTE seatNo) {
    UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(seatNo);
    return pUser->i64Money;
}

LLONG GameTableLogic::getMeMoney() {
    return _userMoney;// getUserMoney(_MYSEATNO);
}

void GameTableLogic::setMeMoney(LLONG money) {
    _userMoney = money;
}

//LLONG GameTableLogic::getMeScore()
//{
//	return _userScore;
//}

//void GameTableLogic::setMeScore(LLONG score)
//{
//	_userScore = score;
//}

LLONG GameTableLogic::getUserCannonNum(BYTE seatNo) {
    return 0;
}

////////////////////////////////////////////////////////////////////
////聊天接口
////////////////////////////////////////////////////////////////////
void GameTableLogic::dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk) {
    auto user = _deskUserList->getUserByUserID(normalTalk->dwSendID);
    _tableUICallBack->showUserChatMsg(user->bDeskStation, normalTalk->szMessage);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void GameTableLogic::sendOpenFire(CMD_C_UserFire* pFire, INT size) {
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY , SUB_C_USER_FIRE, pFire, size);
}

void GameTableLogic::sendHitFish(BYTE seatNo, INT fishId, INT bulletId, BulletKind bulletKind, INT bulletMuriple) {
    CMD_C_CatchFish catch_fish;
    catch_fish.chair_id = seatNo;
    catch_fish.bullet_id = bulletId;
    catch_fish.bullet_kind = bulletKind;
//    int anskind = (int)bulletKind;
    catch_fish.fish_id = fishId;
    catch_fish.bullet_mulriple = bulletMuriple;
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_CATCH_FISH, (CHAR*)&catch_fish, sizeof(CMD_C_CatchFish));
}
    
    void GameTableLogic::sendChangeCoin(BYTE seatNo, bool isAdd) {
        CMD_C_ChangeCoin changeCoin;
        changeCoin.currentBulletMul = getBaseBulletMultiple();
        changeCoin.isUp = isAdd;
        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_CHANGE_COIN, &changeCoin, sizeof(CMD_C_ChangeCoin));
    }

void GameTableLogic::sendSweepFish(CMD_C_CatchSweepFish* pRet, INT size) {
    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_CATCH_SWEEP_FISH, pRet, size);
}

//    /**
//     *	上分通信逻辑
//     */
//    void GameTableLogic::sendScoreUp()
//    {
//        CMD_C_ExchangeFishScore exchangeFishScore;
//        exchangeFishScore.increase = true;
//        exchangeFishScore.isSingle = true;		//代表单次上分操作
//        exchangeFishScore.roomRate = getRoomRate();
//        exchangeFishScore.exchange_fish_money = 0;
//        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_EXCHANGE_FISHSCORE, (CHAR*)&exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
//    }
//
//    /**
//     *	上分通信逻辑
//     *	一次性将一定比例的金币转换成积分
//     *	这个方法对于 AI 来说，应该是随机根据自己的金币数进行上分，而对于具体玩家，则应该传入实际的金币参数进行上分操作。
//     */
//    void GameTableLogic::sendScoreUpWithStaticMoney(LLONG money)
//    {
//        if (money <= 0)
//        {
//            //提示取消上分
//            cancelScoreUpTips();
//            return;
//        }
//        CMD_C_ExchangeFishScore exchangeFishScore;
//        exchangeFishScore.increase = true;
//        exchangeFishScore.isSingle = false;
//        exchangeFishScore.roomRate = getRoomRate();
//        exchangeFishScore.exchange_fish_money = money;
//
//        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_EXCHANGE_FISHSCORE, (CHAR*)&exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
//    }
//
//    /**
//     *	下分通信逻辑
//     */
//    void GameTableLogic::sendScoreDown()
//    {
//        CMD_C_ExchangeFishScore exchangeFishScore;
//        exchangeFishScore.increase = false;
//        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_EXCHANGE_FISHSCORE, (CHAR*)&exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
//    }

/**
 *	获得随机应该转换成积分的金币数值
 */
LLONG GameTableLogic::getRandomMoneyForExchangeScore() {
    LLONG swapMoney = 0;
    //1、计算转换概率，若概率超出界限，需要修正
    double rate = (double)(rand() % 80) / 100;
    rate += 0.2f;		//因为概率要求 20% - 100%，所以生成 0-80的随机数并添加0.2，则0.2-0.8的随机数
    if (rate < 0.2f) {
        rate = 0.2f;
    }
    if (rate > 1.0f) {
        rate = 1.0f;
    }
    //2、按照金币计算需要转换的金币数，若金币数不是10的倍数，需要对其进行修正。
    swapMoney = (LLONG)(getMeMoney() * rate);
    if (swapMoney % 10 != 0) {
        //说明计算出来的积分不是10的倍数，应该对其进行调整
        //调整方案为将多余的数字舍去
        swapMoney -= (swapMoney % 10);
    }

    return swapMoney;		//返回单次需要转换为积分的金币，该值必须为 10 的整数倍。
}

/**
 *	上分金币输入为 0，代表取消上分，此时应该有上分取消提示
 */
void GameTableLogic::cancelScoreUpTips() {
    _tableUICallBack->resetUserMoneyAndScore(_oldMoney, _oldScore);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/*
 * @func: framework message function.
 * @info: they will be called when frame message come from base class.
 */
/////////////////////////////////////////////////////////////////////////////////////////////
void GameTableLogic::dealGameStartResp(BYTE bDeskNO) {
    HNGameLogicBase::dealGameStartResp(bDeskNO);
}

void GameTableLogic::dealGameEndResp(BYTE deskNo) {
    HNGameLogicBase::dealGameEndResp(deskNo);
}

void GameTableLogic::dealGamePointResp(void* object, INT objectSize) {
    HNGameLogicBase::dealGamePointResp(object, objectSize);

    if(objectSize != sizeof(MSG_GR_R_UserPoint)) return;

    //TODO
    //MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;
}

void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    //TODO
    bool isMe = (user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);

    if(isMe) {
        enterGame();
    } else {
        if(_playerSitted[userSit->bDeskStation])	 return;

//            UserInfoStruct* UserInfo = _deskUserList->getUserByDeskStation(userSit->bDeskStation);
        _playerSitted[userSit->bDeskStation] = true;

        _tableUICallBack->addUser(userSit->bDeskStation, userSit->bDeskStation == _mySeatNo);
        _tableUICallBack->dealSetUserMoney(userSit->bDeskStation, user->i64Money);
    }
}

void GameTableLogic::dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user) {
    HNGameLogicBase::dealQueueUserSitMessage(deskNo, user);

    if(_mySeatNo != INVALID_DESKSTATION) {
        enterGame();
    }
}

void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    if(!_playerSitted[userSit->bDeskStation])	return;
    _playerSitted[userSit->bDeskStation] = false;

    //TODO
    _tableUICallBack->removeUser(userSit->bDeskStation, userSit->bDeskStation == _mySeatNo);
    if(_mySeatNo == INVALID_DESKSTATION) {
        stop();
        _tableUICallBack->dealLeaveDesk();
    }
}

void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) {
}

// 游戏状态
void GameTableLogic::dealGameStationResp(void* object, INT objectSize) {
    CHECK_SOCKET_DATA(GameStation, objectSize, "GameStation size of error!");
    GameStation *pGameStation = (GameStation *)object;

    CMD_S_GameConfig* pConfig = &pGameStation->GameConfig;
    _tableUICallBack->dealSetGameConfig(pConfig);

    CMD_S_GameStatus* pStatus = &pGameStation->GameStatus;
    _tableUICallBack->dealSetSceneStyle(pStatus->Scene_Style_Kind);

    /**
     *	进入游戏前更改该参数，因为进入游戏后该参数会影响游戏所执行的逻辑
     *	by HBC
     */
    //Version::GAME_VERSION_NAME = Version::GAME_VERSION_LIKUI_FISH;

    for (int i = 0; i < PLAY_COUNT; i++) {
        //显示桌上玩家炮火倍数
        _tableUICallBack->dealSetUserCannonNum(i, pStatus->iUserFireBase[i]);
    }
}

// 游戏消息（游戏的主体消息来自这里）
void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
    if(MDM_GM_GAME_NOTIFY != messageHead->bMainID) {
        HNLOG("onGameMessage MainID is error.");
    }

    switch (messageHead->bAssistantID) {
    case SUB_S_UPDATE_LOCK_FISH: {
        CHECK_SOCKET_DATA(CMD_COM_ALLOW_LOCK_FISH, objectSize, "CMD_COM_ALLOW_LOCK_FISH size of error!");
        CMD_COM_ALLOW_LOCK_FISH* allowFish = (CMD_COM_ALLOW_LOCK_FISH*)object;
        _tableUICallBack->dealUpdateAllowFish(allowFish);
    }
    break;
    case SUB_S_GAME_CONFIG: { //设置游戏参数
        CHECK_SOCKET_DATA(CMD_S_GameConfig, objectSize, "CMD_S_GameConfig size of error!");
        CMD_S_GameConfig* pConfig = (CMD_S_GameConfig*)object;

        _tableUICallBack->dealSetGameConfig(pConfig);
    }
    break;
    case SUB_S_FISH_TRACE: { //生成新鱼的轨迹
        _tableUICallBack->dealAddFishInfo(object, objectSize);
    }
    break;
    case SUB_S_FISH_GROUP_TRACE: { //生成鱼群的轨迹
        CHECK_SOCKET_DATA(CMD_S_GroupFishTrace, objectSize, "CMD_S_GroupFishTrace size of error!");
        CMD_S_GroupFishTrace* pTrace = (CMD_S_GroupFishTrace*)object;

        _tableUICallBack->dealAddGroupFishInfo(pTrace);
    }
    break;
//            case SUB_S_EXCHANGE_FISHSCORE://上分、下分结果更新
//            {
//                CHECK_SOCKET_DATA(CMD_S_ExchangeFishScore, objectSize, "CMD_S_ExchangeFishScore size of error!");
//                CMD_S_ExchangeFishScore* exchangeFish = (CMD_S_ExchangeFishScore*)object;
//                //不管上下分是否成功，数据都更新
//                //若失败，还需要作出金币不足的提示
//                if (exchangeFish->chair_id == _MYSEATNO)
//                {
//                    _tableUICallBack->dealSetUserCoin(exchangeFish->chair_id, exchangeFish->exchange_fish_score);
//                    _tableUICallBack->dealShowScoreUpTips(exchangeFish->isIncrease, exchangeFish->isExchangeSuccess);
//                }
//                _tableUICallBack->dealSetUserMoney(exchangeFish->chair_id, exchangeFish->swap_fish_score);
//                //维护基础值
//                setBaseScoreValue(exchangeFish->swap_fish_score);
//                //上分、下分结果
//                _tableUICallBack->dealExchangeFishCoinResult(exchangeFish->isIncrease, exchangeFish->isExchangeSuccess);
//            }
//                break;
        case SUB_S_CHANGE_COIN: {
            CHECK_SOCKET_DATA(CMD_S_ChangeCoin, objectSize, "CMD_S_ChangeCoin size of error!");
            CMD_S_ChangeCoin* changeCoin = (CMD_S_ChangeCoin*)object;
            if (changeCoin->chair_id == _MYSEATNO) {
                setBaseBulletMultiple(changeCoin->bullet_mulriple);
                _tableUICallBack->dealUserChangeCoin(changeCoin);
            }
        }
        break;
    case SUB_S_USER_FIRE: { //玩家射击
        CHECK_SOCKET_DATA(CMD_S_UserFire, objectSize, "CMD_S_UserFire size of error!");
        CMD_S_UserFire* pFire = (CMD_S_UserFire*)object;

        _tableUICallBack->dealGameFire(pFire, false);
    }
    break;
    case SUB_S_CATCH_FISH: { //抓到鱼
        CHECK_SOCKET_DATA(CMD_S_CatchFish, objectSize, "CMD_S_CatchFish size of error!");
        CMD_S_CatchFish* pCatch = (CMD_S_CatchFish*)object;

        _tableUICallBack->dealCatchFish(pCatch);
    }
    break;
    case SUB_S_BULLET_ION_TIMEOUT: { //能量炮失效
        CHECK_SOCKET_DATA(CMD_S_BulletIonTimeout, objectSize, "CMD_S_BulletIonTimeout size of error!");
        CMD_S_BulletIonTimeout* pTimeOut = (CMD_S_BulletIonTimeout*)object;

        _tableUICallBack->dealBulletTimeOut(pTimeOut->chair_id);
    }
    break;
    case SUB_S_LOCK_TIMEOUT: { //定屏炸弹失效时间
        _tableUICallBack->dealLockFishTimeOut();
    }
    break;
    case SUB_S_CATCH_SWEEP_FISH: { //抓到特殊鱼（炸弹，鱼王）
        CHECK_SOCKET_DATA(CMD_S_CatchSweepFish, objectSize, "CMD_S_CatchSweepFish size of error!");
        CMD_S_CatchSweepFish* pSweep = (CMD_S_CatchSweepFish*)object;

        _tableUICallBack->dealCatchSweepFish(pSweep);

    }
    break;
    case SUB_S_CATCH_SWEEP_FISH_RESULT: { //抓到特殊鱼结果
        CHECK_SOCKET_DATA(CMD_S_CatchSweepFishResult, objectSize, "CMD_S_CatchSweepFishResult size of error!");
        CMD_S_CatchSweepFishResult* pSweep = (CMD_S_CatchSweepFishResult*)object;

        _tableUICallBack->dealCatchSweepFishResult(pSweep);
    }
    break;
    case SUB_S_CATCH_SWEEP_FISH_RESULT_EX: { //广播通知所有玩家捕到大鱼
        CHECK_SOCKET_DATA(CMD_S_CatchSweepFishResultEx, objectSize, "CMD_S_CatchSweepFishResultEx size of error!");
        CMD_S_CatchSweepFishResultEx* pResult = (CMD_S_CatchSweepFishResultEx*)object;

        _tableUICallBack->dealCatchSweepFishResultEx(pResult);
    }
    break;
    case SUB_S_SWITCH_SCENE: { //切换场景，5种鱼阵
        CHECK_SOCKET_DATA(CMD_S_SwitchScene, objectSize, "CMD_S_SwitchScene size of error!");
        CMD_S_SwitchScene* pScene = (CMD_S_SwitchScene*)object;

        _tableUICallBack->dealSwitchScene(pScene);
    }
    break;
    case SUB_S_HIT_FISH_LK:
    case SUB_S_SCENE_END:
        break;
//            case S_C_SUPER_CONFIG_SIG://是否超端玩家
//            {
//                CHECK_SOCKET_DATA(S_C_SUPER_CONFIG, objectSize, "S_C_SUPER_CONFIG size of error!");
//                S_C_SUPER_CONFIG* pSuper = (S_C_SUPER_CONFIG*)object;
//
//                _tableUICallBack->dealSetAdminConfig(pSuper->bSuperUser);
//            }
//                break;
//            case SUB_S_STOCK_OPERATE_RESULT://查询库存，抽水
//            {
//                CHECK_SOCKET_DATA(CMD_S_StockOperateResult, objectSize, "CMD_S_StockOperateResult size of error!");
//                CMD_S_StockOperateResult* pStock = (CMD_S_StockOperateResult*)object;
//
//                _tableUICallBack->dealStockOperateResult(pStock);
//            }
//                break;
    case S_C_UPDATE_ONLINEUSER_INFO_SIG: { //更新在线玩家信息
        CHECK_SOCKET_DATA(S_C_ON_LINE_USER, objectSize, "S_C_ON_LINE_USER size of error!");
        S_C_ON_LINE_USER* pOnline = (S_C_ON_LINE_USER*)object;

        if (pOnline->bStart) _users.clear();

        _users.push_back(pOnline->user_info_);
    }
    break;
//            case S_C_END_UPDATE_ONLINEUSER_INFO_SIG://更新在线玩家信息列表
//            {
//                _tableUICallBack->dealUpdateOnlinePlayerList(_users);
//            }
//                break;
//            case S_C_UPDATE_CONTROL_INFO_SIG://更新控制概率信息
//            {
//                CCASSERT(objectSize % sizeof(ControlInfo) == 0, "ControlInfo size is error");
//                if (objectSize % sizeof(ControlInfo) != 0) return;
//
//                BYTE count = objectSize / sizeof(ControlInfo);
//                ControlInfo* pInfo = (ControlInfo*)object;
//
//                std::vector<ControlInfo> Controls;
//                while (count > 0)
//                {
//                    Controls.push_back(*pInfo++);
//                    count--;
//                }
//
//                _tableUICallBack->dealUpdateContorlList(Controls);
//            }
//                break;
//            case S_C_UPDATE_SPECIAL_INFO_SIG://更新控制特殊鱼信息
//            {
//                CCASSERT(objectSize % sizeof(SpecialUser) == 0, "SpecialUser size is error");
//                if (objectSize % sizeof(SpecialUser) != 0) return;
//
//                BYTE count = objectSize / sizeof(SpecialUser);
//                SpecialUser* pUser = (SpecialUser*)object;
//
//                std::vector<SpecialUser> Specials;
//                while (count > 0)
//                {
//                    Specials.push_back(*pUser++);
//                    count--;
//                }
//
//                _tableUICallBack->dealUpdateSpecialList(Specials);
//            }
//                break;
//            case S_C_SWITCH_INFO_SIG://控制开关信息
//            {
//                CHECK_SOCKET_DATA(S_C_SWITCH_INFO, objectSize, "S_C_SWITCH_INFO size of error!");
//                S_C_SWITCH_INFO* pSwitch = (S_C_SWITCH_INFO*)object;
//
//                _tableUICallBack->dealSetSwitchInfo(pSwitch);
//            }
//                break;
//            case S_C_UPDATE_FISH_NAME_SIG://更新可控制鱼名称
//            {
//                CHECK_SOCKET_DATA(S_C_FISH_NAME, objectSize, "S_C_FISH_NAME size of error!");
//                S_C_FISH_NAME* pName = (S_C_FISH_NAME*)object;
//
//                _tableUICallBack->dealUpdateFishName(pName);
//            }
//                break;
    ///////////////
    //五花牛全服公告
    case NN::S2C_NOTIFY_MSG: {
//                CHECK_SOCKET_DATA(NN::tag_s2c_notify, objectSize, "tag_s2c_notify size of error!");
//        int size = sizeof(NN::tag_s2c_notify);
//        CCAssert(sizeof(NN::tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
//        if (sizeof(NN::tag_s2c_notify) != objectSize)    return;
//
//        NN::tag_s2c_notify* msg = (NN::tag_s2c_notify*)object;
//        _tableUICallBack->dealGameNotifyWuHuaNiu(msg);
    }
    break;

//    //年兽
//    case NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG : {
//        //年兽出现 150
//        _tableUICallBack->onGameShowNianShou();
//    }
//    break;
//    case NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT: {
//        //赶年兽 151
//        CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//
//        NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//        _tableUICallBack->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//    }
//    break;
//    case NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG: {
//        //对局获得鞭炮 152
//        CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//
//        NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//        _tableUICallBack->onGameMatchGainFirecraker(msg->nUserID);
//    }
//    break;


    default:
        break;
    }
}

void GameTableLogic::initParams() {
    memset(_playerSitted, 0, sizeof(_playerSitted));
}

/*
 * refresh member before every game start.
 */
void GameTableLogic::refreshParams() {

}

//查询奖池操作
void GameTableLogic::sendStockOperate(BYTE operate_code) {
    CMD_C_StockOperate stock_operate;
    stock_operate.operate_code = operate_code;

    RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_STOCK_OPERATE, &stock_operate, sizeof(stock_operate));
}

/**
 *	设定房间炮火倍数
 */
void GameTableLogic::setBaseBulletMultiple(int value) {
    _baseBulletMul = value;
}

int GameTableLogic::getBaseBulletMultiple() {
    return _baseBulletMul;
}

void GameTableLogic::sendDriveNianShou(int nUserID) {
//    NewYearActivityInGame::GanNianShouRequest msg;
//    msg.nUserID = nUserID;
//    //0活动1游戏
//    msg.bSceneType = 1;
//
//    HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, NewYearActivityInGame::C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
}

void GameTableLogic::setOldMoney(long long money) {
    _oldMoney = money;
}

void GameTableLogic::setOldScore(long long score) {
    _oldScore = score;
}

//    long long getOldMoney()
//    {
//        return _oldMoney;
//    }
//
//    long long getOldScore()
//    {
//        return _oldScore;
//    }

}
