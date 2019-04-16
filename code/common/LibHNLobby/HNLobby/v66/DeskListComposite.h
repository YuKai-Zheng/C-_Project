#ifndef _Desk_LIST_COMPOSITE_H_
#define _Desk_LIST_COMPOSITE_H_

#include "HNNetProtocol/HNComStruct.h"
#include "HNLobby/GameDesk.h"
#include "../pattern/factory/deskList/IDeskList.h"

/*

*/
class DeskListComposite : public IDeskList {
public:
    DeskListComposite();
    bool init(ComRoomInfo* roomInfo);
    static DeskListComposite* create(ComRoomInfo* roomInfo);

public:
    virtual void closeDeskList();
    virtual void execute();

private:
    GameDesk * gameDeskLayer;
    ComRoomInfo* _roomInfo;
};

#endif //_Desk_LIST_COMPOSITE_H_
