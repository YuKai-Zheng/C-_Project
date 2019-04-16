#ifndef _GAME_LIST_ROOM_DELEGATE_H_
#define _GAME_LIST_ROOM_DELEGATE_H_
#include "HNLobby/component/delegate/BaseDelegate.h"
#include "PlatformDelegate.h"
/*

*/
class GameListRoomDelegate : public BaseDelegate {
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);
public:
    virtual void closeGameListRoom() = 0;
};

#endif //_GAME_LIST_ROOM_DELEGATE_H_
