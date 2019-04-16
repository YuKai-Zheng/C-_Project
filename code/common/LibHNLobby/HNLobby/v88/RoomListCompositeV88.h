#ifndef _ROOM_LIST_COMPOSITE_V88_H_
#define _ROOM_LIST_COMPOSITE_V88_H_

#include "HNLobby/GameRoom.h"
#include "../pattern/factory/roomList/IRoomList.h"

/*

*/
class RoomListCompositeV88 :public IRoomList , HN::IHNRoomLogicBase, HN::IHNRoomDeskLogic{
public:
    RoomListCompositeV88();
	~RoomListCompositeV88();
	virtual void onEnter() override;
    virtual bool init()override;
    CREATE_FUNC(RoomListCompositeV88);

public:
	virtual void onRoomLoginCallback(bool success, const std::string& message, UINT roomID) override;
	virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) override;
    virtual void updateRoomUserCount(UINT roomID, UINT userCount)override;
    virtual void closeRoomList()override;
    virtual void openRoomList() override;
    virtual void execute()override;
public:			
	void enterRoomCallback(Ref* pSender, Widget::TouchEventType type);
    
    void roomListMoveIn(bool bIn);
public:
	void cancelTouchRoom(float delata);
    bool m_bIsTouch;

	vector<ComRoomInfo*>				m_vRoomInfo;
	vector<Button*>						m_vRoomItem;

	HNRoomLogicBase*	_roomLogic;
	HNRoomDeskLogic*	_deskLogic;
private:
	
    GameRoom* gameRoomLayer;

private:
    std::string converNumberWithWAN(LLONG number);
    
};

#endif //_ROOM_LIST_COMPOSITE_V99_H_
