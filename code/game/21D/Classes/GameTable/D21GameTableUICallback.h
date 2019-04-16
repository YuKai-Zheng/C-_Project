#ifndef __D21_Game_Table_UI_Callback_H__
#define __D21_Game_Table_UI_Callback_H__

#include "HNNetExport.h"
#include "D21MessageHead.h"
#include "HNLogicExport.h"

namespace D21
{
	class GameTableUICallback : public IHNGameLogicBase
	{
	public:
		//处理抢庄
		virtual void dealCatchBankerMsg(GameMessageCatch* msg) = 0;
		//确定庄家
		virtual void dealConfirmBankerMsg(GameMessageNt* msg) = 0;
		//处理下注
		virtual void dealBetMsg(GameMessageBetResult* msg) = 0;
		//发前两张
		virtual void dealStartSendCardMsg(GameMessageSendCard* msg) = 0;
		//购买保险
		virtual void dealBuyInsuranceMsg(GameMessageInsurance* msg) = 0;
		//操作通知
		virtual void dealOperateNotifyMsg(GameMessageOperateNotify* msg) = 0;
		//处理分牌
		virtual void dealDivideResultMsg(GameMessageDivideResult* msg) = 0;

		//创建下注按钮
		virtual void createBetButton(long long i64BetUnit[4]) = 0;
		//显示玩家准备
		virtual void showUserReady(BYTE seatNo, bool IsMe, bool visible) = 0;
		virtual void showUserReady(BYTE seatNo,bool visible) = 0;
		//设置玩家准备
		virtual void showBtnStart(BYTE seatNo) = 0;
		//设置庄家
		virtual void setNt(BYTE seatNo, bool IsMe) = 0;
		//添加玩家
		virtual void addUser(BYTE seatNo, bool bMe,LLONG userId)	= 0;
		//更新计分面板
		virtual void upDataRecordBoard(LLONG historyMoney, LLONG lastMoney, LLONG allPlayerMoney) = 0;
		virtual void upDataRecordBoard(LLONG allPlayerMoney) = 0;
		//添加结算面板
		virtual void openResultView(bool open) = 0;
		virtual void upDataResult(std::string name, LLONG money, LLONG cardShap, bool winrolose,int seatnum) = 0;
		//移除玩家
		virtual void removeUser(BYTE seatNo, bool bMe) = 0;
		//处理操作
		virtual void dealOperateResp(GameMessageNeedCard* msg) = 0;
// 		//开始操作
// 		virtual void startOperateResp(BYTE seatNo, bool bCanDivide) = 0;
// 		//加倍
// 		virtual void addDoubleResp(BYTE seatNo, BYTE value, LLONG money) = 0;
// 		//游戏中弃牌结果
// 		virtual void giveUpCardResp(BYTE seatNo) = 0;
// 		//要牌结果
// 		virtual void needCardResp(BYTE seatNo, BYTE value, int points) = 0;
// 		//停牌结果
// 		virtual void stopCardResp(BYTE seatNo) = 0;
		//设置下一个是自己叫牌时按钮状态
		virtual void setCallingbtnStatus(bool MeIsNt, bool bCanDivide) = 0;
		//隐藏庄家标志
		virtual void hideNtstates(BYTE seatNo) = 0;
		//提示信息
		virtual void setMsg(const std::string &msg) = 0;
		//离开桌子
		virtual void leaveDesk() = 0;
		//重置数据
		virtual void refreshData() = 0;


		/* 计时器*/

		//设置玩家等待时间
		virtual void setWaitTime(BYTE seatNo, BYTE time, bool visible) = 0;
		virtual void stopTimer() = 0;
		virtual void starTimer(BYTE timeID, float dt, BYTE seateNo) = 0;

	};
}


#endif