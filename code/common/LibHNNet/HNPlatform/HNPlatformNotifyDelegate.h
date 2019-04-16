#ifndef __HN_PlatformNotifyDelegate_H__
#define __HN_PlatformNotifyDelegate_H__

#include "HNBaseType.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include <string>

namespace HN {
class IPlatformMessageDelegate {
public:
    // 网络断线
    virtual void onHandleDisConnect() {}
    // 房间列表处理消息
    virtual void onHandleRoomListMessage() {}
    // 游戏列表处理消息
    virtual void onHandleGameListMessage() {}
    // 平台连接处理消息
    virtual void onHandleConnectMessage(bool result) {}
    // 平台登录处理消息
    virtual void onHandleLoginMessage(bool result, UINT dwErrorCode, const std::string& msg = "") {}
    // 平台游戏通知
    virtual void onHandlePlatformNotifyMessage(const std::string& message) {}
    // 平台在线总人数更新消息处理
    virtual void onHandlePlatformUserCountMessage(UINT uOnLineCount) {}
    // 房间人数更新
    virtual void onHandleRoomUserCountMessage(UINT roomID, UINT peopleCount, UINT virtualCount) {}
    // 平台游戏在线人数更新消息处理
    virtual void onHandleGameUserCountMessage(DL_GP_RoomListPeoCountStruct* userCount) {}
    // 平台注册
    virtual void onHandleUserRegisterMessage(MSG_GP_S_Register* registerStruct, UINT ErrorCode) {}
    // 新闻系统消息
    virtual void onNewsMessage(MSG_GR_RS_NormalTalk* pData) {}

    virtual void onHandleUpdateUserInfo() {}
    //游戏公告
    virtual void onHandlePlatformNotice(const std::string message) {}
    
    
public:
    std::string name;
};
}

#endif	//__HN_PlatformNotifyDelegate_H__

