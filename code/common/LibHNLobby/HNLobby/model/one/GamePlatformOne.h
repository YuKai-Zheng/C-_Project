#ifndef __GAME_PLATFORM_ONE_H__
#define __GAME_PLATFORM_ONE_H__

#include "HNLogicExport.h"
#include "HNNetProtocol/HNComStruct.h"
#include "UI/Base/HNLayer.h"
#include "../../component/delegate/MenuDelegate.h"
#include "../../component/delegate/GameListDelegate.h"
#include "../../component/delegate/RoomListDelegate.h"
#include "../../component/delegate/DeskListDelegate.h"
#include "../../component/delegate/GameTypeDelegate.h"
#include "HNPlatformLogic/HNPlatformRoomList.h"
#include "HNPlatformLogic/HNPlatformGameList.h"


class GamePlatformOne : public HNLayer, public PlatformDelegate, public HNSpreadLogic::Delegate {
public:
    GamePlatformOne();
    virtual ~GamePlatformOne();
    CREATE_FUNC(GamePlatformOne);

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
    //公告
    virtual void platformNoticeList(const std::string message) override;

    virtual void handleReturnButton() override;
    virtual void openNewYearActivity()override;
public:
    std::string showHint();
    //请求显示
    void reqIsShow(std::string buttonKey);
    //获取游戏title通过房间ID
    void getRoomSpriteByRoomName(char name[], char fish[] ,char* gameName);

    //创建房间列表
    void createRoomListLayer();
    bool dealVirualSockteMsg(HNSocketMessage* socketMessage);
protected:
    LayerType				m_type;
    MenuDelegate*			m_topMenuDelegate;
    MenuDelegate*			m_bottomMenuDelegate;
    RoomListDelegate*		m_roomListDelegate;
    GameListDelegate *		m_gameListDelegate;
    DeskListDelegate*		m_deskListDelegate;

    GameTypeDelegate*       m_gameTypeDelegate;
    
    HNPlatformRoomList*     _roomListLogic;
    HNPlatformGameList*     _gameListLogic;

    virtual void createGameTypeLayer();
private:
    INT						_httpTime;
};

#endif // __GAME_PLATFORM_ONE_H__
