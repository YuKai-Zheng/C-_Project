#ifndef __BRNNGameTableLogic_h__
#define __BRNNGameTableLogic_h__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "BRNNMessageHead.h"
#include "BRNNGameTableUICallback.h"

//#include "NewYearInGameMessageHead.h"

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BRNN {
class GameTableUICallback;

class GameTableLogic : public HN::HNGameLogicBase {
public:
GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate);
virtual ~GameTableLogic();
public:
////获取user信息 通过座位号
//UserInfoStruct* getUserInfoBySeatNo(BYTE seatNo);
//获取本桌所有用户
HNGameTableUsersData* getDeskUserList() {
	return _deskUserList;
}

//void initTableLogicData();
//virtual BYTE viewToLogicSeatNo(BYTE vSeatNo);
//virtual BYTE logicToViewSeatNo(BYTE lSeatNo);

public://处理服务端收到的消息
       //virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
       //virtual void dealGameStartResp(BYTE bDeskNO);
       //virtual void dealGameEndResp(BYTE bDeskNO);
virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
//virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
virtual void dealGameStationResp(void* object, INT objectSize);
virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk);

void onGameNotify(NN::tag_s2c_notify* msg);
//virtual void dealSystemMessage(MSG_SystemTalk* pData);
public://客服端=>服务端发出的指令
       //下注
void sendBet(char* xiazhu, INT iSize);
////上庄
//void sendGetDealer(char* dealer , INT iSize);

//换座
void sendChangeSeat(BYTE byPosition);
////站起
//void sendStandUpMessage(BYTE byPosition);
//通知
private:
GameTableUICallback* _gameUICallback;
INT _NTSeatNo;
    
public:
    //赶年兽
    void sendDriveNianShou(int nUserID);
    
};

}


#endif // __BRNNGameTableLogic_h__
