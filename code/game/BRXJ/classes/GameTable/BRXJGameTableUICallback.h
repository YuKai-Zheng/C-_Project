#ifndef  __BRXJGameTableUICallBack_H__
#define __BRXJGameTableUICallBack_H__

#include "HNBaseType.h"
#include "BRXJMessageHead.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"

#include "../../NN/Classes/GameMessage/NNCommonDef.h"

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BRXJ
{
	class GameTableUICallback : public HN::IHNGameLogicBase
	{
		public:
			//计数器
			virtual void starTime(int time) = 0;
			//杀计数器
			virtual void killTime() = 0;
			//离开游戏
			virtual void levelGame() = 0;
			//下局离开
			virtual void nextLevelGame() = 0;
			//游戏消息
			virtual void IShowNoticeMessage(const std::string& message) = 0;
			// 庄家申请
			virtual void IApplyNT(bool isNT) = 0;
			//更新庄家列表
			virtual void IUpdateNTList() = 0;
			//更新玩家信息
			virtual void upDataPlayerInfo(std::string name, LLONG money, LLONG totle) = 0;
			virtual void upDataPlayerInfo(LLONG money, LLONG totle) = 0;
			//更新庄家信息
			virtual void upDataNTInfo(INT useId, INT logoid ,std::string name, LLONG money, LLONG totle, int zhuangCount) = 0;
			virtual void upDataNTInfo(LLONG money, LLONG totle) = 0;
			//隐藏庄家信息
			virtual void hideNtInfo(bool flag) = 0;
			//清理桌面
			virtual void clearTable(bool flag) = 0;
			//发牌
			virtual void ISendCard(bool animate) = 0;
			//游戏状态
			virtual void gameStation(BYTE station) = 0;
			//更新区域下注
			virtual void upDataAreaTotleMoney(LLONG* quyuzhu) = 0;
			virtual void upDataAreaMoney(LLONG*quyuzhu) = 0;
			//显示胜利区域
			virtual void showWinArea(BYTE idx) = 0;
			//初始化牌值
			virtual void initCardValue(BYTE value,int turn,int idx) = 0;
			//筹码动画
			virtual void chipAnimation(bool flag,int type,int area) = 0;
			//动画
			virtual void allAnimation(bool flag) = 0;
			//更新路子信息
			virtual void upDataLuzi(TGameLuzi *data) = 0;
			//灰化筹码按钮
			virtual void grayEffectBet(bool flag) = 0;
			//显示牌型
			virtual void showCardShap(int area, int count) = 0;
			//牌值
			virtual void recoveryCards(BYTE value) = 0;
			//重复下注
			virtual void repeatNote() = 0;
			//设置提示框
			virtual void showSettlementDialog(S_C_GameResult* pData) = 0;
			//游戏消息
			virtual void onGameNotify(NN::tag_s2c_notify* msg) = 0;
			//播放游戏状态提示
			virtual void playEffectOnStation() = 0;
			//游戏提醒
			virtual void onGameTipExit(tag_s2c_tip_exit* msg) = 0;
			//开启重复下注
			virtual void openRepeatNote(bool flag) = 0;
        
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


#endif // __BRXJGameTableUICallBack_H__
