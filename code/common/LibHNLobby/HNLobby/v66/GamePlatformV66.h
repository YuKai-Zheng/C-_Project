#ifndef __PLATFORM_V66_H__
#define __PLATFORM_V66_H__

#include "../model/one/GamePlatformOne.h"
//#include "../component/delegate/GameListRoomDelegate.h"

class IGameRank;

class GamePlatformV66 : public GamePlatformOne {
public:
    virtual bool init() override;
    //virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;
	CREATE_FUNC(GamePlatformV66);

public:
    //virtual void initPlatform()override;
    ////创建桌子列表
    //virtual void createDeskListLayer(ComRoomInfo *roomInfo);
    virtual void updateGameList(bool success, const std::string &message) override;
    //virtual void updateGameUserCount(UINT Id, UINT count) override;
    virtual void updateRoomList(bool success, const std::string& message) override;
    //virtual void updateRoomUserCount(UINT roomID, UINT userCount) override;
    //virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;
    //virtual void createGameListAndRoom()override;

public:
    virtual void handleReturnButton() override;
	virtual void setLayerType(LayerType layerType) override;

public:
    ////顶部功能栏的变幻动作
    //void showTopAction(bool isOut, CallFunc* func);
    ////底部功能栏的变幻动作
    //void showBottomAction(bool isOut, CallFunc* func);
    ////创建房间列表
    //void createRoomListLayer();
    ////创建游戏列表
    virtual void createGameListLayer() override;
	void createRoomListLayer();

    virtual void openNewYearActivity()override;
private:
	//void loadRankUI();
	//void rankCallBack(Ref* pSender, Widget::TouchEventType type);
    /*GameListRoomDelegate* m_gameListRoomDelegate;*/
};

#endif // __PLATFORM_V99_H__
