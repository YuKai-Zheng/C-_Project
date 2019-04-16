#ifndef __HNPlatformRoomList_H__
#define __HNPlatformRoomList_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"
#include "HNNetExport.h"
#include <string>
#include "HNLobby/component/delegate/RoomListDelegate.h"
#include "HNLobby/component/delegate/PlatformDelegate.h"

namespace HN {
/*
 * redbird platform room interface.
 */
class IHNPlatformRoomList: public IHNPlatformLogicBase {
public:
    virtual void onPlatformRoomListCallback(bool success, const std::string& message) {}
    virtual void onPlatformRoomUserCountCallback(UINT roomID, UINT userCount) {}
    virtual void onRoomSitCallback(bool success, const std::string& message, INT userID, BYTE deskNo, BYTE seatNo) {}
};

/*
 * redbird platform room logic
 */
class HNPlatformRoomList : public HNPlatformLogicBase, public IRoomMessageDelegate { /*, public IHNPlatformRoomList */
public:
    HNPlatformRoomList(PlatformDelegate* delegate);
    virtual ~HNPlatformRoomList();
    //请求房间列表数据
    void requestRoomList();
    //void requestRoomLogin();
public:
    virtual void start() override;
    virtual void stop() override;
    // room list response.
    //对请求创建房间列表的响应
    virtual void onHandleRoomListMessage() override;
    //更新房间的玩家人数
    virtual void onHandleRoomUserCountMessage(UINT roomID, UINT peopleCount, UINT virtualCount) override;
    //// 用户坐下消息
    //virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

public:
    //virtual void onPlatformRoomListCallback(bool success, const std::string& message) override;
    //virtual void onPlatformRoomUserCountCallback(UINT roomID, UINT userCount) override;

protected:
    //IHNPlatformRoomList* _callback;
    //void roomRequestLogin();
    PlatformDelegate* m_delegate;
};
}

#endif