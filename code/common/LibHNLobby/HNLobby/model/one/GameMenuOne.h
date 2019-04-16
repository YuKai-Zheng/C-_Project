#ifndef __GameMenuOne_H__
#define __GameMenuOne_H__

#include "cocos2d.h"
#include <string>
#include "HNPlatformLogic/HNPlatformLogin.h"
#include "HNPlatformLogic/HNPlatformRegist.h"
#include "../../component/delegate/LoginDelegate.h"
#include "../../component/delegate/RegisterDelegate.h"
#include "../../pattern/factory/gameMenu/IGameMenu.h"

USING_NS_CC;

class GameMenuOne : public HNLayer, public LoginDelegate, public RegisterDelegate {
public:
    GameMenuOne();
    virtual ~GameMenuOne();
    CREATE_FUNC(GameMenuOne);
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    //手机返回键监听回调函数
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;

public:
    virtual void loginByAccount(LoginInfo loginInfo) override;
    virtual void loginByDevice() override;
    virtual void loginByWechat() override;
    virtual void loginResult(bool isSuccess, const std::string &message, int errCode)override;

    virtual void registerByAccount(RegisterInfo registerInfo) override;
    virtual void registerResult(bool isSuccess, bool fastRegist, const std::string& message, const std::string&name, const std::string& pwd, int loginTimes)override;

    virtual void platformDisConnect(const std::string& message)override;
    void setBackGroundImage(const std::string &name);
protected:
    //登录逻辑类
    HNPlatformLogin*	_gameLogin;
    //注册逻辑类
    HNPlatformRegist*	_gameRegist;

    //只有在Android和IOS平台下执行
    void checkUpdate();
    void updateCheckFinishCallback(bool updated, cocos2d::Node* pSender, const std::string& message, const std::string& storePath);

    IGameMenu* gameMenu;
};

#endif // __GameMenuOne_H__
