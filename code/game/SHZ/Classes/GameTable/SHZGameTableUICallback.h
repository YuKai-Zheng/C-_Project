#ifndef __SHZ_GameTableUICallback_h__
#define __SHZ_GameTableUICallback_h__

#include "SHZCommonDef.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace SHZ
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
        virtual void leaveTheGame() = 0;
        //
		virtual void syncBasicDataFirstTime(struct_s2c_base* msg) = 0;
		virtual void updateGameState(struct_game_state* msg) = 0;
		virtual void openLineResultData(struct_s2c_startGame* msg) = 0;
		virtual void openHasMaryTip(struct_s2c_hasMary* msg) {}
        virtual void updateMaryData(struct_s2c_maryData* msg) {}
		virtual void maryFinishTip(struct_s2c_maryFinish* msg) {}
        virtual void openLineResultIsCanGuess(struct_s2c_caiDaXiao* msg) = 0;
		virtual void gueessSizeStar(struct_s2c_doCaiDaXiao* msg) = 0;
		virtual void guessSizeResult(struct_s2c_caiResult* msg) = 0;
		virtual void guessSizeOver(struct_s2c_caiFinish* msg) = 0;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) = 0;
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
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
    };
    
}

#endif
