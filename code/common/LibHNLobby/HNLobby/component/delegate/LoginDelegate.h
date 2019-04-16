#ifndef _LOGIN_DELEGATE_H_
#define _LOGIN_DELEGATE_H_

#include <string>
#include "BaseDelegate.h"
#include "HNLobby/PlayerData.h"

/*

*/
class LoginDelegate :public BaseDelegate {
public:
    virtual void loginResult(bool isSuccess, const std::string &message, int errCode = 0) = 0;
    virtual void loginByAccount(LoginInfo loginInfo) = 0;
    virtual void loginByDevice() = 0;
    virtual void loginByWechat() = 0;
};

#endif //_LOGIN_DELEGATE_H_
