#ifndef _IGAMELIST_H_
#define _IGAMELIST_H_

#include "cocos2d.h"
#include <string>
#include "HNLobby/component/delegate/GameListDelegate.h"
#include "HNPlatformLogic/HNPlatformRoomList.h"
#include "HNPlatformLogic/HNPlatformGameList.h"
#include "HNLobby/component/delegate/MenuDelegate.h"

/*

*/
class IGameList :public cocos2d::Node, public GameListDelegate {
    CC_SYNTHESIZE(MenuDelegate*, topMenudelegate, TopMenuDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);

    CC_SYNTHESIZE(HNPlatformRoomList*, _roomListLogic, RoomListLogic);
    CC_SYNTHESIZE(HNPlatformGameList*, _gameListLogic, GameListLogic);

public:

};

#endif //_IGAMELIST_H_
