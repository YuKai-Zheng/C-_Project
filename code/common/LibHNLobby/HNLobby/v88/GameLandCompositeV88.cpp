#include "GameLandCompositeV88.h"
#include "PlatformResV88.h"

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

static const char* LOGIN_NODE_CSB = "csb/v88/loginUI/loginNode_v88.csb";

GameLandCopositeV88::GameLandCopositeV88()
//:
//onLoginCallBack(nullptr)
//, onRegistCallBack(nullptr)
//, onFindPwCallBack(nullptr)
{
    memset(&_login_node, 0x0, sizeof(_login_node));
}

GameLandCopositeV88::~GameLandCopositeV88() {

}

void GameLandCopositeV88::onExitTransitionDidStart() {
	HNLayer::onExitTransitionDidStart();
//	HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameLandCopositeV88::onEnter() {
	HNLayer::onEnter();

//	if (VERSION_EIGHTY_EIGHT) {
//		if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
//			HNAudioEngine::getInstance()->stopBackgroundMusic();
//		}
//		HNAudioEngine::getInstance()->playBackgroundMusic(LAND_BACKGROUND_MUSIC_V88, true);
//	}
//	else {
//		HNAudioEngine::getInstance()->playBackgroundMusic(LAND_BACKGROUND_MUSIC_V88, true);
//	}
}

bool GameLandCopositeV88::init() {
    if (!HNLayer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();

    // 登录界面ui
    _login_node.node = CSLoader::createNode(LOGIN_NODE_CSB);
//    _login_node.node->setPositionY(-50);
    addChild(_login_node.node);

    auto userDefault = UserDefault::getInstance();
    std::string username = userDefault->getStringForKey(USERNAME_TEXT);
    std::string password = userDefault->getStringForKey(PASSWORD_TEXT);
    bool save = userDefault->getBoolForKey(SAVE_TEXT);

    //账号输入
    auto accountText = (TextField*)_login_node.node->getChildByName("TextField_account");
    accountText->setPlaceHolderColor(FONT_COLOR_ORANGE_V88);
    accountText->setVisible(false);
    _login_node.accountEdit = HNEditBox::createEditBox(accountText, this);
    _login_node.accountEdit->setString(username);

    // 保存密码复选框
    _login_node.keepPwCheckBox = (CheckBox*)_login_node.node->getChildByName("CheckBox_keepPassword");
    _login_node.keepPwCheckBox->setSelected(save);
    _login_node.keepPwCheckBox->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameLandCopositeV88::checkBoxCallback, this)));

    //密码输入
    auto passwordText = (TextField*)_login_node.node->getChildByName("TextField_password");
    passwordText->setPlaceHolderColor(FONT_COLOR_ORANGE_V88);
    passwordText->setVisible(false);
    _login_node.passwordEdit = HNEditBox::createEditBox(passwordText, this);
    _login_node.passwordEdit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    if (_login_node.keepPwCheckBox->isSelected()) {
        _login_node.passwordEdit->setString(password);
    }

    //登录按钮
    auto Button_login = (Button*)_login_node.node->getChildByName("Button_login");
    if (Button_login) {
        Button_login->setVisible(false);
    }
    
    // 注册帐号按钮
    auto Button_Register = (Button*)_login_node.node->getChildByName("Button_register");
    if (Button_Register) {
        Button_Register->setVisible(false);
    }
    

    
    // 找回密碼
    _login_node.findPwBtn = (ImageView*)_login_node.node->getChildByName("Image_missPassword");
    _login_node.findPwBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV88::findPwEventCallback, this));
    if (Tools::isiOSReview()) {
        _login_node.findPwBtn->setVisible(false);
    }

    // 关闭按钮
    _login_node.exitBtn = (Button*)_login_node.node->getChildByName("Button_back");
    _login_node.exitBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV88::closeEventCallback, this));
    _login_node.exitBtn->setPosition(Vec2(10, WIN_SIZE.height - 10) - WIN_SIZE / 2);

    _login_node.loginBtn = AUIButton::create("v88/button/yellow4/da.png", "v88/button/yellow4/da.png");
    _login_node.loginBtn->setName("Button_login");
    _login_node.loginBtn->setTitle("v88/button/yellow4/2.png");
    _login_node.loginBtn->setTitleOffset(Vec2(0, 4));
    _login_node.loginBtn->setPosition(Button_login->getPosition());
    _login_node.loginBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV88::landEventCallback, this));
    _login_node.node->addChild(_login_node.loginBtn);

    _login_node.registerBtn = AUIButton::create("v88/button/blue2/6.png", "v88/button/blue2/6.png");
    _login_node.registerBtn->setName("Button_register");
    _login_node.registerBtn->setTitle("v88/button/blue2/7.png");
    _login_node.registerBtn->setTitleOffset(Vec2(0, 4));
    _login_node.registerBtn->setPosition(Button_Register->getPosition());
    _login_node.registerBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV88::registEventCallback, this));
    _login_node.node->addChild(_login_node.registerBtn);
    
    auto bg = dynamic_cast<Sprite*>(_login_node.node->getChildByName("bg"));
    Size size = Size(469, 141);
    if (bg) {
        size = bg->getContentSize();
    }
    
    auto logo = dynamic_cast<Sprite*>(_login_node.node->getChildByName("Sprite_2"));
    if (logo) {
        logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        logo->setPositionY(WIN_SIZE.height / 2 - 10 - (WIN_SIZE.height / 2 - 10 - size.height / 2) / 2);
        logo->setVisible(false);
        
        Tools::addAnimationFrame("v88/animation/entry/logo/entry_logo.csb", _login_node.node, "entry_logo", Vec2(0, logo->getPositionY()), logo->getScale());
    }
    
    if (_login_node.loginBtn) {
        _login_node.loginBtn->setPositionY((_login_node.findPwBtn->getPositionY() + (-WIN_SIZE.height / 2 + 50)) / 2);
    }
    if (_login_node.registerBtn) {
        _login_node.registerBtn->setPositionY((_login_node.findPwBtn->getPositionY() + (-WIN_SIZE.height / 2 + 50)) / 2);
    }

	UserDefault::getInstance()->setIntegerForKey(ENTER_GAME_LIST_POS, 0);

    return true;
}

void GameLandCopositeV88::landEventCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        btn->setColor(Color3B(155, 155, 155));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        btn->setColor(Color3B(255, 255, 255));

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
                if (passWord.compare(savePassWord) == 0) {
                    onLoginCallBack(userName, passWord);
                } else {
                    passWord = MD5_CTX::MD5String(passWord);
                    onLoginCallBack(userName, passWord);
                }
            }
        } while (0);
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        btn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

void GameLandCopositeV88::weiXInLoginCallBack(Ref* pSender, Widget::TouchEventType type) {
    CCLOG("========================= Weixin");
}


void GameLandCopositeV88::closeEventCallback(Ref* pSender, Widget::TouchEventType type) {
//    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
        onBackCallBack();
//          SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
//			AnimationCache::getInstance()->destroyInstance();
//			// CocosDenshion::SimpleAudioEngine::getInstance()->end();
//			//AudioEngine::end();
//			Director::getInstance()->end();
//
//			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//							exit(0);
//			#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//							exit(0);
//			#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//							exit(0);
//			#endif
    }
    break;
    }
}

void GameLandCopositeV88::checkBoxCallback(Ref* pSender, CheckBox::EventType type) {
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
void GameLandCopositeV88::registEventCallback(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

    onRegistCallBack();
}

// 忘記密碼回調
void GameLandCopositeV88::findPwEventCallback(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

    onFindPwCallBack();
}
