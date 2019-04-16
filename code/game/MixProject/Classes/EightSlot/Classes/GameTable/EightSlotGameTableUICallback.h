#ifndef __EightSlotGameTableUICallback_h__
#define __EightSlotGameTableUICallback_h__

#include "EightSlotMessageHead.h"
#include "HNRoomLogic/HNGameLogicBase.h"
#include "HNLobby/v66/IGameTableUI.h"
/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace EightSlot
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
		//游戏开始
		virtual void onGameStart(struct_s2c_startGame* msg) = 0;
        virtual void onGameUpdateMoney(struct_s2c_update_money* msg)=0;
	};
    
}

#endif // __FruitSlotGameTableUICallback_h__
