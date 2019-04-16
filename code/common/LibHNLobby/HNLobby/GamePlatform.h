#ifndef __PLATFORMFRAME_SCENE_H__
#define __PLATFORMFRAME_SCENE_H__

#include "HNLogicExport.h"
#include "HNNetProtocol/HNComStruct.h"
#include "component/delegate/RoomListDelegate.h"
#include "component/delegate/GameListDelegate.h"
#include "component/delegate/MenuDelegate.h"
#include "component/delegate/DeskListDelegate.h"
#include "component/delegate/PlatformDelegate.h"
#include "UI/Base/HNLayer.h"
#include "globel.h"


class GamePlatform : public HNLayer,public PlatformDelegate,public HNSpreadLogic::Delegate {
public:
    GamePlatform();
    virtual ~GamePlatform();
    CREATE_FUNC(GamePlatform);

    virtual bool init() override;
    virtual void onEnter()override;
    virtual void onEnterTransitionDidFinish()override;
    virtual void onExitTransitionDidStart()override;
    virtual void onExit() override;
    virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;
    virtual void initPlatform();

public:
    virtual void createGameListLayer()override;
    virtual void updateGameList(bool success, const std::string &message) override;
    virtual void updateGameUserCount(UINT Id, UINT count) override;
    virtual void updateRoomList(bool success, const std::string& message) override;
    virtual void updateRoomUserCount(UINT roomID, UINT userCount) override;
    virtual void createDeskListLayer(ComRoomInfo *roomInfo)override;

    //断线通知
    virtual void platformDisConnect(const std::string& message) override;
    //大厅广播通知
    //virtual void platformNews(const std::string& message)override;
	virtual void platformNews(int iType, const std::string &message, int iCount)override;
    virtual void setLayerType(LayerType layerType)override;

public:
    std::string showHint();
    //请求显示
    void reqIsShow(std::string buttonKey);
    //获取游戏title通过房间ID
    void getRoomSpriteByRoomName(char name[], char fish[] ,char* gameName);

protected:
    LayerType				m_type;
    MenuDelegate*			m_topMenuDelegate;
    MenuDelegate*			m_bottomMenuDelegate;
    RoomListDelegate*		m_roomListDelegate;
    GameListDelegate *		m_gameListDelegate;
    DeskListDelegate*		m_deskListDelegate;

    HNPlatformRoomList*     _roomListLogic;
    HNPlatformGameList*     _gameListLogic;

    virtual void createGameTypeLayer() {};
private:
    INT						_httpTime;
};

#endif // __PLATFORMFRAME_SCENE_H__
