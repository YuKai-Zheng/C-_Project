#ifndef __HN_ProtocolExport_h__
#define __HN_ProtocolExport_h__

#include "HNNetProtocol/HNBaseCommand.h"
#include "HNNetProtocol/HNComStruct.h"
#include "HNNetProtocol/HNNetMessageHead.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "HNNetProtocol/HNRoomMessage.h"
#include "HNNetProtocol/HNBankMessage.h"
#include "HNNetProtocol/HNMatchMessage.h"
#include <vector>

// 比赛接口
class IGameMatchMessageDelegate
{
public:
	virtual ~IGameMatchMessageDelegate() {};

public:
	//////////////////////////////////////////////////////////////////////////
	// 转发到游戏
	//////////////////////////////////////////////////////////////////////////
	// 比赛初始化
	virtual void onGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange){}
	//比赛信息广播
	virtual void onGameContestNotic(MSG_GR_I_ContestInfo* contestInfo){}
	// 用户比赛信息
	virtual void onGameUserContset(MSG_GR_ContestChange* contestChange){}
	// 比赛结束
	virtual void onGameContestOver(MSG_GR_ContestAward* contestAward){}
	// 比赛淘汰
	virtual void onGameContestKick(){}
	// 等待比赛结束
	virtual void onGameContestWaitOver(){}

	//////////////////////////////////////////////////////////////////////////
	// 转发到房间
	//////////////////////////////////////////////////////////////////////////
	// 比赛奖励
	virtual void onGameContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards){}
	// 报名数量
	virtual void onGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople){}
	// 个人参赛纪录
	virtual void onGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord){}
};

// 聊天接口
class IGameChartMessageDelegate
{
public:
	virtual ~IGameChartMessageDelegate() {};
public:
	// 聊天消息
	virtual void onUserChatMessage(void* object, INT objectSize){}
};


// 用户行为接口
class IUserActionMessageDelegate
{
public:
	virtual ~IUserActionMessageDelegate() {};
	// 用户坐下消息
	virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user){}
	// 排队用户坐下
	virtual void onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users){}
	// 用户站起消息
	virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user){}
	// 用户断线
	virtual void onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation){}
};



#endif // __HN_ProtocolExport_h__
