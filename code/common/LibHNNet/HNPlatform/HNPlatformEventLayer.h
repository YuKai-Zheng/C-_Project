#ifndef __HN_HNPlatformEventLayer_H__
#define __HN_HNPlatformEventLayer_H__

#include "HNPlatform/HNPlatformNotifyDelegate.h"
#include "HNSocket/HNSocketMessage.h"
#include "UI/Base/HNLayer.h"
#include "HNBaseType.h"
#include <string>

namespace HN {

class HNPlatformEventLayer : public HNLayer, IPlatformMessageDelegate {
public:
    HNPlatformEventLayer(void);
    virtual ~HNPlatformEventLayer(void);

public:
    virtual void onEnter() override;

    virtual void onExit() override;

public:
    // send data
    void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

    // send data
    void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);

    // set handler callback event
    void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

    // remove handler callback event
    void removeEventSelector(UINT MainID, UINT AssistantID);

protected:
    // get the room list function
    void requestRoomListInfo(UINT uKindID, UINT uNameID);

    // platform login function
    void platformLogin(const std::string& name, const std::string& password, UINT uNameID);

    // platform register
    void platformRegister(BYTE byFastRegister, UINT uNameID, const std::string& usn, const std::string& name = "", const std::string& password = "", const std::string &inviteCode = "");

};
};

#endif	//__HN_HNPlatformEventLayer_H__
