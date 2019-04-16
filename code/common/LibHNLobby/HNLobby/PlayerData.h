#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include <string>
#include "HNBaseType.h"

struct LoginInfo {
    std::string name;
    std::string password;
};

struct RegisterInfo {
    BYTE gender;
    bool fastRegist;

    std::string name;
    std::string password;
    std::string nickName;
    std::string inviteCode;
};

#endif	//__PLAYER_DATA_H__

