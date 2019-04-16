#ifndef __HN_RoomNotifyDelegate_H__
#define __HN_RoomNotifyDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <vector>

namespace HN {
class IRoomMessageDelegate	: public IGameMatchMessageDelegate, public IGameChartMessageDelegate, public IUserActionMessageDelegate {
public:
    virtual ~IRoomMessageDelegate() {}
    // 网络断开
    virtual void onDisConnect() {}
    // 房间连接成功
    virtual void onConnectMessage(bool result) {}
    // 房间登录成功
    virtual void onLoginMessage(bool result, UINT dwErrorCode) {}
    // 房间登录完成
    virtual void onLoginFinishMessage() {}
    // 用户进入房间
    virtual void onUserComeMessage(UserInfoStruct* user) {}
    // 用户离开房间
    virtual void onUserLeftMessage(UserInfoStruct* user) {}
    // 房间其他消息
    virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
        return true;
    }
    // 结算消息
    virtual void onGamePointMessage(void * object, INT objectSize) {}
    // 封桌
    virtual void onLockDeskMessage(BYTE deskNo) {}
    // 解封
    virtual void onUnLockDeskMessage(BYTE deskNo) {}
    //坐姿状态
    virtual void onDeskStateMessage(MSG_GR_DeskStation* pDate) {};
	//
	virtual void onDeskStateMessage(BYTE bDeskIndex) {};

};
}

#endif	//__HN_RoomNotifyDelegate_H__

