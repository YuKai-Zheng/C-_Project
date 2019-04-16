#ifndef _ROOM_LIST_COMPOSITE_H_
#define _ROOM_LIST_COMPOSITE_H_

#include "HNLobby/GameRoom.h"
#include "../pattern/factory/roomList/IRoomList.h"

/*

*/
class RoomListComposite :public IRoomList {
public:
    RoomListComposite();
    virtual bool init()override;
    CREATE_FUNC(RoomListComposite);

public:
    virtual void updateRoomUserCount(UINT roomID, UINT userCount)override;
    virtual void closeRoomList()override;
    virtual void execute()override;

public:
    bool isShowBank;

private:
    GameRoom* gameRoomLayer;

};

#endif //_ROOM_LIST_COMPOSITE_H_
