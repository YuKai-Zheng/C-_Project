#ifndef __STUD_Game_Table_UI_Callback_H__
#define __STUD_Game_Table_UI_Callback_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "StudMessageHead.h"

#include "../../NN/Classes/GameMessage/NNCommonDef.h"
/************************************************************************/
/* 类型：回调接口                                                        */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口                        */
/* 说明：带user是针对用户，没有则是针对牌桌                                */
/* -1的参数不用改变                                                      */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace STUD
{
	class GameTableUICallback : public IHNGameLogicBase
	{
	public:

		virtual void loadUser(BYTE seatId, LLONG userId)							= 0;
		virtual void showUserUp(BYTE seatNo)										= 0;
        virtual void startTimer(BYTE seatId, BYTE leftTime, BYTE totalTime) {}
		virtual void startTimer(BYTE seatId, BYTE leftTime)	{}
        virtual void stopTimer(BYTE seatId) {}
		virtual void initGameUi(GameBeginStruct* buff)								= 0;
		virtual void recordDeskInfo(LLONG zhu[]) = 0;

		virtual void updateHandcard(SendOneTurnCardStruct* buff)					= 0;
		virtual void updateHandcard(SendOneTurnCard* buff)							= 0;
		virtual void updateHandcard(BYTE handcardCount, BYTE handcards[PLAY_COUNT][HANDCARD_COUNT])					= 0;
		virtual void shuffleCards(int recycle)										= 0;
		virtual void recycleChips(BYTE seat)										= 0;
		virtual void recycleChipsById(LLONG userId)										= 0;

		virtual void showMaxCardMark(bool visible, BYTE buttonSeat)			= 0;
		virtual void updateNote(BYTE seatId, LLONG money)							= 0;
		virtual void playNoteAudio(BYTE seatId, BYTE audioType)						= 0;
		virtual void setStartBtnVisible(bool visible)								= 0;
		virtual void setBetBtnEnabled(bool follow, bool allin, bool note, bool quit) = 0;
		virtual void setBetBtnEnabled(bool isEnable)								= 0;
		virtual void setBetBtnText(LLONG baseNote) = 0;
		virtual void setBetAndOtherBtnOpen(bool isEnable) = 0;
		virtual void setAreadySpriteVisible(bool visible, BYTE bDeskNo) = 0;// 准备显示
		virtual void setAllAreadySpriteVisible(bool visible) = 0;// 隐藏所有准备精灵显示
		virtual void setMesaVisible(bool visible) = 0; //隐藏台面上几个精灵的显示
		virtual void userMoneyNotEnouth(tag_s2c_tip_exit* pData) = 0;
		virtual void playScanner() = 0;
		//不同牌型播放音效
		virtual void userPlayerCardTypeEffect(BYTE type, float dt) = 0; 
		//勾选下局离开
		virtual void nextTurnLeavl() = 0;
		//恢复玩家信息
		virtual void recoveryUserDataCallBack(BYTE seatId,bool isVisible) = 0;
		//定时器显示
        virtual void clockHide(bool visibility) {}
		virtual void HidTopPlayerOneCard() = 0;

		//通知
		virtual void onGameNotify(NN::tag_s2c_notify* msg) = 0;
		//更新历史输赢和单笔输赢
		virtual void upDataHistoyAndLastMoney(LLONG history, LLONG last) = 0;

		//清空下注
		virtual void resetNote() = 0;
        
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

#endif
