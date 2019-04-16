#include "GameMenuCompositeV88.h"
#include "VersionInfo/VersionInfo.h"
#include "HNOperator.h"
#include "../FontConfig.h"
#include "../pattern/CommandManager.h"
#include "../PlatformDefine.h"
#include "../res/MenuRes.h"
#include "../GameExitLayer.h"
#include "../pattern/factory/gameLand/GameLandFactory.h"
#include "../pattern/factory/gameRegister/GameRegisterFactory.h"
#include "../pattern/factory/findPw/FindPwFactory.h"
#include "../pattern/factory/gameLogOn/GameLogOnFactory.h"
#include "HNLobby/globel.h"
#include "HNLobby/GamePrompt.h"

USING_NS_CC;

bool GameMenuCompositeV88::init() {
    if (!Node::init()) {
        return false;
    }
    
//	///登录界面特效
//	auto aniBaseCsb = CSLoader::createNode("v88/animation/entry/base/entry_base.csb");
//	addChild(aniBaseCsb);
//	aniBaseCsb->setPosition(WIN_SIZE / 2);
//
//	auto aniBaseAction = CSLoader::createTimeline("v88/animation/entry/base/entry_base.csb");
//	aniBaseAction->play("entry_base", true);
//	aniBaseCsb->runAction(aniBaseAction);
	////
//	auto aniGirlCsb = CSLoader::createNode("v88/animation/entry/girl/entry_girl.csb");
//	addChild(aniGirlCsb);
//	aniGirlCsb->setPosition(WIN_SIZE / 2);
//
//	auto aniGirlAction = CSLoader::createTimeline("v88/animation/entry/girl/entry_girl.csb");
//	aniGirlAction->play("entry_girl", true);
//	aniGirlCsb->runAction(aniGirlAction);

    memset(&_login_ui, 0x0, sizeof(_login_ui));
    isVisitor = false;
    
    //登录按钮
    IGameLogOn* logonLayer = GameLogOnFactory::createWithVersion();
    _login_ui.logonLayout = logonLayer;
    this->addChild(logonLayer);
    logonLayer->setPosition(WIN_SIZE / 2);
    
    //创建
    IGameLand *landLayer = GameLandFactory::createWithVersion();
    _login_ui.loginLayout = landLayer;
    this->addChild(landLayer);
    landLayer->setPosition(WIN_SIZE / 2);
    landLayer->setVisible(false);
    
    //注册按钮接口
    IGameRegister *registerLayer = GameRegisterFactory::createWithVersion();
    _login_ui.registerLayout = registerLayer;
    this->addChild(registerLayer);
	registerLayer->setPosition(WIN_SIZE / 2);
    registerLayer->setVisible(false);
    
    //找回密碼按鈕
    IFindPw *findPwLayer = FindPwFactory::createWithVersion();
    _login_ui.findLayout = findPwLayer;
    this->addChild(findPwLayer);
	findPwLayer->setPosition(WIN_SIZE / 2);
    findPwLayer->setVisible(false);

    /**********************登录界面********************/
    
    landLayer->onLoginCallBack = [this](const std::string &name, const std::string &psd) {
        CommandManager::getInstance()->addLoaderCommand1(GetText("LOGING"), gFontConfig_30);
//        this->runAction(Sequence::create(DelayTime::create(5.0f),
//                                         CallFunc::create([this]() {
//            LoadingLayer::removeLoading(Director::getInstance()->getRunningScene());
////            auto prompt = GamePromptLayer::create();
////            prompt->showPrompt(GetText("LOG_IN_TIMEOUT"));
//        }),
//                                         nullptr));
        LoginInfo loginInfo;
        loginInfo.name = name;
        loginInfo.password = psd;

		//记录当前是否为游客登陆 key		 IS_LOCAL_LAND wzx 2017 5 4 
        UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
        UserDefault::getInstance()->setBoolForKey(WX_LOGIN, false);
        UserDefault::getInstance()->flush();
        
        VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_ACCOUNT_TYPE);
        cmd->setLoginInfo(loginInfo);
        cmd->setLoginDelegate(m_loginDelegate);
        cmd->execute();
    };
    
    landLayer->youKeLoginCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
//        isVisitor = true;
//        
//        Button* selectedBtn = (Button*)pSender;
//        switch (type) {
//            case cocos2d::ui::Widget::TouchEventType::BEGAN:
//                selectedBtn->setColor(Color3B(170, 170, 170));
//                break;
//                
//            case cocos2d::ui::Widget::TouchEventType::ENDED: {
//                selectedBtn->setColor(Color3B(255, 255, 255));
//                HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
//
//				//记录当前是否为游客登陆 key		 IS_LOCAL_LAND wzx 2017 5 4
//                UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", true);
//                UserDefault::getInstance()->setBoolForKey(WX_LOGIN, false);
//                UserDefault::getInstance()->flush();
//                
//                CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("游客登陆中......"));
//                this->runAction(Sequence::create(DelayTime::create(15.0f),
//                                                 CallFunc::create([this]() {
//                    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
//                }),
//                                                 nullptr));
//                
//                VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_DEVICE_TYPE);
//                cmd->setLoginDelegate(m_loginDelegate);
//                cmd->execute();
//            }
//                break;
//                
//            case cocos2d::ui::Widget::TouchEventType::CANCELED:
//            case cocos2d::ui::Widget::TouchEventType::MOVED:
//                selectedBtn->setColor(Color3B(255, 255, 255));
//                break;
//        }
    };
    
    landLayer->onRegistCallBack = [=]() {
        landLayer->setVisible(false);
        logonLayer->setVisible(false);
        findPwLayer->setVisible(false);
        registerLayer->setVisible(true);
        registerLayer->initRegisterUI();
    };
    
    landLayer->onFindPwCallBack = [=]() {
        landLayer->setVisible(false);
        logonLayer->setVisible(false);
        registerLayer->setVisible(false);
        findPwLayer->setVisible(true);
        findPwLayer->initFindPwUI();
    };
    
    landLayer->onBackCallBack = [=]() {
        landLayer->setVisible(false);
        findPwLayer->setVisible(false);
        registerLayer->setVisible(false);
        logonLayer->setVisible(true);
    };
    
    logonLayer->wechatLogOnCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
                UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
                UserDefault::getInstance()->setBoolForKey(WX_LOGIN, true);
                UserDefault::getInstance()->flush();
                CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("微信登陆中......"));
//                this->runAction(Sequence::create(DelayTime::create(5.0f),
//                                                 CallFunc::create([this]() {
//                    LoadingLayer::removeLoading(Director::getInstance()->getRunningScene());
////                    auto prompt = GamePromptLayer::create();
////                    prompt->showPrompt(GetText("LOG_IN_TIMEOUT"));
//                }),
//                                                 nullptr));
                VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_WECHAT_TYPE);
                cmd->setLoginDelegate(m_loginDelegate);
            }
                break;
            default:
                break;
        }
        
    };
    
    logonLayer->onLogOnCallBack = [=]() {
        logonLayer->setVisible(false);
        findPwLayer->setVisible(false);
        registerLayer->setVisible(false);
        landLayer->setVisible(true);
    };
    
    logonLayer->localLogonCallBack = [=]() {
        isVisitor = true;
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        
        //记录当前是否为游客登陆 key		 IS_LOCAL_LAND wzx 2017 5 4
        UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", true);
        UserDefault::getInstance()->setBoolForKey(WX_LOGIN, false);
        UserDefault::getInstance()->flush();
        
        CommandManager::getInstance()->addLoaderCommand1(GetText("LOCALLOGIN"), gFontConfig_30);
//        this->runAction(Sequence::create(DelayTime::create(5.0f),
//                                         CallFunc::create([this]() {
//            LoadingLayer::removeLoading(Director::getInstance()->getRunningScene());
////            auto prompt = GamePromptLayer::create();
////            prompt->showPrompt(GetText("LOG_IN_TIMEOUT"));
//        }),
//                                         nullptr));

        
        VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_DEVICE_TYPE);
        cmd->setLoginDelegate(m_loginDelegate);
        cmd->execute();
    };


    
    /*********************************************************/
    
    /*******************账号注册*****************************/
    registerLayer->onCloseCallBack = [=]() {
        registerLayer->setVisible(false);
        findPwLayer->setVisible(false);
        logonLayer->setVisible(false);
        landLayer->setVisible(true);
    };
    
    registerLayer->onRegisterCallBack =
    [this](const std::string &name, const std::string &psd, BYTE gender, const std::string nickName, const std::string inviteCode) {
        CommandManager::getInstance()->addLoaderCommand1(GetText("REGISTRATION"), gFontConfig_30);
//        this->runAction(Sequence::create(DelayTime::create(5.0f),
//                                         CallFunc::create([this]() {
//            LoadingLayer::removeLoading(Director::getInstance()->getRunningScene());
////            auto prompt = GamePromptLayer::create();
////            prompt->showPrompt(GetText("LOG_IN_TIMEOUT"));
//        }),
//                                         nullptr));
        
        UserDefault::getInstance()->setBoolForKey(WX_LOGIN, false);
        
        RegisterInfo registerInfo;
        registerInfo.name = name;
        registerInfo.password = MD5_CTX::MD5String(psd);
        registerInfo.gender = gender;
        registerInfo.nickName = nickName;
        registerInfo.inviteCode = inviteCode;
        registerInfo.fastRegist = false;
        
        VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(REGISTER_TYPE);
        cmd->setRegisterInfo(registerInfo);
        cmd->setRegisterDelegate(m_registerDelegate);
        cmd->execute();
    };
    
    /*********************************************************/
    
    /******************找回密码*****************************/
    findPwLayer->onCloseCallBack = [=]() {
        registerLayer->setVisible(false);
        findPwLayer->setVisible(false);
        logonLayer->setVisible(false);
        landLayer->setVisible(true);
    };
    
    return true;
}

void GameMenuCompositeV88::handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        if (EventKeyboard::KeyCode::KEY_F1 == keyCode)
#endif
        {
            auto winSize = Director::getInstance()->getVisibleSize();
            do {
                if (_login_ui.registerLayout->isVisible() == true) {
                    _login_ui.registerLayout->onCloseCallBack();
                    break;
                }
                
                if (_login_ui.findLayout->isVisible() == true) {
                    _login_ui.findLayout->onCloseCallBack();
                    break;
                }
                
                if (!this->getChildByName("GameExitLayer")) {
                    auto exit = GameExitLayer::create();
                    exit->setPosition(Size::ZERO);
                    addChild(exit, 100000000);
                }
            } while (0);
        }
}
