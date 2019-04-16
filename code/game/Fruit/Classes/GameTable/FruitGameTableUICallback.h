#ifndef __Fruit_GameTableUICallback_h__
#define __Fruit_GameTableUICallback_h__

#include "FruitCommonDef.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace Fruit
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
        virtual void leaveTheGame() = 0;
        //
        virtual void syncBasicDataFirstTime(SyncDataStruct* msg) = 0;
//        //进入或者短线重连恢复游戏桌子场景
//        virtual void resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) = 0;
//		virtual void resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) = 0;
//		virtual void resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) = 0;
//		virtual void resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) = 0;

        
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        
        virtual void addUser(BYTE seatNo, bool bMe) = 0;
       
        //服务端返回下注结果
        virtual void onGameNoteResponse(GameNoteResponse* msg) = 0;
        //服务端下注失败结果
        virtual void onGameNoteFail(GameNoteFailedMsg* msg) = 0;
        //显示开奖结果
        virtual void onGameOpenLogo(GameLoopResult* msg) = 0;
        //
        virtual void onGameNotifyGuess(GameNotifyGuessNumber* msg) = 0;
        //
        virtual void onGameGuessOverTime(GameGuessOverTime* msg) = 0;
        //
        virtual void onGameGuessResult(GameGuessResult* msg) = 0;
        //
        virtual void onGameSwapScoreResponse(GameSwapScoreReseponse* msg) = 0;
        //
        virtual void onGameNotifyLucky(GameNotifyLucky* msg) = 0;
        //展示开Lucky结果
        virtual void onGameOpenLuckyLogos(GameLuckyOpen* msg) = 0;
        //接收结算消息
        virtual void onGameSettlementMsg(GameSettlementMsg* msg) = 0;
        
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) = 0;
		//通知
		virtual void onGameNotify(tag_s2c_notify* msg) = 0;

		//奖池
		virtual void onGameRewardsPool(GameRewardsPool* msg) = 0;
		
		//开始游戏金币不足
		virtual void onGameBeginFailLackCoin(GameBeginFailLackCoin* msg) = 0;
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
    };
    
}

#endif
