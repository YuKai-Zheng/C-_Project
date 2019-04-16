#ifndef _ROOM_LIST_COMPOSITE_V66_H_
#define _ROOM_LIST_COMPOSITE_V66_H_

#include "HNLobby/GameRoom.h"
#include "../pattern/factory/roomList/IRoomList.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "UI/JackpotModel.hpp"
/*

*/
class RoomListCompositeV66 :public IRoomList , HN::IHNRoomLogicBase, HN::IHNRoomDeskLogic{
public:
    RoomListCompositeV66();
	~RoomListCompositeV66();
	virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool init()override;
    CREATE_FUNC(RoomListCompositeV66);

public:
	virtual void onRoomLoginCallback(bool success, const std::string& message, UINT roomID) override;
	virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) override;
    virtual void updateRoomUserCount(UINT roomID, UINT userCount)override;
    virtual void closeRoomList()override;
    virtual void openRoomList() override;
    virtual void execute()override;
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
public:			
	void enterRoomCallback(Ref* pSender, Widget::TouchEventType type);
    
    void roomListMoveIn(bool bIn);
    bool roomExperienceTimeSelector(HNSocketMessage* socketMessage);
public:
	void cancelTouchRoom(float delata);
    bool m_bIsTouch;

	vector<ComRoomInfo*>				m_vRoomInfo;
	vector<Button*>						m_vRoomItem;

	HNRoomLogicBase*	_roomLogic;
	HNRoomDeskLogic*	_deskLogic;
private:
	
    GameRoom* gameRoomLayer;
	Sprite * iconSpr;
    Button* m_experienceItem;
private:
    std::string converNumberWithWAN(LLONG number);
    vector<JackpotModel*> m_jackpotModelData;
    int m_nRoomCount;

	bool _canClose;
	void canTouchTime(float dt);
    
};

#endif //_ROOM_LIST_COMPOSITE_V99_H_
