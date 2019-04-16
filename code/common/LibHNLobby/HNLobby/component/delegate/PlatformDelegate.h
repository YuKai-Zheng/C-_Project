#ifndef _PLATFORM_DELEGATE_H_
#define _PLATFORM_DELEGATE_H_

#include "HNNetProtocol/HNComStruct.h"
#include "BaseDelegate.h"
#include "../../globel.h"

/*

*/
class PlatformDelegate : public BaseDelegate {
public:
    //创建桌子列表层
    virtual void createDeskListLayer(ComRoomInfo *roomInfo) = 0;

    //创建游戏列表层
    virtual void createGameListLayer() = 0;
    virtual void updateGameList(bool success, const std::string &message) = 0;
    virtual void updateGameUserCount(UINT Id, UINT count) = 0;

    //创建房间列表层
    virtual void closeRoomList() {};
    virtual void updateRoomList(bool success, const std::string& message) {};
    virtual void updateRoomUserCount(UINT roomID, UINT userCount) {};


    //创建游戏列表和房间（腾讯版）
    virtual void createGameListAndRoom() {};

    //设置图层的类型
    virtual void setLayerType(LayerType type) {};

    //处理顶部菜单的返回按钮
    virtual void handleReturnButton() {};

    virtual void openNewYearActivity() {};
};

#endif //_PLATFORM_DELEGATE_H_
