#include "HNPlatformEventLayer.h"

#include "HNPlatformLogic.h"

namespace HN {

HNPlatformEventLayer::HNPlatformEventLayer(void) {
}

HNPlatformEventLayer::~HNPlatformEventLayer(void) {
}

void HNPlatformEventLayer::onEnter() {
    HNLayer::onEnter();
    PlatformLogic()->addObserver(this);
}

void HNPlatformEventLayer::onExit() {
    HNLayer::onExit();
    PlatformLogic()->removeObserver(this);
}


void HNPlatformEventLayer::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize) {
    PlatformLogic()->sendData(MainID, AssistantID, object, objectSize);
}

void HNPlatformEventLayer::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector) {
    PlatformLogic()->sendData(MainID, AssistantID, object, objectSize, selector);
}

void HNPlatformEventLayer::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector) {
    PlatformLogic()->addEventSelector(MainID, AssistantID, selector);
}

void HNPlatformEventLayer::removeEventSelector(UINT MainID, UINT AssistantID) {
    PlatformLogic()->removeEventSelector(MainID, AssistantID);
}

void HNPlatformEventLayer::platformLogin(const std::string& name, const std::string& password, UINT uNameID) {
    MSG_GP_S_LogonByNameStruct data;
    data.uRoomVer = 4;
    data.iUserID = -1;
    data.gsqPs = 5471;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
    data.uNameID  = uNameID;
#endif
    strcpy(data.TML_SN, "EQ4gG6vEUL06ajaGn4EAuXDa662vaeeqL6UdoOQatxuujAlnqovO6VndvXT4Tv0l4a28XGoDxqde4El6XUAXLXe66lg2o6gQN4tlOgeAoV6gulE2jTNneUulE");
    strcpy(data.szName, name.c_str());
    strcpy(data.szMD5Pass, password.c_str());
    strcpy(data.szMathineCode, "000266-703028-103FA5-C05024-006BBE-007F50");
    strcpy(data.szCPUID, "612826255");
    strcpy(data.szHardID, "2222222");
    strcpy(data.szIDcardNo, "123456789");
    strcpy(data.szMathineCode, "123456789");
    strcpy(data.szIDcardNo, "*");
    strcpy(data.szMobileVCode, "*");
    strcpy(data.szUDID, UserDefault::getInstance()->getStringForKey("UUID").c_str());
    sendData(MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME, &data, sizeof(data));
}

void HNPlatformEventLayer::platformRegister(BYTE byFastRegister, UINT uNameID, const std::string& usn, const std::string& name, const std::string& password, const std::string &inviteCode) {
    MSG_GP_S_Register data = {0};
    data.byFromPhone = 1;
    data.byFastRegister = byFastRegister;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
    data.uNameID = uNameID;
#endif
    strcpy(data.szName, name.c_str());
    strcpy(data.szPswd, password.c_str());
    strcpy(data.szHardID, usn.c_str());
    strcpy(data.szUDID, UserDefault::getInstance()->getStringForKey("UUID").c_str());
    PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_REGISTER, &data, sizeof(data));
}

void HNPlatformEventLayer::requestRoomListInfo(UINT uKindID, UINT uNameID) {
    MSG_GP_SR_GetRoomStruct GetRoomStruct;
    GetRoomStruct.uKindID = uKindID;
    GetRoomStruct.uNameID = uNameID;
    sendData(MDM_GP_LIST, ASS_GP_LIST_ROOM,&GetRoomStruct, sizeof(GetRoomStruct));
}

}