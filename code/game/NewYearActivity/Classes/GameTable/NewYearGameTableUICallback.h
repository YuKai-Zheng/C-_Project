#ifndef __NEWYEAR_GameTable_BRNNTableUICallback_h__
#define __NEWYEAR_GameTable_BRNNTableUICallback_h__

#include "NewYearMessageHead.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

namespace NewYearActivity
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
//         virtual void leaveTheGame() = 0;
//         //进入或者短线重连恢复游戏桌子场景
//         //virtual void resumeTheGameFree(TagGameStation_WaiteAgee* pcene, BYTE gameStation) = 0;
//         virtual void resumeTheCatchBaker(le_s2c_station_catch_banker* pMsg, BYTE bGameStation) = 0;
// 		virtual void resumeSetMoney(le_s2c_station_set_money* msg, BYTE bGameStation) = 0;
// 		virtual void resumeSetBomb(le_s2c_station_set_bomb* msg, BYTE bGameStation) = 0;
// 		virtual void resumeOpenLE(le_s2c_station_open_le* msg, BYTE bGameStation) = 0;
// 		virtual void resumeShowResult(le_s2c_station_result* msg, BYTE bGameStation) = 0;
// 
//         
//         //玩家站起
//         virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
//         //玩家坐下
//         virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
//         
//         virtual void addUser(BYTE seatNo, bool bMe) = 0;
		virtual void onGameUpdateSignInfo(tag_s2c_SignInfo* msg) = 0;
		virtual void onGameUpdateSignResult(tag_s2c_SignResult* msg) = 0;
		virtual void onGameUpdateDriveNianShouInfo(tag_s2c_DriveNianShouInfo* msg) = 0;
		virtual void onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) = 0;
		virtual void onGameTurnTableResult(int winArea) = 0;
		virtual void onGameTurnTableToday(int flag) = 0;
    };
    
}

#endif // __BRNN_GameTable_BRNNTableUICallback_h__
