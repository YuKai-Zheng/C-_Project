#include "GameMenuCompositeV66.h"
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
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "PlatformResV66.h"
#include "HNLobby/GameData.h"

#include "base/WeChatAndroid.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/WechatMgr.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/ClipboardHelper.h"
#endif

USING_NS_CC;
using namespace spine;

bool GameMenuCompositeV66::init()
{
	if (!Node::init())
	{
		return false;
	}

	memset(&_login_ui, 0x0, sizeof(_login_ui));
	isVisitor = false;

	UserDefault::getInstance()->setBoolForKey("WELCOME_TO_GAME_8LE", false);
	UserDefault::getInstance()->setBoolForKey("IS_OPEN_BANK", false);

#if 0
	std::string pathStr = "csb/v66/loginUI/girlEffect.csb";
	auto girlNode = CSLoader::createNode(pathStr);
	if (girlNode) {
		girlNode->setPosition(Vec2(WIN_SIZE.width / 2 + 385, WIN_SIZE.height / 2));
		addChild(girlNode);
		auto action = CSLoader::createTimeline(pathStr);
		if (action) {
			girlNode->runAction(action);
			action->gotoFrameAndPlay(0);
		}
	}

	auto tipsImg = ImageView::create("v66/denglujiemian/865.png");
	if (tipsImg) {
		tipsImg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		tipsImg->setPosition(Vec2(WIN_SIZE.width / 2 - 620, WIN_SIZE.height - 20));
		addChild(tipsImg);
	}

	auto customerBtn = Button::create("v66/denglujiemian/yy.png");
	if (customerBtn) {
		customerBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 570, WIN_SIZE.height - 100));
		addChild(customerBtn);
		customerBtn->addTouchEventListener([](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
			if (type != cocos2d::ui::Widget::TouchEventType::ENDED) {
				return;
			}

			int userId = UserDefault::getInstance()->getIntegerForKey("CUSTOMER_USERID_LOCAL");
			string url1 = StringUtils::format("http://live.game5186.net/live.aspx?UserID=%d", userId);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			Application::getInstance()->openURL(url1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			Application::getInstance()->openURL(url1);
#endif

		});
	}
#endif

	//登录主页;
	IGameLogOn* logonLayer = GameLogOnFactory::createWithVersion();
	_login_ui.logonLayout = logonLayer;
	this->addChild(logonLayer);
	logonLayer->setPosition(Vec2(WIN_SIZE.width*0.5, WIN_SIZE.height*0.5));

	//用户账号密码登陆界面;
	IGameLand *landLayer = GameLandFactory::createWithVersion();
	_login_ui.loginLayout = landLayer;
	this->addChild(landLayer);
	landLayer->setPosition(Vec2(WIN_SIZE.width*0.5, WIN_SIZE.height*0.55f));
	landLayer->setVisible(false);

	//注册按钮接口;
	IGameRegister *registerLayer = GameRegisterFactory::createWithVersion();
	_login_ui.registerLayout = registerLayer;
	this->addChild(registerLayer);
	registerLayer->setPosition(Vec2(WIN_SIZE.width*0.5, WIN_SIZE.height*0.55f));
	registerLayer->setVisible(false);

	//找回密碼按鈕;
	IFindPw *findPwLayer = FindPwFactory::createWithVersion();
	_login_ui.findLayout = findPwLayer;
	this->addChild(findPwLayer);
	findPwLayer->setPosition(Vec2(WIN_SIZE.width*0.65, WIN_SIZE.height*0.55f));
	findPwLayer->setVisible(false);

	//版本信息;
	char versionStr[64] = { 0 };
	sprintf(versionStr, "版本号: %s", APP_VERSION.c_str());

	auto version = Text::create(GBKToUtf8(versionStr), DEFAULT_FONT, 18);
	if (version) {
		version->setName("version");
		version->setColor(Color3B(155, 151, 194));
		version->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		version->setPosition(Vec2(/*WIN_SIZE.width-*/120, 10));
		addChild(version);
	}

	/**********************登录界面********************/
	landLayer->onLoginCallBack = [this](const std::string &name, const std::string &psd) {

		if (!this->judgeIsUpdateNewVerison()) {

			return;
		}

		CommandManager::getInstance()->addLoaderCommand1(GetText("LOGING"), gFontConfig_30);

		LoginInfo loginInfo;
		loginInfo.name = name;
		loginInfo.password = psd;

		//记录当前是否为游客登陆 key IS_LOCAL_LAND wzx 2017 5 4;
		UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
		UserDefault::getInstance()->setBoolForKey(WX_LOGIN, false);
		UserDefault::getInstance()->flush();

		VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_ACCOUNT_TYPE);
		cmd->setLoginInfo(loginInfo);
		cmd->setLoginDelegate(m_loginDelegate);
		cmd->execute();
	};

	landLayer->youKeLoginCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
	{

	};

	landLayer->onRegistCallBack = [=]()
	{
		landLayer->setVisible(false);
		//logonLayer->setVisible(false);
		//findPwLayer->setVisible(false);
		registerLayer->setVisible(true);
		registerLayer->initRegisterUI();
	};

	landLayer->onFindPwCallBack = [=]()
	{
		//landLayer->setVisible(false);
		//logonLayer->setVisible(false);
		//registerLayer->setVisible(false);
		//findPwLayer->setVisible(true);
		findPwLayer->initFindPwUI();
	};

	landLayer->onBackCallBack = [=]()
	{
		landLayer->setVisible(false);
		//findPwLayer->setVisible(false);
		//registerLayer->setVisible(false);
		logonLayer->setVisible(true);
	};

	logonLayer->wechatLogOnCallBack = [this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			if (!this->judgeIsUpdateNewVerison())
			{
				return;
			}

			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
			UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
			UserDefault::getInstance()->setBoolForKey(WX_LOGIN, true);
			UserDefault::getInstance()->flush();
			CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("微信登陆中......"));

			VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_WECHAT_TYPE);
			cmd->setLoginDelegate(m_loginDelegate);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			WechatMgr::sendAuthRequestScope();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			WeChatAndroid::getInstance()->goToWeiXin();
#endif
		}
		break;
		default:
			break;
		}

	};

	logonLayer->onLogOnCallBack = [=]()
	{
		//logonLayer->setVisible(false);
		//findPwLayer->setVisible(false);
		//registerLayer->setVisible(false);
		landLayer->setVisible(true);
	};


	logonLayer->localLogonCallBack = [this]()
	{
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		if (!this->judgeIsUpdateNewVerison())
		{
			return;
		}

		isVisitor = true;
		//记录当前是否为游客登陆 key IS_LOCAL_LAND wzx 2017 5 4;
		UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", true);
		UserDefault::getInstance()->setBoolForKey(WX_LOGIN, false);
		UserDefault::getInstance()->flush();

		CommandManager::getInstance()->addLoaderCommand1(GetText("LOCALLOGIN"), gFontConfig_30);

		VertifyCodeCom* cmd = CommandManager::getInstance()->checkVertifyCode(LOGIN_DEVICE_TYPE);
		cmd->setLoginDelegate(m_loginDelegate);
		cmd->execute();
	};



	/*********************************************************/

	/*******************账号注册*****************************/
	registerLayer->onCloseCallBack = [=]()
	{
		registerLayer->setVisible(false);
		//findPwLayer->setVisible(false);
		logonLayer->setVisible(true);
		//landLayer->setVisible(true);
//        _login_ui.girl->setVisible(true);
//        _login_ui.bg->setVisible(true);
	};

	registerLayer->onRegisterCallBack = [this](const std::string &name, const std::string &psd, BYTE gender, const std::string nickName, const std::string inviteCode)
	{
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
		registerLayer->setVisible(false);
		findPwLayer->setVisible(false);
		logonLayer->setVisible(false);
		landLayer->setVisible(true);
		//        _login_ui.girl->setVisible(false);
		//        _login_ui.bg->setVisible(false);
	};

	return true;
}

void GameMenuCompositeV66::handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
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
bool GameMenuCompositeV66::judgeIsUpdateNewVerison() //是否有新版本没有下载
{
	auto isNotUpdate = GameData::getInstance()->getIsNotUpdateNewVerison();
	if (isNotUpdate) {

		auto promptLayer = GamePromptLayer::create();
		promptLayer->setPrompt("为了您的游戏体验，请更新最新版本的游戏");
		addChild(promptLayer);

		return false;
	}

	return true;

}
