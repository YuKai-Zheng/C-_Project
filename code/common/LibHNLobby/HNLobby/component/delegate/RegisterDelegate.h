#ifndef _REGISTER_DELEGATE_H_
#define _REGISTER_DELEGATE_H_

//#include <string>
#include "HNLobby/PlayerData.h"

/*

*/
class RegisterDelegate :public BaseDelegate {
public:
    virtual void registerByAccount(RegisterInfo registerInfo) = 0;
    virtual void registerResult(bool isSuccess, bool fastRegist, const std::string& message, const std::string&name, const std::string& pwd, int loginTimes) = 0;
};

#endif //_REGISTER_DELEGATE_H_
