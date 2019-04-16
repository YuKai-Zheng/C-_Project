#ifndef __BRNN_GameTable_BRNNTableUICallback_h__
#define __BRNN_GameTable_BRNNTableUICallback_h__

#include "BRNNMessageHead.h"
#include "HNRoomLogic/HNGameLogicBase.h"

#include "../../NN/Classes/GameMessage/NNCommonDef.h"
/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

//using namespace BRNN;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BRNN {
class GameTableUICallback : public IHNGameLogicBase {
public:
//离开游戏
    virtual void leaveTheGame() = 0;
//进入或者短线重连恢复游戏桌子场景
    virtual void resumeTheGameFree(TagGameState_WaiteAgee* pcene, BYTE gameStation) = 0;
    virtual void resumeTheGameBet(TagGameState_Bet* pcene, BYTE gameStation) = 0;
    virtual void resumeTheGameOpenCard(TagGameState_OpenCard* pcene, BYTE gameStation) = 0;
    virtual void resumeTheGameResult(TagGameState_ShowResult* pcene, BYTE gameStation) = 0;
//开始新一轮游戏
    virtual void startNewTurn(BEGIN_DATA* pcene) = 0;
//下注
    virtual void showBet(TagXiaZhu* pcene) = 0;
//开牌
    virtual void showCard(KAI_PAI* pcene) = 0;
//结算
    virtual void showSettlement(tagResult* pcene) = 0;
////换庄
//virtual void changeDealer(void* object) = 0;
//无庄等待
    virtual void waitForNoDealer(DENG_DAI* pcene) = 0;
//空闲等待
    virtual void waitForIdle(DENG_DAI* pcene) = 0;
//下注达到上限
    virtual void showUnableBet() = 0;
//上庄
    virtual void getOrQuitBanker(SHANG_ZHUANG* pcene) = 0;
//玩家聊天
    virtual void showUserWords(MSG_GR_RS_NormalTalk* normalTalk) = 0;
//赠送礼物
    virtual void giveGift(MSG_PROP_GIVE* pcene, int resultCode) = 0;
//获取合作上庄的组合列表
    virtual void getUionBankerList(SHANG_ZHUANG_UION_LIST* pcene) = 0;
//联合上庄上下庄
    virtual void uionBankerUpOrDown(SHANG_ZHUANG_UION* pcene) = 0;
////玩家站起
//virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
//玩家坐下
    virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
//系统消息
    virtual void showSystemWords(char* talk) = 0;
//设置下注上限
    virtual void setLIMIT(LLONG value) = 0;
//开始提示
    virtual void showStartMes() = 0;
//换座
    virtual void changeSeat(BYTE byStation, BYTE byLastStation, BYTE seatNo) = 0;
//站起
    virtual void playerStandUp(BYTE byStation, BYTE seatNo) = 0;
//提前发牌
    virtual void reachMaxBetLimit() = 0;
//获取庄家的座位号
    virtual int  getBankerSeat() = 0;
//连庄次数
    virtual Label*  getRemainDealer() = 0;
//等待变更庄家，再判断坐庄人
    virtual void judgeZhuangJiaMessage() = 0;
//提醒退出房间【可能因为金币不足】
    virtual void onGameTipExit(S_C_TIP_EXIT* msg)=0;
//通知
    virtual void onGameNotify(NN::tag_s2c_notify* msg) = 0;
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
//    //年兽触发
//    virtual void onGameShowNianShou() = 0;
//    //赶年兽结果
//    virtual void onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) = 0;
//    //对局获得鞭炮
//    virtual void onGameMatchGainFirecraker(int nUserID) = 0;
};

}

#endif // __BRNN_GameTable_BRNNTableUICallback_h__
