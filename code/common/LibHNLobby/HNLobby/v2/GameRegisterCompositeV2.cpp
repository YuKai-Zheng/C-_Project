//
//  GameRegisterCompositeV2.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameRegisterCompositeV2.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNMarketExport.h"
#include "VersionInfo/VersionInfo.h"

static const char* REGISTERUI_NODE_CSB = "csb/platform/loginUI/registerNode.csb";


GameRegisterCompositeV2::GameRegisterCompositeV2()
: _visiblePassword("")
, _gender(1) {
    memset(&_regist_node, 0x0, sizeof(_regist_node));
}

GameRegisterCompositeV2::~GameRegisterCompositeV2() {
    
}

bool GameRegisterCompositeV2::init() {
    if (!HNLayer::init()) return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    // 注册界面ui
    _regist_node.node = CSLoader::createNode(REGISTERUI_NODE_CSB);
    //auto pos = GD_NEED_OPEN != true ? Vec2(winSize.width*0.75f, winSize.height*0.5f) : Vec2(winSize / 2);
    //_regist_node.node->setPosition(pos);
    addChild(_regist_node.node);
    
    // 关闭按钮
    _regist_node.backBtn = (Button*)_regist_node.node->getChildByName("Button_back");
    _regist_node.backBtn->setShowEffect(false);
    _regist_node.backBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV2::closeClickCallback, this));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto gender = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("xingbie"));
    gender->setVisible(false);
    auto man = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("man"));
    man->setVisible(false);
    auto woman = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("woman"));
    woman->setVisible(false);
    _regist_node.manCheckBox = (CheckBox*)_regist_node.node->getChildByName("CheckBox_man");
    _regist_node.manCheckBox->setVisible(false);
    _regist_node.womanCheckBox = (CheckBox*)_regist_node.node->getChildByName("CheckBox_woman");
    _regist_node.womanCheckBox->setVisible(false);
#else
    //性别复选框
    _regist_node.manCheckBox = (CheckBox*)_regist_node.node->getChildByName("CheckBox_man");
    _regist_node.manCheckBox->setTag(1);
    _regist_node.manCheckBox->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameRegisterCompositeV2::checkBoxCallback, this)));
    
    _regist_node.womanCheckBox = (CheckBox*)_regist_node.node->getChildByName("CheckBox_woman");
    _regist_node.womanCheckBox->setTag(2);
    _regist_node.womanCheckBox->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameRegisterCompositeV2::checkBoxCallback, this)));
#endif
    // 账号输入框
    auto accoutText = (TextField*)_regist_node.node->getChildByName("TextField_account");
    accoutText->setVisible(false);
    _regist_node.accountEdit = HNEditBox::createEditBox(accoutText, this);
    
    // 昵称
    auto nickText = (TextField*)_regist_node.node->getChildByName("TextField_nick");
    nickText->setVisible(false);
    _regist_node.nickEdit = HNEditBox::createEditBox(nickText, this);
    
    // 密码输入框
    auto pwText = (TextField*)_regist_node.node->getChildByName("TextField_pw");
    pwText->setVisible(false);
    _regist_node.passwordEdit = HNEditBox::createEditBox(pwText, this);
    
    //确认密码
    auto checkPwText = (TextField*)_regist_node.node->getChildByName("TextField_pwCheck");
    checkPwText->setVisible(false);
    _regist_node.pwCheckEdit = HNEditBox::createEditBox(checkPwText, this);
    
    //邀请码
    auto inviteCodeText = (TextField*)_regist_node.node->getChildByName("TextField_yaoqingma");
    inviteCodeText->setVisible(false);
    _regist_node.inviteCodeEdit = HNEditBox::createEditBox(inviteCodeText, this);
    
    // 	auto textField_PassWord = (TextField*)_regist_node.registBG->getChildByName("TextField_passWord");
    // 	textField_PassWord->setVisible(false);
    // 	_regist_node.editBoxPassWord = HNEditBox::createEditBox(textField_PassWord, this);
    // 	_regist_node.editBoxPassWord->setPasswordEnabled(true);
    // 	_regist_node.editBoxPassWordVisible = HNEditBox::createEditBox(textField_PassWord, this);
    // 	_regist_node.editBoxPassWordVisible->setPasswordEnabled(false);
    // 	_regist_node.editBoxPassWordVisible->setVisible(false);
    
    
    // 注册按钮
    auto btn = (Button*)_regist_node.node->getChildByName("Button_register");
    btn->setVisible(false);
    
    _regist_node.registerBtn = AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
    _regist_node.registerBtn->setName("Button_register");
    _regist_node.registerBtn->setTitle("button/redLogin.png");
    _regist_node.registerBtn->setTitleOffset(Vec2(-4, 4));
    _regist_node.registerBtn->setPosition(btn->getPosition());
    _regist_node.registerBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV2::registerClickCallback, this));
    _regist_node.node->addChild(_regist_node.registerBtn);
    //
    // 	// 重置按钮
    // 	Button * buttonReset = (Button*)_regist_node.registBG->getChildByName("Button_reset");
    // 	buttonReset->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::resetClickCallBack, this));
    //
    
    // 	// 同意复选框
    // 	_regist_node.agree = (CheckBox*)_regist_node.registBG->getChildByName("CheckBox_agree");
    // 	_regist_node.agree->setSelected(true);
    //
    // 	// 显示密码复选框
    // 	_regist_node.showPass = (CheckBox*)_regist_node.registBG->getChildByName("CheckBox_showPass");
    // 	_regist_node.showPass->setSelected(false);
    // 	_regist_node.showPass->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(RegisterLayer::checkBoxCallback, this)));
    //
    // 	//协议按钮
    //Button 	// * buttonagree = (Button*)_regist_node.registBG->getChildByName("Button_agree");
    // 	buttonagree->addTouchEventListener(CC_CALLBACK_2(RegisterLayer::AgreeClickCallBack, this));
    
    
    
    return true;
}

void GameRegisterCompositeV2::registerClickCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;
    
    // 	if (!_regist_node.agree->isSelected())
    // 	{
    // 		GamePromptLayer::create()->showPrompt(GBKToUtf8("请勾选同意用户协议！"));
    // 		return;
    // 	}
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            btn->setColor(Color3B(155, 155, 155));
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            btn->setColor(Color3B(255, 255, 255));
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            
            do {
                // 获取输入框账号
                std::string userName = _regist_node.accountEdit->getString();
                
                if (userName.empty()) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("账号不能为空！"));
                    break;
                }
                
                if (_regist_node.accountEdit->getStringLength() < 6 ||
                    _regist_node.accountEdit->getStringLength() > 15 ||
                    Tools::verifyChinese(userName)) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入6-15位英文或数字帐号！"));
                    break;
                }
                
                // 获取输入框密码
                std::string passWord = _regist_node.passwordEdit->getString();
                if (passWord.empty()) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("密码不能为空！"));
                    break;
                }
                
                if (_regist_node.passwordEdit->getStringLength() < 6 ||
                    _regist_node.passwordEdit->getStringLength() > 15 ||
                    Tools::verifyChinese(passWord)) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入6-15位字符密码！"));
                    break;
                }
                
                //获取确认密码
                std::string checkPassWord = _regist_node.pwCheckEdit->getString();
                if (checkPassWord.empty()) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("确认密码不能为空！"));
                    break;
                }
                
                if (0 != passWord.compare(checkPassWord)) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("密码不一致，请检查！"));
                    break;
                }
                
                //获取昵称
                std::string nickEdit = Utf8ToGB(_regist_node.nickEdit->getText());
                if (nickEdit.empty()) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("昵称不能为空！"));
                    break;
                }
                CCLOG("length = %d ", _regist_node.nickEdit->getStringLength());
                if (_regist_node.nickEdit->getStringLength() < 6 ||
                    _regist_node.nickEdit->getStringLength() > 18)			{
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入2-6位昵称！"));
                    break;
                }
                
                std::string inviteCodeEdit = Utf8ToGB(_regist_node.inviteCodeEdit->getText());
                
                if (nullptr != onRegisterCallBack) {

					UserDefault::getInstance()->setStringForKey(userName.c_str(), passWord.c_str());
                    onRegisterCallBack(userName, passWord, _gender, nickEdit, inviteCodeEdit);
                    break;
                }
            } while (0);
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            btn->setColor(Color3B(255, 255, 255));
            break;
        default:
            break;
    }
}

void GameRegisterCompositeV2::resetClickCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    
    // 	_regist_node.editBoxUserName->setText("");
    // 	_regist_node.editBoxPassWord->setText("");
}

void GameRegisterCompositeV2::initRegisterUI() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
    _regist_node.manCheckBox->setSelected(false);
    _regist_node.womanCheckBox->setSelected(false);
#endif
    _regist_node.accountEdit->setString("");
    _regist_node.nickEdit->setString("");
    _regist_node.passwordEdit->setString("");
    _regist_node.pwCheckEdit->setString("");
}

void GameRegisterCompositeV2::closeClickCallback(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            selectedBtn->setColor(Color3B(170, 170, 170));
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
            selectedBtn->setColor(Color3B(255, 255, 255));
            onCloseCallBack();
        }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            selectedBtn->setColor(Color3B(255, 255, 255));
            break;
        default:
            break;
    }
}

void GameRegisterCompositeV2::AgreeClickCallBack(Ref* pSender, Widget::TouchEventType type) {
    
}

void GameRegisterCompositeV2::checkBoxCallback(Ref* pSender, CheckBox::EventType type) {
    auto checkBox = (CheckBox*)pSender;
    int tag = checkBox->getTag();
    
    if (type == CheckBox::EventType::SELECTED) {
        if (1 == tag) {
            _gender = 1;
            _regist_node.womanCheckBox->setSelected(false);
        } else if (2 == tag) {
            _gender = 0;
            _regist_node.manCheckBox->setSelected(false);
        }
    }
}

void GameRegisterCompositeV2::editBoxEditingDidEnd(ui::EditBox* editBox) {
    
}

void GameRegisterCompositeV2::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {
    // 	if (editBox == _regist_node.editBoxPassWord)
    // 	{
    // 		_regist_node.editBoxPassWordVisible->setString(text);
    // 	}
    // 	else if (editBox == _regist_node.editBoxPassWordVisible)
    // 	{
    // 		_regist_node.editBoxPassWord->setString(text);
    // 	}
}
