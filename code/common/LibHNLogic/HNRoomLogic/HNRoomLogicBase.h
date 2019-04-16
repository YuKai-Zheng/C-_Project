#ifndef __HNRoomLogicBase_H__
#define __HNRoomLogicBase_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetProtocol/HNMatchMessage.h"
#include "HNBaseType.h"
#include <string>
#include <vector>
#include "HNNetProtocol/HNRoomMessage.h"
#include "HNNetProtocol/HNNetMessageHead.h"
#include "HNRoom/HNRoomMessageDelegate.h"
#include "HNPlatform/HNPlatformNotifyDelegate.h"

namespace HN {
class IHNRoomLogicBase {
public:
    virtual void onRoomLoginCallback(bool success, const std::string& message, UINT roomID) {}
    virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {}
    virtual void onRoomQueueSitCallback(bool success, const std::string& message, UINT roomID) {}
    virtual void onRoomSitUpCallback(bool success, const std::string& message, BYTE destNo, BYTE seatNo) {}
    virtual void onRoomUserCome(INT userID) {}
    virtual void onRoomUserLeft(INT userID) {}
    virtual void onRoomDisConnect(const std::string &message) {}
};

class IRoomMessageDelegate;
/*
 * redbird room logic base.
 */
class HNRoomLogicBase: public HNLogicBase, public IRoomMessageDelegate,public IPlatformMessageDelegate {
public:
    virtual void start() override;
    virtual void stop() override;
    void requestLogin(UINT roomID);
    void requestSit(BYTE deskNo);
    void requestSit(BYTE deskNo, BYTE seatNo);
    void requestJoinQueue();
public:
    // for match
    virtual void onGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange)override {}
    virtual void onGameUserContset(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange) {}
    virtual void onGameContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward) {}
    virtual void onGameContestKick()override {}
    virtual void onGameContestWaitOver()override {}
    virtual void onGameContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards)override {}
    virtual void onGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople)override {}
    virtual void onGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord)override {};

    // for chat
    virtual void onUserChatMessage(void* object, INT objectSize)override {}

    // for user action
    virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)override;
    virtual void onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)override;
    virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)override;
    virtual void onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation)override {}

    virtual void onDisConnect() override;
    virtual void onConnectMessage(bool result) override;
    virtual void onLoginMessage(bool result, UINT dwErrorCode) override;
    virtual void onLoginFinishMessage() override;
    virtual void onUserComeMessage(UserInfoStruct* user) override;
    virtual void onUserLeftMessage(UserInfoStruct* user) override;
    virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
    virtual void onGamePointMessage(void * object, INT objectSize) override;
public:
    HNRoomLogicBase(IHNRoomLogicBase* callback);
    virtual ~HNRoomLogicBase();
protected:
    IHNRoomLogicBase* _callback;
    UINT _roomID;
};
}

#endif
