#ifndef __PLATFORM_V3_H__
#define __PLATFORM_V3_H__

#include "../GamePlatform.h"

class GamePlatformV3 : public GamePlatform {
public:
    virtual bool init() override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;
	
	CREATE_FUNC(GamePlatformV3);

public:
	virtual void createGameTypeLayer()override;
    virtual void initPlatform()override;
	virtual void openNewYearActivity()override;
    //创建桌子列表
    virtual void createDeskListLayer(ComRoomInfo *roomInfo)override;
    virtual void updateGameList(bool success, const std::string &message) override;
    virtual void updateGameUserCount(UINT Id, UINT count) override;
    virtual void updateRoomList(bool success, const std::string& message) override;
    virtual void updateRoomUserCount(UINT roomID, UINT userCount) override;
    virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;

    virtual void createGameListAndRoom()override;

	virtual void setLayerType(LayerType layerType) override;

public:
    //顶部功能栏的变幻动作
    void showTopAction(bool isOut, CallFunc* func);
    //底部功能栏的变幻动作
    void showBottomAction(bool isOut, CallFunc* func);
    //创建房间列表
    void createRoomListLayer();
    //创建游戏列表
    void createGameListLayer()override;

private:
};

#endif // __PLATFORM_V3_H__
