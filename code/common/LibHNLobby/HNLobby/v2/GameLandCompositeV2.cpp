//
//  GameLandCompositeV2.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameLandCompositeV2.h"
//#include "HNLobby/GameMenu/GameLandLayer.h"
#include "HNLobby//GameExitLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "VersionInfo/VersionInfo.h"


static const char* LOGIN_NODE_CSB = "csb/platform/loginUI/loginNode.csb";

//#define Word_Empty_Name		GBKToUtf8("账号不能为空!")
//#define Word_Empty_Pass		GBKToUtf8("密码不能为空!")

GameLandCopositeV2::GameLandCopositeV2()
//:
//onLoginCallBack(nullptr)
//, onRegistCallBack(nullptr)
//, onFindPwCallBack(nullptr)
{
    memset(&_login_node, 0x0, sizeof(_login_node));
}

GameLandCopositeV2::~GameLandCopositeV2() {
    
}

bool GameLandCopositeV2::init() {
    if (!HNLayer::init()) return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    // 登录界面ui
    _login_node.node = CSLoader::createNode(LOGIN_NODE_CSB);
    //auto pos = GD_NEED_OPEN != true ? Vec2(winSize.width*0.75f, winSize.height*0.5f) : Vec2(winSize / 2);
    //_login_node.node->setPosition(pos);
    addChild(_login_node.node);
    
    auto userDefault = UserDefault::getInstance();
    std::string username = userDefault->getStringForKey(USERNAME_TEXT);
    std::string password = userDefault->getStringForKey(PASSWORD_TEXT);
	userDefault->setBoolForKey(SAVE_TEXT, true);
    bool save = userDefault->getBoolForKey(SAVE_TEXT);
    //账号输入
    auto accountText = (TextField*)_login_node.node->getChildByName("TextField_account");
    accountText->setVisible(false);
//	_login_node.accountEdit->setString("wzx2016");
    _login_node.accountEdit = HNEditBox::createEditBox(accountText, this);
    _login_node.accountEdit->setString(username);
    
    // 保存密码复选框
    _login_node.keepPwCheckBox = (CheckBox*)_login_node.node->getChildByName("CheckBox_keepPassword");
    _login_node.keepPwCheckBox->setSelected(save);
    _login_node.keepPwCheckBox->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameLandCopositeV2::checkBoxCallback, this)));
    
    //密码输入
    auto passwordText = (TextField*)_login_node.node->getChildByName("TextField_password");
    passwordText->setVisible(false);
    _login_node.passwordEdit = HNEditBox::createEditBox(passwordText, this);
    _login_node.passwordEdit->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
	//_login_node.passwordEdit->setString("1234567");
    if (_login_node.keepPwCheckBox->isSelected()) {
        _login_node.passwordEdit->setString(password);
    }
    
    ////登录按钮
    auto btn = (Button*)_login_node.node->getChildByName("Button_login");
    auto Button_youke = (Button*)_login_node.node->getChildByName("Button_youke");
    btn->setVisible(false);
    Button_youke->setVisible(false);
    //_login_node.loginBtn->addTouchEventListener(CC_CALLBACK_2(LandLayer::landEventCallback, this));
    
    
    // 注册帐号按钮
    _login_node.registerBtn = (ImageView*)_login_node.node->getChildByName("Image_register");
    _login_node.registerBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV2::registEventCallback, this));
    
    // 找回密碼
    _login_node.findPwBtn = (ImageView*)_login_node.node->getChildByName("Image_missPassword");
    _login_node.findPwBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV2::findPwEventCallback, this));
    
    // 关闭按钮
    _login_node.exitBtn = (Button*)_login_node.node->getChildByName("Button_exit");
    _login_node.exitBtn->setShowEffect(false);
    _login_node.exitBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV2::closeEventCallback, this));
    
    _login_node.loginBtn = AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
    _login_node.loginBtn->setName("Button_login");
    _login_node.loginBtn->setTitle("button/redLogin.png");
    _login_node.loginBtn->setTitleOffset(Vec2(-4, 4));
    _login_node.loginBtn->setPosition(btn->getPosition());
    _login_node.loginBtn->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV2::landEventCallback, this));
    _login_node.node->addChild(_login_node.loginBtn);
    
    _login_node.youke = AUIButton::create("button/blueBg.png", "button/blueBg.png");
    _login_node.youke->setName("Button_login");
    _login_node.youke->setTitle("button/phoneLogin.png");
    _login_node.youke->setTitleOffset(Vec2(-4, 4));
    _login_node.youke->setPosition(Button_youke->getPosition());
    _login_node.youke->addTouchEventListener([this](Ref * pSender, cocos2d::ui::Widget::TouchEventType type) {
        this->youKeLoginCallBack(pSender, type);
    });
    _login_node.node->addChild(_login_node.youke);
    
    auto backGround = static_cast<Sprite*>(_login_node.node->getChildByName("bg"));
    char versionStr[64] = {0};
    sprintf(versionStr, "v %s", APP_VERSION.c_str());
    auto version = Text::create(versionStr, "platform/fonts/simhei.ttf", 26);
    version->setContentSize(Size(100, 26));
    version->setColor(Color3B::WHITE);
    version->setPosition(Vec2(version->getContentSize().width + 6, version->getContentSize().height + 6));
    backGround->addChild(version, 10);

	// 显示更多账号
	_login_node.Button_moreAss = (Button*)_login_node.node->getChildByName("Button_moreAss");
	_login_node.Button_moreAss->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV2::moreAssCallBack, this));

	// 账号listview
	_login_node.ListView_moreAss = (ListView*)_login_node.node->getChildByName("ListView_moreAss");
	_login_node.ListView_moreAss->setVisible(false);
	_login_node.ListView_moreAss->setScrollBarAutoHideEnabled(false);
	auto zorderValue = _login_node.youke->getLocalZOrder();
	_login_node.ListView_moreAss->setLocalZOrder(zorderValue  + 1);

	for (auto layout : _login_node.ListView_moreAss->getChildren())
	{
		auto tmp = static_cast<Layout*>(layout);
		tmp->addTouchEventListener(CC_CALLBACK_2(GameLandCopositeV2::layoutCallBack, this));
	}
    
    if (!LOGIN_ONE_KEY) {
        //隐藏快捷登陆
        //        //快捷登陆
        //        auto onekeyLogin = (Sprite*)_login_node.node->getChildByName("kuaijiedenglu_3");
        //        onekeyLogin->setVisible(false);
        //        //QQ快捷登陆
        //        _login_node.weixinBtn = (Button*)_login_node.node->getChildByName("Button_qq");
        //        _login_node.weixinBtn->setVisible(false);
        //        //微信快捷登陆
        //        _login_node.weixinBtn = (Button*)_login_node.node->getChildByName("Button_winxin");
        //        _login_node.weixinBtn->setVisible(false);
    } else {
        //快捷登陆
        
        //QQ快捷登陆
        _login_node.weixinBtn = (Button*)_login_node.node->getChildByName("Button_qq");
        //        _login_node.weixinBtn->setVisible(false);
        //        _login_node.weixinBtn->addTouchEventListener(CC_CALLBACK_2(LandLayer::closeEventCallback, this));
        //微信快捷登陆
    }
    //      _login_node.weixinBtn = (Button*)_login_node.node->getChildByName("Button_winxin")
    //      _login_node.weixinBtn->setVisible(true);
    //_login_node.weixinBtn->addTouchEventListener(CC_CALLBACK_2(LandLayer::weiXInLoginCallBack, this));
    

	addDataMoreList();

    // 2016 9 5 添加本地数据 key =  curSelectedGameData 默认值为0 记录当前选择游戏列表位置  用于gameList类
    UserDefault::getInstance()->setDoubleForKey("curSelectedGameData", 0);
    UserDefault::getInstance()->setIntegerForKey("LAST_GAME_ID",0);
    
    return true;
}

void GameLandCopositeV2::layoutCallBack(Ref* pSender, Widget::TouchEventType type)
{
	auto layout = (Layout*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
													   auto text = static_cast<Label*>(layout->getChildByName("account"));
													   if (!text)
														   return;
													   auto account = text->getString();
													   auto pass = UserDefault::getInstance()->getStringForKey(account.c_str());
													 
													   _login_node.accountEdit->setString(account);
													   _login_node.passwordEdit->setString(pass);

													   break; 
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void GameLandCopositeV2::landEventCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;
    
    switch (type) {
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
            btn->setColor(Color3B(155, 155, 155));
            break;
	}
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

						auto pass = _login_node.passwordEdit->getString();
						auto account = _login_node.accountEdit->getString();
						UserDefault::getInstance()->setStringForKey(account.c_str(), pass.c_str());
						
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

void GameLandCopositeV2::weiXInLoginCallBack(Ref* pSender, Widget::TouchEventType type) {
    CCLOG("========================= Weixin");
}

	void GameLandCopositeV2::closeEventCallback(Ref* pSender, Widget::TouchEventType type) {
		Button* selectedBtn = (Button*)pSender;
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			selectedBtn->setColor(Color3B(170, 170, 170));
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED: {
															 HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
															 selectedBtn->setColor(Color3B(255, 255, 255));

															 auto exit = GameExitLayer::create();
															 exit->setPosition(Size::ZERO);
															 this->getParent()->addChild(exit, 100000000);
		}
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			selectedBtn->setColor(Color3B(255, 255, 255));
			break;
		default:
			break;
		}
	}

	void GameLandCopositeV2::checkBoxCallback(Ref* pSender, CheckBox::EventType type) {
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
		auto flag = userDefault->getBoolForKey(SAVE_TEXT);
		CCLOG("end");
	}

	// 注册按钮回调函数
	void GameLandCopositeV2::registEventCallback(Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::ENDED != type) return;
		auto btn = (Button*)pSender;

		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

		onRegistCallBack();
	}

	// 忘記密碼回調
	void GameLandCopositeV2::findPwEventCallback(Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::ENDED != type) return;
		auto btn = (Button*)pSender;

		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

		onFindPwCallBack();
	}

	void GameLandCopositeV2::moreAssCallBack(Ref* pSender, Widget::TouchEventType type)
	{
		Button* selectedBtn = (Button*)pSender;
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::BEGAN:

			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED: {
															 auto flag = _login_node.ListView_moreAss->isVisible();
															 _login_node.ListView_moreAss->setVisible(!flag);
															 break;
		}
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	}

	void GameLandCopositeV2::addDataMoreList()
	{
		auto totleAccStr = UserDefault::getInstance()->getStringForKey("RECORD_SIGN_ACCOUNT");
		std::string array[5] = { "", "", "", "", "" };

		vector<Layout*> vecLayout;
		for (auto layout : _login_node.ListView_moreAss->getChildren())
		{			
			vecLayout.push_back(static_cast<Layout*>(layout));
		}
		log("GameLandCopositeV2addDataMoreList()");
		if (totleAccStr != "")
		{
			int i = 0;
			for (auto charTmp : totleAccStr)
			{
				if (charTmp != '_')
				{
					array[i] = array[i] + charTmp;
				}
				else
				{

					Label* lb = Label::createWithTTF(array[i], "platform/fonts/simhei.ttf", 32);
					lb->setName("account");
					lb->setPosition(Vec2(100, 16));
					vecLayout[i]->addChild(lb);

				//	lb->enableShadow(Color4B::GREEN, Size(10, 10)); //阴影
				//	lb->enableOutline(Color4B::WHITE, 3);
					i++;
				}
			}
		}
	}