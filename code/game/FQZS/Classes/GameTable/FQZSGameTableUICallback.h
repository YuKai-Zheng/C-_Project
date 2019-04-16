#ifndef __FQZS_GameTable_BRNNTableUICallback_h__
#define __FQZS_GameTable_BRNNTableUICallback_h__

#include "FQZSCommonDef.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace FQZS
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
        virtual void leaveTheGame() = 0;
        //进入或者短线重连恢复游戏桌子场景
        virtual void resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) = 0;
		virtual void resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) = 0;
		virtual void resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) = 0;
		virtual void resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) = 0;

        
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        
        virtual void addUser(BYTE seatNo, bool bMe) = 0;

		//游戏开始
		virtual void onGameBegin(GameBeginStruct* msg) = 0;
		//通知下注
		virtual void onGameBeginBet(GameNote* msg) = 0;
		//下注结果
		virtual void onGameBetResult(GameXiaZhuResponse* msg) = 0;
		//转圈开奖
		virtual void onGameStartCircle(GamePlay* msg) = 0;
		//显示开奖结果
		virtual void onGameShowResult(GamePlayResult* msg) = 0;
		//计算结果
		virtual void onGameCalcuResult(GameEndMsg* msg) = 0;
		//提醒退出
		virtual void onGameTipExit(le_s2c_tip_exit* msg) = 0;
		//通知
		virtual void onGameNotify(tag_s2c_notify* msg) = 0;
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
//    public:
//        //年兽触发
//        virtual void onGameShowNianShou() = 0;
//        //赶年兽结果
//        virtual void onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) = 0;
//        //对局获得鞭炮
//        virtual void onGameMatchGainFirecraker(int nUserID) = 0;
        
    };
    
}

#endif // __BRNN_GameTable_BRNNTableUICallback_h__
