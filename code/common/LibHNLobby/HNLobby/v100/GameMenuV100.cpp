#include "GameMenuV100.h"
#include "../GameExitLayer.h"
#include "../PlatformDefine.h"
#include "../pattern/CommandManager.h"
#include "VersionInfo/VersionInfo.h"
#include "../FontConfig.h"
#include "../res/MenuRes.h"
#include "../CommonMacro.h"
#include "HNLobby/PlayerData.h"

using namespace HN;

bool GameMenuV100::init() {
    if (!GameMenuOne::init()) {
        return false;
    }
    return true;
}

//void GameMenuV100::initMenu() {
//    memset(&_login_ui, 0x0, sizeof(_login_ui));
//    isVisitor = false;
//    //
//
//    setBackGroundImage(LOGIN_BG);
//
//    //创建
//    LandLayer *landLayer = LandLayer::create();
//    _login_ui.loginLayout = landLayer;
//    this->addChild(landLayer);
//    //landLayer->setAnchorPoint(Vec2(0, 0));
//    landLayer->setPosition(WIN_SIZE / 2);
//
//    //注册按钮接口
//    RegisterLayer *registerLayer = RegisterLayer::create();
//    _login_ui.registerLayout = registerLayer;
//    this->addChild(registerLayer);
//    registerLayer->setAnchorPoint(Vec2(0, 0));
//    //registerLayer->setPosition(Vec2(0, 0));
//    registerLayer->setPosition(WIN_SIZE / 2);
//    registerLayer->setVisible(false);
//
//    //找回密碼按鈕
//    FindPwLayer *findPwLayer = FindPwLayer::create();
//    _login_ui.findLayout = findPwLayer;
//    this->addChild(findPwLayer);
//    findPwLayer->setAnchorPoint(Vec2(0, 0));
//    //findPwLayer->setPosition(Vec2(0, 0));
//    findPwLayer->setPosition(WIN_SIZE / 2);
//    findPwLayer->setVisible(false);
//    /**********************登录界面********************/
//
//    landLayer->onLoginCallBack = [this](const std::string &name, const std::string &psd) {
//        CommandManager::getInstance()->addLoaderCommand1(GetText("LOGING"), gFontConfig_30);
//
//        LoginInfo loginInfo;
//        loginInfo.name = name;
//        loginInfo.password = psd;
//        loginByAccount(loginInfo);
//        //账号登陆
////        enterGame(name, psd);
//    };
//
//    landLayer->youKeLoginCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
//        isVisitor = true;
//
//        Button* selectedBtn = (Button*)pSender;
//        switch (type) {
//        case cocos2d::ui::Widget::TouchEventType::BEGAN:
//            selectedBtn->setColor(Color3B(170, 170, 170));
//            break;
//        case cocos2d::ui::Widget::TouchEventType::ENDED:
//            selectedBtn->setColor(Color3B(255, 255, 255));
//            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
//            CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("游客登陆中......"));
//
//            this->runAction(Sequence::create(DelayTime::create(15.0f),
//            CallFunc::create([this]() {
//                CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
//            }),
//            nullptr));
//
//
//            _gameRegist->visitorRegister();
//            break;
//        case cocos2d::ui::Widget::TouchEventType::CANCELED:
//            selectedBtn->setColor(Color3B(255, 255, 255));
//            break;
//        }
//    };
//
//    landLayer->onRegistCallBack = [=]() {
//        landLayer->setVisible(false);
//        registerLayer->setVisible(true);
//        registerLayer->initRegisterUI();
//    };
//    landLayer->onFindPwCallBack = [=]() {
//        landLayer->setVisible(false);
//        findPwLayer->setVisible(true);
//        findPwLayer->initFindPwUI();
//    };
//
//    /*********************************************************/
//
//    /*******************账号注册*****************************/
//    registerLayer->onCloseCallBack = [=]() {
//        registerLayer->setVisible(false);
//        landLayer->setVisible(true);
//    };
//
//    registerLayer->onRegisterCallBack =
//    [this](const std::string &name, const std::string &psd, BYTE gender, const std::string nickName, const std::string inviteCode) {
//        _gameRegist->accountRegister(name, MD5_CTX::MD5String(psd), gender, nickName, inviteCode, false);
//
//        CommandManager::getInstance()->addLoaderCommand1(GetText("REGISTRATION"), gFontConfig_30);
//    };
//
//
//    /*********************************************************/
//
//    /******************找回密码*****************************/
//    findPwLayer->onCloseCallBack = [=]() {
//        findPwLayer->setVisible(false);
//        landLayer->setVisible(true);
//    };
//
//    if (RELEASE_MODE) {
//
//    } else {
//        // 游戏版本
//        char buffer[64] = { 0 };
//        std::string version = Operator::requestChannel("sysmodule", "getversion");
//        sprintf(buffer, GBKToUtf8("Version: %s"), version.c_str());
//        auto gameVersion = createLabel(buffer, 18);
//        gameVersion->setAnchorPoint(Vec2(0.0f, 0.0f));
//        gameVersion->setPosition(Vec2(10.f, 10.f));
//        addChild(gameVersion, 2);
//    }
//
//    // 设置音乐和音效的默认音量
//    auto userDefault = UserDefault::getInstance();
//    float music = 60.0f, sound = 50.0f;
//    userDefault->setIntegerForKey(MUSIC_VALUE_TEXT, music);
//    userDefault->setIntegerForKey(SOUND_VALUE_TEXT, sound);
//
//    HNAudioEngine::getInstance()->setBackgroundMusicVolume((music / 100));
//    HNAudioEngine::getInstance()->setEffectsVolume((sound / 100));
//    UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, true);
//    if (UserDefault::getInstance()->getIntegerForKey(IS_FIRST_REGIST, 0) == 0)
//        UserDefault::getInstance()->setBoolForKey(MUSIC_SWITCH, true);
//    UserDefault::getInstance()->flush();
//}
//
//void GameMenuV100::handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    if (EventKeyboard::KeyCode::KEY_F1 == keyCode)
//#endif
//    {
//        do {
//            if (_login_ui.registerLayout->isVisible() == true) {
//                _login_ui.registerLayout->onCloseCallBack();
//                break;
//            }
//
//            if (_login_ui.findLayout->isVisible() == true) {
//                _login_ui.findLayout->onCloseCallBack();
//                break;
//            }
//
//            if (!this->getChildByName("GameExitLayer")) {
//                auto exit = GameExitLayer::create();
//                exit->setPosition(Size::ZERO);
//                addChild(exit, 100000000);
//            }
//        } while (0);
//    }
//}
