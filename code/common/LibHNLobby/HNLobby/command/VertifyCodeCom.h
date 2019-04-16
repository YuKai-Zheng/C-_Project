#ifndef _VERTIFY_CODE_COMMAND_H_
#define _VERTIFY_CODE_COMMAND_H_
#include "../pattern/Command.h"

#include <string>
#include "cocos2d.h"

#include "network/HttpClient.h"
#include "HNLobby/component/delegate/LoginDelegate.h"
#include "HNLobby/component/delegate/RegisterDelegate.h"
using namespace cocos2d::network;

USING_NS_CC;

enum OperType{
    //账号登录
    LOGIN_ACCOUNT_TYPE = 0,
    //注册
    REGISTER_TYPE = 1,
    //本地登录
    LOGIN_DEVICE_TYPE = 2,
    //微信登录
    LOGIN_WECHAT_TYPE = 3
};

/*
*/
class VertifyCodeCom : public Command {
public:
    virtual void execute()override;

    void setType(int type);
    void setPassed(bool isPassed);
    
    void setLoginInfo(LoginInfo loginInfo);
//    void setLocalLoginInfo(std::string IMEI);
    void setRegisterInfo(RegisterInfo registerInfo);
    
    void setLoginDelegate(LoginDelegate* delegate);
    void setRegisterDelegate(RegisterDelegate* delegate);
    
    
    
private:
    int                 m_type;
    bool                m_passed;
    
//    std::string         m_IMEI;
    LoginInfo           m_loginInfo;
    RegisterInfo        m_registerInfo;
    
    LoginDelegate*      m_loginDelegate;
    RegisterDelegate*   m_registerDelegate;
    
    void direct();
    void requestURL(std::string url,std::string params);
    void onHttpOpenFuncResponse(network::HttpClient *client, network::HttpResponse *response) ;
};

#endif //_VERTIFY_CODE_COMMAND_H_
