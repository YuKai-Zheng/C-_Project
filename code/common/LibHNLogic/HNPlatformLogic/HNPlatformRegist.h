#ifndef __HNPlatformRegist_H__
#define __HNPlatformRegist_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"
#include "HNLobby/component/delegate/RegisterDelegate.h"

namespace HN {
/*
 * redbird regist interface
 */
class IHNPlatformRegist: public IHNPlatformLogicBase {
public:
    //注册结果回调
    virtual void onPlatformRegistCallback(bool success, bool fastRegist, const std::string& message, const std::string&name, const std::string& pwd,int loginTimes) {}
};

/*
 * redbird regist logic
 */
class HNPlatformRegist : public HNPlatformLogicBase { /*, public IHNPlatformRegist*/
    //CC_SYNTHESIZE(RegisterDelegate*, m_delegate, Delegate);

public:
    HNPlatformRegist(RegisterDelegate* delegate);
    virtual ~HNPlatformRegist();

    //游客注册
    void visitorRegister();
    //微信注册
    void wechatUnionRegister();
    //账号注册
    void accountRegister(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, const std::string inviteCode, bool fastRegist);
    //void accountRegister(std::string _userName, std::string param2, BYTE _gender, const std::string nickName, std::string _inviteCode, bool param6);
public:
    bool requestRegist(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, const std::string inviteCode, bool fastRegist);
    bool requestTouristRegist(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, bool fastRegist);
    bool requestWechatRegist(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, bool fastRegist);
public:
    // platform socket connet.
    virtual void onHandleConnectMessage(bool result) override;
    // platform regist.
    virtual void onHandleUserRegisterMessage(MSG_GP_S_Register* registerStruct, UINT ErrorCode) override;

public:
    void onPlatformRegistCallback(bool success, bool fastRegist, const std::string& message, const std::string&name, const std::string& pwd, int loginTimes) ;


protected:
    bool _fastRegist;
    bool _wechatRegist;
    std::string _name;
    std::string _pwd;
    BYTE			   _gender;
    std::string _nickName;
    std::string _inviteCode;
    //IHNPlatformRegist* _callback;
    void platformRegist();
    void wechatRegist();
private:
    RegisterDelegate* m_delegate;
};
}


#endif // !__HNPlatformRegist_H__
