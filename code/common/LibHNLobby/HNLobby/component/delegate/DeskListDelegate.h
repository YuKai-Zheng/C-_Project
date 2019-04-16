#ifndef _DESK_LIST_DELEGATE_H_
#define _DESK_LIST_DELEGATE_H_
#include "HNLobby/component/delegate/BaseDelegate.h"
#include "PlatformDelegate.h"
/*

*/
class DeskListDelegate : public BaseDelegate {
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);
public:
    virtual void closeDeskList() = 0;
};

#endif //_DESK_LIST_DELEGATE_H_
