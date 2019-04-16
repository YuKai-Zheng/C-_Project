#include "GameRegisterCompositeV66.h"
#include "PlatformResV66.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNMarketExport.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/GameData.h"
#include "../pattern/CommandManager.h"

static const char* REGISTERUI_NODE_CSB = "csb/newplatform/login/node_regist.csb";

#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())


GameRegisterCompositeV66::GameRegisterCompositeV66()
: _visiblePassword("")
, _gender(1) {
    memset(&_regist_node, 0x0, sizeof(_regist_node));
}

GameRegisterCompositeV66::~GameRegisterCompositeV66() {

}

bool GameRegisterCompositeV66::init() {
    if (!HNLayer::init()) return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    // 注册界面ui
    _regist_node.node = CSLoader::createNode(REGISTERUI_NODE_CSB);
	_regist_node.node->setPositionX(300);
    addChild(_regist_node.node);
    
    // 关闭按钮
	_regist_node.backBtn = (Button*)_regist_node.node->getChildByName("bg")->getChildByName("Button_back");
	_regist_node.backBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV66::closeClickCallback, this));


	_regist_node.codeBtn = (Button*)_regist_node.node->getChildByName("bg")->getChildByName("Button_Get");
	_regist_node.codeBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV66::codeClickCallBack, this));

	auto codeNumber = static_cast<Text*>(_regist_node.codeBtn->getChildByName("Text_number"));
	codeNumber->setVisible(false);
	_regist_node.number = codeNumber;
    
    // 账号输入框
    auto accoutText = (TextField*)_regist_node.node->getChildByName("bg")->getChildByName("accountbg")->getChildByName("txt");
	//accoutText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    accoutText->setVisible(false);
    _regist_node.accountEdit = HNEditBox::createEditBox(accoutText, this);

	//验证码;
	auto codeText = (TextField*)_regist_node.node->getChildByName("bg")->getChildByName("codegb")->getChildByName("txt");
	//accoutText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
	codeText->setVisible(false);
	_regist_node.codeEdit = HNEditBox::createEditBox(codeText, this);
    
    // 密码输入框
    auto pwText = (TextField*)_regist_node.node->getChildByName("bg")->getChildByName("newbg")->getChildByName("txt");
	//pwText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    pwText->setVisible(false);
    _regist_node.passwordEdit = HNEditBox::createEditBox(pwText, this);
    
    //确认密码
    auto checkPwText = (TextField*)_regist_node.node->getChildByName("bg")->getChildByName("checkbg")->getChildByName("txt");
	//checkPwText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    checkPwText->setVisible(false);
    _regist_node.pwCheckEdit = HNEditBox::createEditBox(checkPwText, this);
    
    //邀请码
	//auto inviteCodeText = (TextField*)_regist_node.node->getChildByName("TextField_invite");
	//inviteCodeText->setVisible(false);
	//_regist_node.inviteCodeEdit = HNEditBox::createEditBox(inviteCodeText, this);
    
    //auto invite = dynamic_cast<Sprite*>(_regist_node.node->getChildByName("Sprite_Invite"));
    //if (Tools::isiOSReview() || true) {
    //    if (_regist_node.inviteCodeEdit) {
    //        _regist_node.inviteCodeEdit->setVisible(false);
    //    }
    //    if (invite) {
    //        invite->setVisible(false);
    //    }
    //}
    
    // 注册按钮
    //auto btn = (Button*)_regist_node.node->getChildByName("bg")->getChildByName("Button_register");
    //btn->setVisible(false);
    
#if 1
    _regist_node.registerBtn = (Button*)_regist_node.node->getChildByName("bg")->getChildByName("Button_register");// Button::create("v66/button/blue/bg.png", "v66/button/blue/bg_1.png");
    _regist_node.registerBtn->setName("Button_register");
    //_regist_node.registerBtn->setTitle("v66/button/blue/3.png");
    //_regist_node.registerBtn->setTitleOffset(Vec2(-4, 4));
    //_regist_node.registerBtn->setPosition(btn->getPosition()+Vec2(0,-40));
    _regist_node.registerBtn->addTouchEventListener(CC_CALLBACK_2(GameRegisterCompositeV66::registerClickCallback, this));
    //_regist_node.node->addChild(_regist_node.registerBtn);
#endif

    return true;
}

void GameRegisterCompositeV66::registerClickCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            //btn->setColor(Color3B(155, 155, 155));
			btn->setScale(0.8f);
            break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			btn->setScale(1.0f);
			break;
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            //btn->setColor(Color3B(255, 255, 255));
			btn->setScale(1.0f);
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            
            do {
                // 获取输入框账号
                std::string userName = _regist_node.accountEdit->getString();
                
                if (userName.empty()) {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("账号不能为空！"));
                    break;
                }
				std::string strTextCode = MD5_CTX::MD5String(_regist_node.codeEdit->getString());
				std::string strWebCode = GameData::getInstance()->getCodeString();
				
				if (strTextCode.empty() || strWebCode.compare(strTextCode) != 0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("验证码不正确！"));
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
				std::string inviteCodeEdit = "";// = Utf8ToGB(_regist_node.inviteCodeEdit->getText());
                
                if (nullptr != onRegisterCallBack) {
					//记录当前是否为游客登陆 key		 IS_LOCAL_LAND wzx 2017 5 4 
					UserDefault::getInstance()->setIntegerForKey("IS_LOCAL_LAND", false);
                    onRegisterCallBack(userName, passWord, _gender, nickEdit, inviteCodeEdit);
					GameData::getInstance()->setCodeString("");
                    break;
                }
            } while (0);
            break;
        }
        default:
            break;
    }
}

void GameRegisterCompositeV66::resetClickCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn = (Button*)pSender;
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
}

void GameRegisterCompositeV66::codeClickCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type) return;
	auto btn = (Button*)pSender;

	if (_regist_node.codeBtn != btn)
		return;
	
	std::string phoneNumber = _regist_node.accountEdit->getString();
	if (!phoneNumber.empty())
	{
		_fTimeOut = 0.0f;
		schedule(schedule_selector(GameRegisterCompositeV66::UpdateTimer), 1.0f);
		_regist_node.codeBtn->setBright(false);
		_regist_node.codeBtn->setTouchEnabled(false);
		_regist_node.number->setVisible(true);
		_regist_node.number->setString(std::string("60"));

		std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
		std::string str("type=sendcode&mobile=");
		str += phoneNumber;

		network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
		if (request == nullptr)
		{
			return;
		}
		request->setUrl(URL.c_str());
		request->setRequestType(network::HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(GameRegisterCompositeV66::onHttpReqClientResponse, this));
		request->setRequestData(str.c_str(), str.size());
		network::HttpClient::getInstance()->sendImmediate(request);
	}
}

void GameRegisterCompositeV66::onHttpReqClientResponse(network::HttpClient * client, network::HttpResponse * response)
{
	if (client == nullptr || response == nullptr)
		return;

	if (response->isSucceed())
	{
		/////////////////////////////////////////////////////
		std::vector<char> recv = *response->getResponseData();
		std::string data;
		data.assign(recv.begin(), recv.end());

		data.erase(std::remove(data.begin(), data.end(), '}'), data.end());
		data.erase(std::remove(data.begin(), data.end(), '{'), data.end());
		data.erase(std::remove(data.begin(), data.end(), '"'), data.end());

		std::vector<std::string> listData;
		Tools::SplitString(data, listData, std::string(","));
		int result = -1;
		std::string strCode;
		for (int i = 0; i < listData.size() && listData.size()>1; ++i)
		{
			std::vector<std::string> items;
			Tools::SplitString(listData[i], items, std::string(":"));
			if (items.size() > 1)
			{
				if (items[0].compare(std::string("status")) == 0)
				{
					result = Value(items[1]).asInt();
				}
				else if (items[0].compare(std::string("obj")) == 0)
				{
					strCode = Value(items[1]).asString();
				}
			}
		}

		if (result == 0)
		{
			GameData::getInstance()->setCodeString("");
		}
		else if (result == 1)
		{
			GameData::getInstance()->setCodeString(strCode);
		}
	}	
}

void GameRegisterCompositeV66::initRegisterUI() {
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// #else
//     _regist_node.manCheckBox->setSelected(false);
//     _regist_node.womanCheckBox->setSelected(false);
// #endif
    _regist_node.accountEdit->setString("");
    _regist_node.passwordEdit->setString("");
    _regist_node.pwCheckEdit->setString("");
}

void GameRegisterCompositeV66::closeClickCallback(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            //selectedBtn->setColor(Color3B(170, 170, 170));
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
            //selectedBtn->setColor(Color3B(255, 255, 255));
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

void GameRegisterCompositeV66::AgreeClickCallBack(Ref* pSender, Widget::TouchEventType type) {
    
}

void GameRegisterCompositeV66::checkBoxCallback(Ref* pSender, CheckBox::EventType type) {
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

void GameRegisterCompositeV66::editBoxEditingDidEnd(ui::EditBox* editBox) {
    
}

void GameRegisterCompositeV66::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {

}

void GameRegisterCompositeV66::UpdateTimer(float dt)
{
	
	if (_fTimeOut > 60.0f)
	{
		unschedule(schedule_selector(GameRegisterCompositeV66::UpdateTimer));
		_regist_node.codeBtn->setBright(true);
		_regist_node.codeBtn->setTouchEnabled(true);
		_regist_node.number->setVisible(false);
	}
	
	_regist_node.number->setString(StringUtils::format("%d", 60 - (int)_fTimeOut));

	_fTimeOut += dt;
}
