#ifndef __HNPlatformLogin_H__
#define __HNPlatformLogin_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"
#include <string>
#include "HNLobby/component/delegate/LoginDelegate.h"

namespace HN {
/*
 *	redbird login interface.
 */
class IHNPlatformLogin: public IHNPlatformLogicBase {
public:
    //登陆结果回调
    virtual void onPlatformLoginCallback(bool success, const std::string& message,const std::string& name, const std::string& pwd, int errCode) {}
};


/*
 * redbird login logic.
 */
class HNPlatformLogin : public HNPlatformLogicBase { /*, public HN::IHNPlatformLogin*/
    //CC_SYNTHESIZE(LoginDelegate*, m_delegate, Delegate);

public:
    HNPlatformLogin(LoginDelegate* delegate);
    virtual ~HNPlatformLogin();
public:
    bool requestLogin(const std::string& name, const std::string& pwd);
public:
    // platform socket connet.
    virtual void onHandleConnectMessage(bool result) override;
    // platform login.
    virtual void onHandleLoginMessage(bool result, UINT dwErrorCode, const std::string& msg) override;

public:
    virtual void onPlatformLoginCallback(bool success, const std::string& message, const std::string& name, const std::string& pwd, int errCode) ;

    //保存用户账号
    void saveUserInfo(const std::string &userName, const std::string &userPswd);
    void enterGame(const std::string &userName, const std::string &userPswd);

    //是否是账号登录
    bool				_isAccountLogin;

protected:
    std::string _name;
    std::string _pwd;
    IHNPlatformLogin* m_callback;
    void platformLogin();

private:
    LoginDelegate* m_delegate;

};
}

#endif
