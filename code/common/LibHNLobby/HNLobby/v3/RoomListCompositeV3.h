#ifndef _ROOM_LIST_COMPOSITE_V3_H_
#define _ROOM_LIST_COMPOSITE_V3_H_

#include "HNLobby/GameRoom.h"
#include "../pattern/factory/roomList/IRoomList.h"

/*

*/
class RoomListCompositeV3 :public IRoomList {
public:
    RoomListCompositeV3();
    virtual bool init()override;
    CREATE_FUNC(RoomListCompositeV3);

public:
    virtual void updateRoomUserCount(UINT roomID, UINT userCount)override;
    virtual void closeRoomList()override;
    virtual void execute()override;

public:
    bool isShowBank;

private:
    GameRoom* gameRoomLayer;

};

#endif //_ROOM_LIST_COMPOSITE_V3_H_
