#include "GameMenuOne.h"
#include "HNLobby/HNUpdate.h"
#include "VersionInfo/VersionInfo.h"
#include "../../PlatformDefine.h"
#include "../../pattern/factory/gameMenu/GameMenuFactory.h"
#include "../../pattern/factory/background/BackgroundFactory.h"
#include "../../pattern/CommandManager.h"
#include "../../command/ShowGamePrompt.h"
#include "HNLobby/GamePrompt.h"
#include "../../res/MenuRes.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/GameHTTPRequestManager.h"
#include "HNLobby/GamePrompt.h"

GameMenuOne::GameMenuOne() {
    //memset(&_login_ui, 0x0, sizeof(_login_ui));
    _gameRegist = new HNPlatformRegist(this);
    _gameLogin = new HNPlatformLogin(this);
}

GameMenuOne::~GameMenuOne() {
    HN_SAFE_DELETE(_gameRegist);
    HN_SAFE_DELETE(_gameLogin);
}

bool GameMenuOne::init() {
    if (!HNLayer::init()) {
        return false;
    }

    enableKeyboard();

	if (VERSION_SIXTY_SIX) {
        setBackGroundImage(LOGON_BG_V66);
//        setBackGroundImage("NewYear/loginBg.png");
	}

    if (!Tools::isiOSReview()) {
//        auto kefu = Sprite::create("v66/denglujiemian/25.png");
//        if (kefu) {
//            kefu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//            kefu->setPosition(Vec2(10, 8));
//            addChild(kefu);
//        }
    }
    
    gameMenu = GameMenuFactory::createWithVersion();
    gameMenu->setLoginDelegate(this);
    gameMenu->setRegisterDelegate(this);
    addChild(gameMenu);

    //UserDefault::getInstance()->getXMLFilePath();
    // 设置音乐和音效的默认音量
    if (UserDefault::getInstance()->getIntegerForKey(IS_FIRST_REGIST, 0) == 0) {
        UserDefault::getInstance()->setIntegerForKey(IS_FIRST_REGIST, 1);
        UserDefault::getInstance()->setBoolForKey(MUSIC_SWITCH, true);
        UserDefault::getInstance()->setBoolForKey(EFFECT_SWITCH, true);
        UserDefault::getInstance()->flush();
    }
    HNAudioEngine::getInstance()->setSwitchOfMusic(UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH));
    HNAudioEngine::getInstance()->setSwitchOfEffect(UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH));

    UserDefault::getInstance()->setBoolForKey("isLoginTipBindPhone",false); //设置登录进入大厅提示绑定手机
    
    return true;
}
void GameMenuOne::setBackGroundImage(const std::string &name) {
    auto loadingBG = BackgroundFactory::create(name.c_str());
    loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2)); // visibleOrigin +
    //float _xScale = VISIBLE_SIZE.width / loadingBG->getContentSize().width;
    //float _yScale = VISIBLE_SIZE.height / loadingBG->getContentSize().height;
    //loadingBG->setScaleX(_xScale);
    //loadingBG->setScaleY(_yScale);
    this->addChild(loadingBG, -1);
}
void GameMenuOne::onEnterTransitionDidFinish() {
    HNLayer::onEnterTransitionDidFinish();

    if (UPDATE_ONOFF) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        checkUpdate();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID/* || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32*/)
        checkUpdate();
#endif
    }
}

void GameMenuOne::checkUpdate() {
    if (!getOnly186Try()) {
       //HNUpdate *update = HNUpdate::create();modify by zengwc
       //if (update != nullptr) {
       //    this->addChild(update, 100);
       //    update->onFinishCallback = std::bind(&GameMenuOne::updateCheckFinishCallback, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
       //    update->checkUpdate();
       //}
    }
}

void GameMenuOne::updateCheckFinishCallback(bool updated, cocos2d::Node *pSender,const std::string &message,const std::string &storePath) {
    if (updated) {
// string url1 = (dynamic_cast<HNUpdate*>(pSender))->getDownloadUrl();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        HN::Operator::requestChannel("sysmodule", "installApp", storePath);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //以下url是链接到appstore上的
        // std::string url =
        // "itms-services://?action=download-manifest&amp;url=https://";
        // url.append(getWebServerUrl());
        // url.append("/download/app/");
        // url.append(PlatformConfig::getInstance()->getDownloadName());
        // url.append(".plist");

        string url1 = (dynamic_cast<HNUpdate *>(pSender))->getDownloadUrl();
        Application::getInstance()->openURL(url1);
#else
        ;
#endif
        if (pSender != nullptr) {
            pSender->removeFromParent();
        }
    } else {
        std::string str(GBKToUtf8("更新失败！\n"));
        str.append(GBKToUtf8(message.c_str()));
        HNLOG_INFO("%s", GBKToUtf8(message.c_str()));
        CommandManager::getInstance()->showGamePrompt(str);
        if (pSender != nullptr) {
            pSender->removeFromParent();
        }
    }
}

void GameMenuOne::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *pEvent) {
//     if (gameMenu) {
//         gameMenu->handleKeyReleased(keyCode, pEvent);
//     }
}

void GameMenuOne::loginByAccount(LoginInfo loginInfo) {
    _gameLogin->_isAccountLogin = true;
    _gameLogin->enterGame(loginInfo.name, loginInfo.password);
}

void GameMenuOne::loginByDevice() {
    _gameLogin->_isAccountLogin = false;
    _gameRegist->visitorRegister();
}

void GameMenuOne::loginByWechat() {
    _gameLogin->_isAccountLogin = false;
    _gameRegist->wechatUnionRegister();
}

void GameMenuOne::loginResult(bool isSucess, const std::string &message, int errCode) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);

//    if (!isSucess) {
//        auto str = (message == "" ? ForceGBKToUtf8("服务器断开连接") : message);
//        CommandManager::getInstance()->showGamePrompt(str);
//    }
    if (isSucess)
    {
        GameHTTPRequestManager::getInstance()->ReqClienIp(PlatformLogic()->loginResult.dwUserID, "login");
    }
    else
    {
        if (!message.empty()) {
            //CommandManager::getInstance()->showGamePrompt(message);
			GamePromptLayer::create()->showPrompt(message);
        }
    }
}

void GameMenuOne::registerByAccount(RegisterInfo registerInfo) {
    _gameRegist->accountRegister(registerInfo.name, registerInfo.password, registerInfo.gender, registerInfo.nickName, registerInfo.inviteCode, registerInfo.fastRegist);
}

void GameMenuOne::registerResult(bool isSuccess, bool fastRegist, const std::string& message, const std::string&name, const std::string& pwd, int loginTimes) {
    if (isSuccess) {
        LoginInfo loginInfo;
        loginInfo.name = name;
        loginInfo.password = pwd;

        if (0 == loginTimes && !fastRegist) {
            std::string title = StringUtils::format(GetText("SHOW_NAME_PSD"),name.c_str());

            ShowGamePrompt* prompt =(ShowGamePrompt*) CommandManager::getInstance()->get(SHOW_GAME_PROMPT);
            prompt->setText(title);
            prompt->setCallBack(CC_CALLBACK_0(GameMenuOne::loginByAccount, this, loginInfo));
            prompt->execute();
        } else {
            _gameLogin->enterGame(loginInfo.name, loginInfo.password);
        }

        GameHTTPRequestManager::getInstance()->ReqClienIpByName(name, "register");
    } else {
        CommandManager::getInstance()->showGamePrompt(message);
    }
}

void GameMenuOne::platformDisConnect(const std::string& message) {
    CommandManager::getInstance()->showGamePrompt(message);
}
