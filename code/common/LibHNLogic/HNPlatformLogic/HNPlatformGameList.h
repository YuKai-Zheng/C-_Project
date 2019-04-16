#ifndef __HNPlatformGameList_H__
#define __HNPlatformGameList_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"
#include "HNLobby/component/delegate/PlatformDelegate.h"

namespace HN {
/*
 * platform game list interface.
 */
class IHNPlatformGameList: public IHNPlatformLogicBase {
public:
    virtual void onPlatformGameListCallback(bool success, const std::string& message) {}
    virtual void onPlatformGameUserCountCallback(UINT Id, UINT count) {}
};

/*
 * platform room list logic.
 */
class HNPlatformGameList :public HNPlatformLogicBase { /*, public HN::IHNPlatformGameList */
    CC_SYNTHESIZE(PlatformDelegate*, m_delegate, Delegate);

public:
    HNPlatformGameList(PlatformDelegate* delegate);
    virtual ~HNPlatformGameList();

public:
    void requestGameList();

public:
    virtual void onHandleGameListMessage() override;
    virtual void onHandleGameUserCountMessage(DL_GP_RoomListPeoCountStruct* userCount) override;
    virtual void start() override;
    virtual void stop() override;
public:
    //virtual void onPlatformGameListCallback(bool success, const std::string& message)override;
    //virtual void onPlatformGameUserCountCallback(UINT Id, UINT count) override;


protected:
    //IHNPlatformGameList* _callback;
    void platformRequestGameList();
};
}

#endif