#include "GameMenuCompositeV1.h"
#include "VersionInfo/VersionInfo.h"
#include "HNOperator.h"
#include "../FontConfig.h"
#include "../pattern/CommandManager.h"
#include "../PlatformDefine.h"
#include "../pattern/factory/gameLand/GameLandFactory.h"
#include "../pattern/factory/gameRegister/GameRegisterFactory.h"
#include "../pattern/factory/findPw/FindPwFactory.h"
#include "../pattern/factory/gameLogOn/GameLogOnFactory.h"
#include "../res/MenuRes.h"
#include "../GameExitLayer.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "HNLobby/globel.h"

USING_NS_CC;
using namespace spine;

bool GameMenuCompositeV1::init() {
    if (!Node::init()) {
        return false;
    }

    memset(&_login_ui, 0x0, sizeof(_login_ui));
    isVisitor = false;

    //
	if (!GD_NEED_OPEN)
	{
		auto effectBg = Sprite::create("platform/effectBg.png");
		effectBg->setPosition(Vec2(effectBg->getContentSize().width * 0.5f, WIN_SIZE.height - effectBg->getContentSize().height * 0.5f));
		this->addChild(effectBg);

		auto skeletonNode = SkeletonAnimation::createWithJsonFile(GIRL_SPRIBE_JSON, GIRL_SPRIBE_ATLAS);
		skeletonNode->setPosition(Vec2(WIN_SIZE.width * 0.25f, 0));

		skeletonNode->setAnimation(0, "daxiong", true);
		addChild(skeletonNode);

		ParticleSystemQuad *particle = ParticleSystemQuad::create(GIRL_SPRIBE_PLIST);
		particle->setPosition(Vec2(skeletonNode->getPositionX() + (skeletonNode->getContentSize().width - particle->getContentSize().width) / 2, skeletonNode->getPositionY()));
		addChild(particle);

		_login_ui.node = CSLoader::createNode(LOGIN_CSB);
		auto bgSize = static_cast<ImageView *>(_login_ui.node->getChildByName("Image_5"))->getContentSize();
		_login_ui.node->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.45f));
		addChild(_login_ui.node);

		auto antAction = CSLoader::createTimeline(LOGIN_CSB);
		_login_ui.node->runAction(antAction);
		antAction->play("vip", true);
		//
	}

	//是否审核版本
	auto posX = GD_NEED_OPEN ? WIN_SIZE.width*0.5f : WIN_SIZE.width*0.75f;

    //登录按钮
    IGameLogOn* logonLayer = GameLogOnFactory::createWithVersion();
    _login_ui.logonLayout = logonLayer;
    this->addChild(logonLayer);
    logonLayer->setPosition(WIN_SIZE / 2);
    
    //创建
    IGameLand *landLayer = GameLandFactory::createWithVersion();
    _login_ui.loginLayout = landLayer;
    this->addChild(landLayer);
    landLayer->setAnchorPoint(Vec2(0, 0));
	landLayer->setPosition(Vec2(posX, WIN_SIZE.height*0.5f));

    //注册按钮接口
    IGameRegister *registerLayer = GameRegisterFactory::createWithVersion();
    _login_ui.registerLayout = registerLayer;
    this->addChild(registerLayer);
    registerLayer->setAnchorPoint(Vec2(0, 0));
	registerLayer->setPosition(Vec2(posX, WIN_SIZE.height*0.5f));
    registerLayer->setVisible(false);
    //找回密碼按鈕
    IFindPw* findPwLayer = FindPwFactory::createWithVersion();
    _login_ui.findLayout = findPwLayer;
    this->addChild(findPwLayer);
    findPwLayer->setAnchorPoint(Vec2(0, 0));
	findPwLayer->setPosition(Vec2(posX, WIN_SIZE.height*0.5f));
    findPwLayer->setVisible(false);

    /**********************登录界面********************/

    landLayer->onLoginCallBack = [this](const std::string &name, const std::string &psd) {
        CommandManager::getInstance()->addLoaderCommand1(GetText("LOGING"), gFontConfig_30);

        LoginInfo loginInfo;
        loginInfo.name = name;
        loginInfo.password = psd;

        VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_ACCOUNT_TYPE);
        cmd->setLoginInfo(loginInfo);
        cmd->setLoginDelegate(m_loginDelegate);
        cmd->execute();
    };

    landLayer->youKeLoginCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
        isVisitor = true;

        Button* selectedBtn = (Button*)pSender;
        switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            selectedBtn->setColor(Color3B(170, 170, 170));
            break;

        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            selectedBtn->setColor(Color3B(255, 255, 255));
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("游客登陆中......"));

            this->runAction(Sequence::create(DelayTime::create(15.0f),
            CallFunc::create([this]() {
                CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
            }),
            nullptr));

            VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_DEVICE_TYPE);
            cmd->setLoginDelegate(m_loginDelegate);
            cmd->execute();
        }
        break;

        case cocos2d::ui::Widget::TouchEventType::CANCELED:
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            selectedBtn->setColor(Color3B(255, 255, 255));
            break;
        }
    };

    logonLayer->wechatLogOnCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
        switch (type) {
            case cocos2d::ui::Widget::TouchEventType::ENDED: {
                HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
                UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
                UserDefault::getInstance()->setBoolForKey(WX_LOGIN, true);
                UserDefault::getInstance()->flush();
                CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("微信登陆中......"));
                this->runAction(Sequence::create(DelayTime::create(15.0f),
                                                 CallFunc::create([this]() {
                    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
                }), nullptr));
                VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_WECHAT_TYPE);
                cmd->setLoginDelegate(m_loginDelegate);
            }
                
                break;
                
            default:
                
                break;
        }
        
    };
    
    landLayer->onRegistCallBack = [=]() {
        landLayer->setVisible(false);
        registerLayer->setVisible(true);
        registerLayer->initRegisterUI();
    };
    landLayer->onFindPwCallBack = [=]() {
        landLayer->setVisible(false);
        findPwLayer->setVisible(true);
        findPwLayer->initFindPwUI();
    };

    /*********************************************************/

    /*******************账号注册*****************************/
    registerLayer->onCloseCallBack = [=]() {
        registerLayer->setVisible(false);
        landLayer->setVisible(true);
    };

    registerLayer->onRegisterCallBack =
    [this](const std::string &name, const std::string &psd, BYTE gender, const std::string nickName, const std::string inviteCode) {
        CommandManager::getInstance()->addLoaderCommand1(GetText("REGISTRATION"), gFontConfig_30);

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
        findPwLayer->setVisible(false);
        landLayer->setVisible(true);
    };

    if (RELEASE_MODE) {

    } else {
        // 游戏版本
        char buffer[64] = { 0 };
        std::string version = Operator::requestChannel("sysmodule", "getversion");
        sprintf(buffer, GBKToUtf8("Version: %s"), version.c_str());
        auto gameVersion = createLabel(buffer, 18);
        gameVersion->setAnchorPoint(Vec2(0.0f, 0.0f));
        gameVersion->setPosition(Vec2(10.f, 10.f));
        addChild(gameVersion, 2);
    }

    return true;
}

void GameMenuCompositeV1::handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) {
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
