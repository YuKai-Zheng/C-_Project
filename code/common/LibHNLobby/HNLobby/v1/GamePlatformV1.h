#ifndef __PLATFORM_v1_H__
#define __PLATFORM_v1_H__

#include "../GamePlatform.h"

class GamePlatformV1 : public GamePlatform {
public:
    virtual bool init() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;
    CREATE_FUNC(GamePlatformV1);

public:
    virtual void handleReturnButton()override;
    virtual void initPlatform()override;
    virtual void createDeskListLayer(ComRoomInfo *roomInfo)override;
    virtual void updateGameList(bool success, const std::string &message) override;
    virtual void updateGameUserCount(UINT Id, UINT count) override;
    virtual void updateRoomList(bool success, const std::string& message) override;
    virtual void updateRoomUserCount(UINT roomID, UINT userCount) override;
    virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;
    virtual void openNewYearActivity()override;

public:
    //顶部功能栏的变幻动作
    void showTopAction(bool isOut);
    //底部功能栏的变幻动作
    void showBottomAction(bool isOut);
    //创建房间列表
    void createRoomListLayer();
    //创建游戏列表
    void createGameListLayer();

private:

};

#endif // __PLATFORM_v1_H__
