#ifndef __RedPacket_GameTableUICallback_h__
#define __RedPacket_GameTableUICallback_h__

#include "RedPacketCommonDef.h"
#include "HNRoomLogic/HNGameLogicBase.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

namespace RedPacket
{
    class  GameTableUICallback : public IHNGameLogicBase
    {
    public:
        virtual void leaveTheGame() = 0;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //
        virtual void addUser(BYTE seatNo, bool bMe) = 0;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) = 0;
        
        //新红包通知
        virtual void onNotifyNewPacketResponse(tag_s2c_RedPacketListReq* packetRes) {}
        //发送红包回复
        virtual void onSendPacketResponse(tag_c2s_EmitRedPacketReq* packetRes) {}
        //抢红包回复
        virtual void onOpenPacketResponse(tag_c2s_RobRedPacketReq* packetRes) {}
//        //当前有玩家抢红包的消息
//        virtual void onNotifyPlayerOpenPacketResponse(RobRedPacketUserInfo* packetRes) {}
        //红包被抢的情况消息回复
        virtual void onNotifyPacketOpenDetailsResponse(tag_c2s_RedPacketRobLogReq* packetRes) {}
        //抢红包玩家消息回包
        virtual void onNotifyPacketOpenPlayerDetailResponse(RobRedPacketUserInfo* packetRes) {}
        //个人抢红包历史信息
        virtual void onGetMyPacketHistiryResponse(tag_s2c_MyHistoryRobLogReq* packetRes) {}
        //更新在线玩家列表
        virtual void onUpdateAllPlayerListResponse(std::vector<tag_s2c_PlayerListMsgReq> allPlayer) {}
        //红包领取完或者过期提醒
        virtual void onNotityPacketExpiredMessage(tag_s2c_RobRedPacketOverReq* packetRes) {}
        //更新身上的金币
        virtual void onUpdateMyPacketMoney(LLONG money) {}
        //更新房间基础信息展示
        virtual void onUpdateRoomBaseInfo() {}
        
    };
    
}

#endif
