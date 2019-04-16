#include "GameLandCompositeV66.h"
#include "PlatformResV66.h"

#include "HNLobby//GameExitLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "VersionInfo/VersionInfo.h"
#include "Sound/HNAudioEngine.h"

#include "base/WeChatAndroid.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/WechatMgr.h"
#endif

static const char* LOGIN_NODE_CSB = "csb/newplatform/login/node_accountlogin.csb";

GameLandCopositeV66::GameLandCopositeV66()
//:
//onLoginCallBack(nullptr)
//, onRegistCallBack(nullptr)
//, onFindPwCallBack(nullptr)
{
	m_bEdit = false;
    memset(&_login_node, 0x0, sizeof(_login_node));
	m_strPassword = "";
}

GameLandCopositeV66::~GameLandCopositeV66() {

}

void GameLandCopositeV66::onExitTransitionDidStart() {
	HNLayer::onExitTransitionDidStart();
//	HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameLandCopositeV66::onEnter() {
	HNLayer::onEnter();

//	if (VERSION_EIGHTY_EIGHT) {
//		if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
//			HNAudioEngine::getInstance()->stopBackgroundMusic();
//		}
//		HNAudioEngine::getInstance()->playBackgroundMusic(LAND_BACKGROUND_MUSIC_V66, true);
//	}
//	else {
//		HNAudioEngine::getInstance()->playBackgroundMusic(LAND_BACKGROUND_MUSIC_V66, true);
//	}
}

bool GameLandCopositeV66::init() 
{
    if (!HNLayer::init())
	{
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    // 登录界面ui
    _login_node.node = CSLoader::createNode(LOGIN_NODE_CSB);
    _login_node.node->setPositionX(300);
    addChild(_login_node.node);

    auto userDefault = UserDefault::getInstance();
    std::string username = userDefault->getStringForKey(USERNAME_TEXT);
    std::string password = userDefault->getStringForKey(PASSWORD_TEXT);
    bool save = userDefault->getBoolForKey(SAVE_TEXT);

    //账号输入
    auto accountText = (TextField*)_login_node.node->getChildByName("bg")->getChildByName("accountbg")->getChildByName("txt");
    accountText->setVisible(false);
    _login_node.accountEdit = HNEditBox::createEditBox(accountText, this);
    _login_node.accountEdit->setString(username);

    // 保存密码复选框
    _login_node.keepPwCheckBox = (CheckBox*)_login_node.node->getChildByName("bg")->getChildByName("CheckBox_keepPassword");
    _login_node.keepPwCheckBox->setSelected(save);
    //_login_node.keepPwCheckBox->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameLandCopositeV66::checkBoxCallback, this)));

	_login_node.btnPwCheckBox = static_cast<Button*>(_login_node.node->getChildByName("bg")->getChildByName("Button_keepPassword"));
	_login_node.btnPwCheckBox->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV66::keepPasswordEventCallback, this));

	_login_node.haveReadBox = (CheckBox*)_login_node.node->getChildByName("bg")->getChildByName("CheckBox_HaveRead");
	_login_node.haveReadBtn = static_cast<Button*>(_login_node.node->getChildByName("bg")->getChildByName("Button_HaveRead"));
	_login_node.haveReadBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV66::haveReadEventCallback, this));
	

    //密码输入
    auto passwordText = (TextField*)_login_node.node->getChildByName("bg")->getChildByName("newbg")->getChildByName("txt");
    //passwordText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    passwordText->setVisible(false);
    _login_node.passwordEdit = HNEditBox::createEditBox(passwordText, this);
    _login_node.passwordEdit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    if (_login_node.keepPwCheckBox->isSelected()) {
		m_strPassword = password;
        _login_node.passwordEdit->setString("********");
    }

    //登录按钮
    //auto Button_login = (Button*)_login_node.node->getChildByName("bg")->getChildByName("Button_login");
    //if (Button_login) {
    //    Button_login->setVisible(false);
    //}
    
    // 注册帐号按钮
    //auto Button_Register = (Button*)_login_node.node->getChildByName("Button_register");
    //if (Button_Register) {
    //    Button_Register->setVisible(false);
    //}
    

    
    // 找回密碼
//    _login_node.findPwBtn = (ImageView*)_login_node.node->getChildByName("Image_missPassword");
//    _login_node.findPwBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV66::findPwEventCallback, this));
////    if (Tools::isiOSReview())
//    {
//        _login_node.findPwBtn->setVisible(false);
//    }

    // 关闭按钮
    _login_node.exitBtn = (Button*)_login_node.node->getChildByName("Button_back");
    _login_node.exitBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV66::closeEventCallback, this));



#if 1
	_login_node.loginBtn = (Button*)_login_node.node->getChildByName("bg")->getChildByName("Button_login") ;// AUIButton::create("v66/button/blue/bg.png", "v66/button/blue/bg_1.png");
    _login_node.loginBtn->setName("Button_login");
    //_login_node.loginBtn->setTitle("v66/button/blue/1.png");
    //_login_node.loginBtn->setTitleOffset(Vec2(0, 4));
    //_login_node.loginBtn->setPosition(Button_login->getPosition()+Vec2(0,-40));
    _login_node.loginBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV66::landEventCallback, this));
    //_login_node.node->addChild(_login_node.loginBtn);

    _login_node.registerBtn = (Button*)_login_node.node->getChildByName("bg")->getChildByName("btn_register"); //AUIButton::create("v66/button/green/bg1.png", "v66/button/green/bg2.png");
    _login_node.registerBtn->setName("Button_register");
    //_login_node.registerBtn->setTitle("v66/button/green/01.png");
    //_login_node.registerBtn->setTitleOffset(Vec2(0, 4));
    //_login_node.registerBtn->setPosition(Button_Register->getPosition()+Vec2(0,-40));
    _login_node.registerBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV66::registEventCallback, this));
    //_login_node.node->addChild(_login_node.registerBtn);
#endif

	UserDefault::getInstance()->setIntegerForKey(ENTER_GAME_LIST_POS, 0);

    return true;
}

void GameLandCopositeV66::landEventCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        //btn->setColor(Color3B(155, 155, 155));
		btn->setScale(0.8f);
        break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		btn->setScale(1.0f);
		break;

    case cocos2d::ui::Widget::TouchEventType::ENDED:
        //btn->setColor(Color3B(255, 255, 255));
		btn->setScale(1.0f);
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        do {
            // 获取输入框账号
            std::string userName = _login_node.accountEdit->getString();
            if (userName.empty()) {
                GamePromptLayer::create()->showPrompt(GetText("Account_NOT_EMPTY"));
                break;
            }

            // 获取输入框密码
            std::string passWord = _login_node.passwordEdit->getString();
            if (passWord.empty()) {
                GamePromptLayer::create()->showPrompt(GetText("PASSWORLD_NOT_EMPTY"));
                break;
            }

            std::string savePassWord = UserDefault::getInstance()->getStringForKey(PASSWORD_TEXT);
            if (nullptr != onLoginCallBack) {
                if (!m_strPassword.empty() && m_strPassword.compare(savePassWord) == 0) {
                    onLoginCallBack(userName, m_strPassword);
                } else {
                    passWord = MD5_CTX::MD5String(passWord);
                    onLoginCallBack(userName, passWord);
                }
            }
        } while (0);
        break;
    default:
        break;
    }
}

void GameLandCopositeV66::weiXInLoginCallBack(Ref* pSender, Widget::TouchEventType type) {
    CCLOG("========================= Weixin");
}

void GameLandCopositeV66::keepPasswordEventCallback(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	auto btn = (Button*)pSender;

	auto userDefault = UserDefault::getInstance();
	if (_login_node.keepPwCheckBox->isSelected())
	{
		_login_node.keepPwCheckBox->setSelected(false);
		userDefault->setBoolForKey(SAVE_TEXT, true);
	}
	else
	{
		_login_node.keepPwCheckBox->setSelected(true);
		userDefault->setBoolForKey(SAVE_TEXT, false);
		_login_node.passwordEdit->setText("");
	}
	
	userDefault->flush();
}

void GameLandCopositeV66::haveReadEventCallback(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	auto btn = (Button*)pSender;

	if (_login_node.haveReadBox->isSelected())
	{
		_login_node.haveReadBox->setSelected(false);
	}
	else
	{
		_login_node.haveReadBox->setSelected(true);
	}
}


void GameLandCopositeV66::closeEventCallback(Ref* pSender, Widget::TouchEventType type) {
//    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
	case cocos2d::ui::Widget::TouchEventType::MOVED:
	{
		if (_login_node.accountEdit->isFocused() || _login_node.passwordEdit->isFocused())
		{
			m_bEdit = true;
		}
	}
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
		//m_bEdit = false;
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
		if (!m_bEdit)//编辑账户密码的时候点空白处退出键盘，不隐藏账户登陆页面
		{
			HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
			onBackCallBack();
		}

		m_bEdit = false;
    }
    break;
    }
}

void GameLandCopositeV66::checkBoxCallback(Ref* pSender, CheckBox::EventType type) {
    auto userDefault = UserDefault::getInstance();
    switch (type) {
    case cocos2d::ui::CheckBox::EventType::SELECTED:
        userDefault->setBoolForKey(SAVE_TEXT, true);
        break;
    case cocos2d::ui::CheckBox::EventType::UNSELECTED:
        userDefault->setBoolForKey(SAVE_TEXT, false);
        _login_node.passwordEdit->setText("");
        break;
    default:
        break;
    }
    userDefault->flush();
}

// 注册按钮回调函数
void GameLandCopositeV66::registEventCallback(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

    onRegistCallBack();
}

// 忘記密碼回調
void GameLandCopositeV66::findPwEventCallback(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

    onFindPwCallBack();
}
