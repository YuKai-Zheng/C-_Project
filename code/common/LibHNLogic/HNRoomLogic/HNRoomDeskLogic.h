#ifndef __HNRoomDeskLogic_H__
#define __HNRoomDeskLogic_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetExport.h"

namespace HN {
class IHNRoomDeskLogic {
public:
    virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {}
    virtual void onRoomDeskLock(BYTE deskNo) {}
    virtual void onRoomDeskUnLock(BYTE deskNo) {}
    virtual void onRoomUserCome(INT userID) {}
    virtual void onRoomUserLeft(INT userID) {}
    virtual void onRoomDeskUserCountChanged(){}
    virtual void onRoomDeskUserCountChanged(MSG_GR_R_UserUp* pData) {}
	virtual void onDeskState(MSG_GR_DeskStation* pDate){}
	virtual void onDeskState(BYTE bDeskIndex){}

};


class HNRoomDeskLogic:public HNLogicBase, public IRoomMessageDelegate {
public:
    virtual void start() override;
    virtual void stop() override;
    void requestSit(BYTE deskNo);
    void requestSit(BYTE deskNo, BYTE seatNo);
public:
    HNRoomDeskLogic(IHNRoomDeskLogic* callback);
    virtual ~HNRoomDeskLogic();
public:
    virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
	virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;
	virtual void onDeskStateMessage(MSG_GR_DeskStation* pDate) override;
	virtual void onDeskStateMessage(BYTE bDeskIndex) override;
    virtual void onLockDeskMessage(BYTE deskNo) override;
    virtual void onUnLockDeskMessage(BYTE deskNo) override;
    virtual void onUserComeMessage(UserInfoStruct* user) override;
    virtual void onUserLeftMessage(UserInfoStruct* user) override;
protected:
    IHNRoomDeskLogic* _callback;
    UINT _roomID;
};
}

#endif