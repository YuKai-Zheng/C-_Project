#ifndef __OnePieceSlotGameTableUICallback_h__
#define __OnePieceSlotGameTableUICallback_h__

#include "OnePieceSlotMessageHead.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace OnePieceSlot
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
		//离开
        virtual void leaveTheGame() = 0;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //加载玩家
        virtual void addUser(BYTE seatNo, bool bMe) = 0;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) = 0;
		//游戏状态
		virtual void onGameState() = 0;
		//基础消息包
		virtual void onGameBaseInfo(struct_s2c_base* msg) = 0;
		//更新奖池
		virtual void onGamePoolMoney(struct_s2c_pool* msg) = 0;
		//游戏开始
		virtual void onGameStart(struct_s2c_startGame* msg) = 0;
		//是否猜大小
		virtual void onGameDoCaiDaXiao(struct_s2c_doCaiDaXiao* msg) = 0;
		//猜大小
		virtual void onGameCaiDaXiao(struct_s2c_caiDaXiao* msg) = 0;
		//猜大小结算
		virtual void onGameCaiResult(struct_s2c_caiResult* msg) = 0;
		//游戏结算结束
		virtual void onGameFinish(struct_s2c_caiFinish* msg) = 0;
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
    };
    
}

#endif // __OnePieceSlotGameTableUICallback_h__
