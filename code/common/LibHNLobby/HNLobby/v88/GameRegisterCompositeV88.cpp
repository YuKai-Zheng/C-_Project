#include "GameRegisterCompositeV88.h"
#include "PlatformResV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNMarketExport.h"
#include "VersionInfo/VersionInfo.h"

static const char* REGISTERUI_NODE_CSB = "csb/v88/loginUI/registerNode_v88.csb";


GameRegisterCompositeV88::GameRegisterCompositeV88()
: _visiblePassword("")
, _gender(1) {
    memset(&_regist_node, 0x0, sizeof(_regist_node));
}

GameRegisterCompositeV88::~GameRegisterCompositeV88() {

}

bool GameRegisterCompositeV88::init() {
    if (!HNLayer::init()) return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    // 注册界面ui
    _regist_node.node = CSLoader::createNode(REGISTERUI_NODE_CSB);
    addChild(_regist_node.node);
    
    // 关闭按钮
    _regist_node.backBtn = (Button*)_regist_node.node->getChildByName("Button_back");
    _regist_node.backBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV88::closeClickCallback, this));
    _regist_node.backBtn->setPosition(Vec2(10, WIN_SIZE.height - 10) - WIN_SIZE / 2);
    
    // 账号输入框
    auto accoutText = (TextField*)_regist_node.node->getChildByName("TextField_account");
	accoutText->setPlaceHolderColor(FONT_COLOR_ORANGE_V88);
    accoutText->setVisible(false);
    _regist_node.accountEdit = HNEditBox::createEditBox(accoutText, this);
    
    // 密码输入框
    auto pwText = (TextField*)_regist_node.node->getChildByName("TextField_pw");
	pwText->setPlaceHolderColor(FONT_COLOR_ORANGE_V88);
    pwText->setVisible(false);
    _regist_node.passwordEdit = HNEditBox::createEditBox(pwText, this);
    
    //确认密码
    auto checkPwText = (TextField*)_regist_node.node->getChildByName("TextField_pwCheck");
	checkPwText->setPlaceHolderColor(FONT_COLOR_ORANGE_V88);
    checkPwText->setVisible(false);
    _regist_node.pwCheckEdit = HNEditBox::createEditBox(checkPwText, this);
    
    //邀请码
	auto inviteCodeText = (TextField*)_regist_node.node->getChildByName("TextField_invite");
	inviteCodeText->setVisible(false);
	_regist_node.inviteCodeEdit = HNEditBox::createEditBox(inviteCodeText, this);
    
    auto invite = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("Sprite_Invite"));
    if (Tools::isiOSReview() || true) {
        if (_regist_node.inviteCodeEdit) {
            _regist_node.inviteCodeEdit->setVisible(false);
        }
        if (invite) {
            invite->setVisible(false);
        }
    }
    
    // 注册按钮
    auto btn = (Button*)_regist_node.node->getChildByName("Button_register");
    btn->setVisible(false);
    
    _regist_node.registerBtn = AUIButton::create("v88/button/blue2/6.png", "v88/button/blue2/6.png");
    _regist_node.registerBtn->setName("Button_register");
    _regist_node.registerBtn->setTitle("v88/button/blue2/9.png");
    _regist_node.registerBtn->setTitleOffset(Vec2(-4, 4));
    _regist_node.registerBtn->setPosition(btn->getPosition());
    _regist_node.registerBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV88::registerClickCallback, this));
    _regist_node.node->addChild(_regist_node.registerBtn);
    
    auto bg = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("bg"));
    Size size = Size(488, 280);
    if (bg) {
        size = bg->getContentSize();
    }
    auto logo = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("Sprite_1"));
    if (logo) {
        logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        logo->setPositionY(WIN_SIZE.height / 2 - 10 - (WIN_SIZE.height / 2 - 10 - size.height / 2) / 2);
    }
    
    if (_regist_node.registerBtn) {
        _regist_node.registerBtn->setPositionY((-size.height / 2 + (-WIN_SIZE.height / 2 + 50)) / 2);
    }
    
    return true;
}

void GameRegisterCompositeV88::registerClickCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;
    
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
                
                std::string nickEdit = "";
				std::string inviteCodeEdit = Utf8ToGB(_regist_node.inviteCodeEdit->getText());
                
                if (nullptr != onRegisterCallBack) {
					//记录当前是否为游客登陆 key		 IS_LOCAL_LAND wzx 2017 5 4 
					UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
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

void GameRegisterCompositeV88::resetClickCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
}

void GameRegisterCompositeV88::initRegisterUI() {
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// #else
//     _regist_node.manCheckBox->setSelected(false);
//     _regist_node.womanCheckBox->setSelected(false);
// #endif
    _regist_node.accountEdit->setString("");
    _regist_node.passwordEdit->setString("");
    _regist_node.pwCheckEdit->setString("");
}

void GameRegisterCompositeV88::closeClickCallback(Ref* pSender, Widget::TouchEventType type) {
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

void GameRegisterCompositeV88::AgreeClickCallBack(Ref* pSender, Widget::TouchEventType type) {
    
}

void GameRegisterCompositeV88::checkBoxCallback(Ref* pSender, CheckBox::EventType type) {
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

void GameRegisterCompositeV88::editBoxEditingDidEnd(ui::EditBox* editBox) {
    
}

void GameRegisterCompositeV88::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {

}
