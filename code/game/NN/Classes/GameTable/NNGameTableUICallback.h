#ifndef __NN_GameTable_BRNNTableUICallback_h__
#define __NN_GameTable_BRNNTableUICallback_h__

#include "NNCommonDef.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace NN
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
        virtual void leaveTheGame() = 0;
        //进入或者短线重连恢复游戏桌子场景
        //virtual void resumeTheGameFree(TagGameStation_WaiteAgee* pcene, BYTE gameStation) = 0;
        virtual void resumeTheCatchBaker(tag_s2c_station_catch_banker* pMsg, BYTE bGameStation) = 0;
        virtual void resumeTheGameBet(tag_s2c_station_bet* pMsg, BYTE bGameStation) = 0;
        virtual void resumeTheGameSendCard(tag_s2c_station_send* pMsg, BYTE bGameStation) = 0;
        
        // 		//玩家聊天
        // 		virtual void showUserWords(MSG_GR_RS_NormalTalk* normalTalk) = 0;
        // 		//赠送礼物
        // 		virtual void giveGift(MSG_PROP_GIVE* pcene, int resultCode) = 0;
        //
        // 		//系统消息
        // 		virtual void showSystemWords(char* talk) = 0;
        
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        
        virtual void addUser(BYTE seatNo, bool bMe) = 0;
        //配置包
        virtual void onGameConfig(tag_s2c_config* msg) = 0;
        //准备
        virtual void onGamePrepare(tag_s2c_prepare* msg) = 0;
        //抢庄
        virtual void onGameCatchBanker(tag_s2c_catch_banker* msg) = 0;
        //确定庄家
        virtual void onGameConfirmBanker(tag_s2c_confirm_banker_two* msg) = 0;
        //押注消息
        virtual void onGameBet(tag_s2c_bet_two* msg) = 0;
        //发牌
        virtual void onGameSendCard(tag_s2c_send_card* msg) = 0;
        //ok
        virtual void onGameOpenOk(tag_s2c_open_card_ok* msg) = 0;
        //开牌
        virtual void onGameOpenCard(tag_s2c_open_card* msg) = 0;
        //结算
        virtual void onGameResult(tag_s2c_result* msg) = 0;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) = 0;
		//通知
		virtual void onGameNotify(tag_s2c_notify* msg) = 0;
		//奖池总额
		virtual void onGamePoolMoney(tag_s2c_reward_pool* msg) = 0;
		//获奖记录
		virtual void onGameRewardGetRecord(tag_s2c_reward_get_record* msg) = 0;
		//个人贡献
		virtual void onGamePersonalContribution(tag_s2c_personal_contribution* msg) = 0;
		//自己状态
		virtual void onGameUpdatPlayerState(tag_s2c_player_state* msg) = 0;
		//输赢标记
		virtual void onGameResultTag(bool bResult){};
		//vip游戏对局记录
		virtual void onVipGameResultLog(VipPlayerGameLog gameLog[]){};
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
    };
    
}

#endif // __BRNN_GameTable_BRNNTableUICallback_h__
