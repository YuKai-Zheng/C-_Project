#ifndef _IROOMLIST_H_
#define _IROOMLIST_H_

#include "cocos2d.h"
#include "HNLobby/component/delegate/RoomListDelegate.h"
#include "HNLobby/component/delegate/PlatformDelegate.h"
#include "HNPlatformLogic/HNPlatformRoomList.h"
#include "HNPlatformLogic/HNPlatformGameList.h"
#include "HNLobby/component/delegate/MenuDelegate.h"

/*

*/
class IRoomList :public cocos2d::Node, public RoomListDelegate {
    CC_SYNTHESIZE(MenuDelegate*, topMenudelegate, TopMenuDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);
    CC_SYNTHESIZE(HNPlatformRoomList*, _roomListLogic, RoomListLogic);
    CC_SYNTHESIZE(HNPlatformGameList*, _gameListLogic, GameListLogic);

public:
    virtual void execute() = 0;
};

#endif //_IROOMLIST_H_
