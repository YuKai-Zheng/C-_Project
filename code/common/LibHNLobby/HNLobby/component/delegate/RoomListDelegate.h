#ifndef _ROOM_LIST_DELEGATE_H_
#define _ROOM_LIST_DELEGATE_H_

#include "BaseDelegate.h"
#include "HNBaseType.h"
/*

*/
class RoomListDelegate : public BaseDelegate {
public:
    virtual void closeRoomList() {};
    virtual void openRoomList() {};
    virtual void updateRoomList(bool success, const std::string& message) {};
    virtual void updateRoomUserCount(UINT roomID, UINT userCount) {};
};

#endif //_ROOM_LIST_DELEGATE_H_
