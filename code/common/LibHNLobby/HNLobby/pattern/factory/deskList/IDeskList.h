#ifndef _IDESKLIST_H_
#define _IDESKLIST_H_

#include "cocos2d.h"
#include <string>
#include "HNLobby/component/delegate/DeskListDelegate.h"
#include "HNPlatformLogic/HNPlatformRoomList.h"
#include "HNPlatformLogic/HNPlatformGameList.h"
#include "HNLobby/PlatformMessage.h"
#include "HNLobby/component/delegate/MenuDelegate.h"

/*

*/
class IDeskList :public cocos2d::Node, public DeskListDelegate {
    CC_SYNTHESIZE(MenuDelegate*, topMenudelegate, TopMenuDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
    CC_SYNTHESIZE(HNPlatformRoomList*, _roomListLogic, RoomListLogic);
    CC_SYNTHESIZE(HNPlatformGameList*, _gameListLogic, GameListLogic);
    CC_SYNTHESIZE(BacksTageOpen, _backsTageOpen, BacksTageOpen);

public:
    virtual void execute() = 0;
};

#endif //_ITOPMENU_H_
