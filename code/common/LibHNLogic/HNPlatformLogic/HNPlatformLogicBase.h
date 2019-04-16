#ifndef __HNPlatformLogicBase_H__
#define __HNPlatformLogicBase_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetExport.h"
#include "HNLobby/component/delegate/BaseDelegate.h"

namespace HN {
/*
 * common interface for platform logic.
 */
class IHNPlatformLogicBase {
public:
    virtual void onPlatformDisConnectCallback(const std::string& message) {}
    //virtual void onPlatformNewsCallback(const std::string& message) {};
    //virtual void onPlatformUpdateUserInfoCallBack(MSG_GP_Get_User_WalletMoney_BankMoney* pData){};
};

/*
 * platform logic base.
 */
class HNPlatformLogicBase: public HNLogicBase, public IPlatformMessageDelegate {
    //CC_SYNTHESIZE(BaseDelegate*, m_delegate, Delegate);

public:
    HNPlatformLogicBase(BaseDelegate* delegate);
    virtual ~HNPlatformLogicBase();
    virtual void start() override;
    virtual void stop() override;
public:
    // network break.
    virtual void onHandleDisConnect() override;
    virtual void onNewsMessage(MSG_GR_RS_NormalTalk* pData) override;
    //游戏公告
    virtual void onHandlePlatformNotice(const std::string message) override;
protected:
    //IHNPlatformLogicBase* _callback;
    BaseDelegate*  _delegate;
};
}


#endif
