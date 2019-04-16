#ifndef  __BRXJGameLogic_H__
#define __BRXJGameLogic_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"

#include "BRXJMessageHead.h"
#include "BRXJGameTableUICallback.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"

//#include "NewYearInGameMessageHead.h"

using namespace std;

namespace BRXJ {
class GameTableLogic : public HNGameLogicBase {
public:
    // 构造函数
    GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool autoCreate);

    // 析构函数
    virtual ~GameTableLogic();

private:

    // 同意开始
    virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree) override;

    // 游戏开始
    virtual void dealGameStartResp(BYTE bDeskNO) override;

    // 玩家坐下
    virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, const UserInfoStruct* user);

    // 玩家站起
    virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

    // 游戏信息
    virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) override;

    // 游戏状态
    virtual void dealGameStationResp(void* object, INT objectSize) override;

    // 游戏消息
    virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;

public:
    // 请求站起
    void requestStandUp();

    // 请求下注
    void requestBet(int betType, int betArea);

    // 请求上庄、下庄
    void requestBeNT(bool isNT);

    // 申请断线重连，或者刚刚进入房间时申请恢复桌子数据包
    void requestGameData();

    // 请求庄家列表
    vector<string> requestNTList();

    // 发送下注消息
    void sendBet(BYTE deskNo, int betType, int betArea);

public:
    // 申请上庄
    void dealApplyNTResp(void* object, INT objectSize);

    // 无庄等待
    void dealNoNTWaitResp(void* object, INT objectSize);

    // 游戏开始下注
    void dealNoticeBetResp(void* object, INT objectSize);

    // 游戏下注结果
    void dealBetResultResp(void* object, INT objectSize);

    // 开始发牌
    void dealSendCardResp(void* object, INT objectSize);

    // 显示中奖区域
    void dealShowWinAreaResp(void* object, INT objectSize);

    //强制退出
    void dealUserCutEnd(void* object, INT objectSize);

    // 游戏结束
    void dealGameEndResp(void* object, INT objectSize);

    //钱少提醒退出
    void dealGameTipExit(void* object, INT objectSize);

    //通知
    void onGameNotify(NN::tag_s2c_notify* msg);

    //桌子所有玩家
    HNGameTableUsersData* getDeskUserList() {
        return _deskUserList;
    }

	int * GetCardShapes() {
		return _iCardShape;
	}
    //获取游戏状态
    BYTE getGameStatus();
private:
    //是否是庄家
    bool isInNTList(BYTE seatNo);
private:
    void sendBeNT(bool isNT);

public:
    // 游戏状态
    BYTE _gameStatus;

    BYTE getBankerDeskStation() {
        return _iNowNtStation;
    };


public:
    //赶年兽
    void sendDriveNianShou(int nUserID);

private:
    GameTableUICallback*			_gameUICallback;

    vector<string>			_vecMessageStr;

    BYTE				byZhuangList[PLAY_COUNT];						//上庄列表
    int					_iXiaZhuTime;											// 下注时间
    int					_iKaiPaiTime;     										// 开牌时间
    int					_iFreeTime;												// 空闲时间
    int					_iShowWinTime;										// 显示中奖时间
    int					_iRemaindTime;											// 阶段剩余时间

    int					_iGameCount;											//当前进行把数
    int					_iGameBeen;												//当前进行局数

    int					_iNtStation;												//庄家位置
    int					_iNtPlayCount;											//庄家坐庄次数
    int					_iNowNtStation;											//庄家现在的位置
    LLONG			_i64NtMoney;											//庄家金币
    LLONG			_i64NtWinMoney;										//庄家输赢情况


    LLONG			_i64UserWin;												//个人输赢
    LLONG			_i64MyMoney;											//个人金币数 -从服务端发送过去
    LLONG			_i64UserMoney;											//用户金币

    LLONG			_i64UserMaxNote;										//玩家最大下注数
    LLONG			_i64ShangZhuangLimit;								//上庄需要的最少金币
    LLONG			_i64ChouMaValues[6];								//6个筹码值
    LLONG			_i64PerMaxNotes;										//本局最大下注数

    LLONG			_i64RemainBetNum;									//剩余最大下注数
    LLONG			_i64RemainBetNumTwo;								//剩余最大下注数
    TGameLuzi	_tGameLuzi[GAME_SAVE_COUNT];				//路子信息

    LLONG			_i64UserXiaZhuData[MAX_NOTE_COUNT];	//玩家下注情况
    LLONG			_i64QuYuZhuTrue[MAX_NOTE_COUNT];		//真实玩家的下注值
    LLONG			_i64UserXiaZhu[MAX_NOTE_COUNT];			//真实玩家的下注值
    LLONG			_i64QuYuZhu[MAX_NOTE_COUNT];			// 本把每个区域下的注额
    LLONG			_i64Money;												//筹码值
    LLONG			_i64ZhongZhu;											//下注总筹码

    BYTE				_byUserCard[4][2];										//牌数据
    BYTE				_byCardCount[4];										//牌张数
    int					_iCardShape[4];											//牌型点数
    int					_iCompareResult[4];									//中奖区域

    BYTE				_byDeskStation;											//申请的玩家
};
}

#endif // __BJLGameTableLogic_h__
