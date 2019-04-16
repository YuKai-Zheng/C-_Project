#ifndef __ThirteenZhang_Game_Table_UI_Callback_H__
#define __ThirteenZhang_Game_Table_UI_Callback_H__

//#include "HNNetExport.h"
//#include "HNLogicExport.h"
#include "ThirteenZhangMessageHead.h"
#include <string>
#include <vector>
#include "HNRoomLogic/HNGameLogicBase.h"
#include "HNBaseType.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace ThirteenZhang
{
    /*
     * game ui callback
     */
    
    class IGameTableUICallback : public HN::IHNGameLogicBase
    {
    public:
        // 玩家坐下
        virtual void onAddPlayer(BYTE lSeatNo, INT userID, bool self) = 0;
        // 玩家站起
        virtual void onRemovePlayer(BYTE lSeatNo, LLONG userID, bool self) = 0;
        //  强制离开
        virtual void forceLeaveTheGame() = 0;
        
    public:
        // 玩家准备
        virtual void onGameAgree(BYTE lSeatNo, bool self) = 0;
        //// 通知抢庄
        //virtual void onGameNoticeRobNt(BYTE lSeatNo, bool self) = 0;
        //// 通知抢庄结果
        //virtual void onGameRobNtResult(BYTE lSeatNo, bool robResult, bool self) = 0;
        //// 确定庄家位置
        //virtual void onGameMakeSureNt(BYTE lSeatNo, bool self) = 0;
//        // 发牌
//        virtual void onGameSendCard(const std::vector<BYTE>& cards) = 0;
        // 提示出现特殊牌型
        virtual void onGameHasSpecialCard(const std::vector<int>& specialShapeArray, const std::vector<BYTE>& cards) = 0;
//		//手动一键摊牌
        virtual void onGameManualOpenCard(void* manualResult) {}
        // 通知开牌
        virtual void onGameNoticeOpenCard() = 0;
        // 开牌结果
        virtual void onGameOpenCardResult(BYTE lSeatNo) = 0;
        // 比牌
        virtual void onGameCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) = 0;
        // 开火
        virtual void onGameOpenFire(S_C_OpenFire* scOpenFire) = 0;
        // 特殊牌开牌
        virtual void onGameOpenSpecialShape(S_C_OpenSpecialShape* scSpecialShape) = 0;
        // 游戏结算
        virtual void onGameResult(const S_C_GameResult* pObject) = 0;
        //金币不足，强制退出
        virtual void IShowTipExit(long long i64Money) = 0;
        // 更新游戏底注
        virtual void onGameDizhuValue(const int value) = 0;
        //五花牛全服公告
        virtual void onGameNotifyWuHuaNiu(NN::tag_s2c_notify* msg) = 0;
		//是否播放开始动画（玩家全部准备后）
		virtual void onIsCanPlayBeginAni(bool isCanPlay) = 0;
		//输赢标记
		virtual void onGameResultTag(bool bResult){};
		//vip游戏对局记录
		virtual void onVipGameResultLog(VipPlayerGameLog gameLog[]){};
    public:
        virtual void onGSFree(const bool userReady[PLAY_COUNT]) = 0;
        //virtual void onGSRobNt(BYTE currRobUser) = 0;

        virtual void onGSSendCard(const std::vector<BYTE>& cards) = 0;
        virtual void onGSOpenCard(const std::vector<BYTE>& cards, const BYTE heapCard[3][5]) = 0;
        virtual void onGSCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) = 0;
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) = 0;
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) = 0;
    public:
        // 聊天消息
        virtual void onUserChat(INT sendID, INT targetID, const std::string& msg) = 0;
        
        
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
