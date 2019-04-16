#ifndef __KooSDK__PayManager__
#define __KooSDK__PayManager__

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "paycb.h"
#import "PayViewController.h"
#endif

#import "HNModule.h"
#import "HNPayCallBack.h"

#include <string>


////大陆个人开发者账号 Game8090 支付使用密钥
//static const std::string G8090AppId_IAP      = "1140343958";
//static const std::string G8090ShardKey_IAP   = "1c12deb7749e44869ce1caefd91ff9db";
////澳门个人开发者账号 8090娱乐中心 支付使用密钥
//static const std::string AC8090AppId_IAP     = "1159545042";
//static const std::string AC8090ShardKey_IAP  = "1c12deb7749e44869ce1caefd91ff9db";

//static std::string VERIFY_LINK      = "https://sandbox.itunes.apple.com/verifyReceipt";
//static const std::string PRODUCTION_VERIFY   = "https://buy.itunes.apple.com/verifyReceipt";


namespace HN
{
    class PayModule;
    
    class PayManager : public Module, public MultiPayDelegate
                        , public PayDelegate
    {
		create_declare(PayManager)
        
    public:
        void start(UIViewController *viewController, bool MultiPay = false);
        void registerPay(PayModule* payModule);//,
//                         const std::string& appId = getAppId(),//G8090AppId_IAP,
//                         const std::string& appKey = getAppShareKey());//G8090ShardKey_IAP);
        void setPayPriority(PayModule* first,
                            const std::string& firstName,
                            PayModule* second,
                            const std::string& secondName);
        
        
    public:
        virtual void handleMultiPayStart(PayModule* pay) override;
        
        virtual void handleMultiPayCancel() override;
        
    public:
        virtual void handlePayResult(int status, const std::string& ext) override;
        virtual void handleLoginResult(int status) override;
        virtual void handleLogoutResult() override;
        
    private:
        std::string pay(const std::string& args, CALLBACK_PRAGMA* callback);
        std::string restore(const std::string& args, CALLBACK_PRAGMA* callback);
        std::string isLogin(const std::string& args);
        std::string login(const std::string& args, CALLBACK_PRAGMA* callback);
        std::string logout(const std::string& args, CALLBACK_PRAGMA* callback);
        
    private:
        PayManager();
        
    public:
        ~PayManager();
        
    private:
        Controller<PayModule*> _payMap;
        
        PayViewController*     _payViewController;
        
        UIViewController*      _viewController;
        
        bool                   _multiPay;
    };
}

#endif      // __KooSDK__PayManager__
