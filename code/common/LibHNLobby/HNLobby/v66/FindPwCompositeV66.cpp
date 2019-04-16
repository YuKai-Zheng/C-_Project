#include "FindPwCompositeV66.h"
#include "PlatformResV66.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformDefine.h"
#include "VersionInfo/VersionInfo.h"
#include "../FontConfig.h"
#include "UI/LoadingLayer.h"

static const char* FIND_NODE_CSB = "csb/v66/loginUI/findPwNode_v66.csb";

FindPwCompositeV66::FindPwCompositeV66() {
    memset(&_find_node, 0x0, sizeof(_find_node));
}

FindPwCompositeV66::~FindPwCompositeV66() {

}

bool FindPwCompositeV66::init() {
    if (!HNLayer::init()) return false;

    //加载csb
    _find_node.node = CSLoader::createNode(FIND_NODE_CSB);
    addChild(_find_node.node);

    //返回按钮
    _find_node.backBtn = (Button*)_find_node.node->getChildByName("Button_back");
	_find_node.backBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV66::closeClickCallback, this));
//    _find_node.backBtn->setPosition(Vec2(10, WIN_SIZE.height - 10) - WIN_SIZE / 2);
    
    //账号输入
    auto accountText = (TextField*)_find_node.node->getChildByName("TextField_account");
    accountText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    accountText->setVisible(false);
    _find_node.accountEdit = HNEditBox::createEditBox(accountText, this);

    //手机号输入
    auto phoneText = (TextField*)_find_node.node->getChildByName("TextField_phone");
    phoneText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    phoneText->setVisible(false);
    _find_node.phoneEdit = HNEditBox::createEditBox(phoneText, this);

    //验证码输入
    auto codeText = (TextField*)_find_node.node->getChildByName("TextField_code");
    codeText->setPlaceHolderColor(FONT_COLOR_ORANGE_V66);
    codeText->setVisible(false);
    _find_node.codeEdit = HNEditBox::createEditBox(codeText, this);

    //发送验证码按钮
    _find_node.sendCodeBtn = (Button*)_find_node.node->getChildByName("Button_sendCode");
    _find_node.sendCodeBtn->setShowEffect(false);
	_find_node.sendCodeBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV66::sendCodeCallback, this));

    //find按钮
    auto btn = (Button*)_find_node.node->getChildByName("Button_find");
    btn->setVisible(false);

    _find_node.findBtn = AUIButton::create("v66/button/blue2/6.png", "v66/button/blue2/6.png");
    _find_node.findBtn->setName("Button_sendCode");
    _find_node.findBtn->setTitle("v66/button/blue2/8.png");
    _find_node.findBtn->setTitleOffset(Vec2(-4, 4));
    _find_node.findBtn->setPosition(btn->getPosition()+Vec2(0,-40));
	_find_node.findBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV66::sendFindCallback, this));
    _find_node.node->addChild(_find_node.findBtn);
    
    auto bg = dynamic_cast<Sprite*>(_find_node.node->getChildByName("bg"));
    Size size = Size(469, 141);
    if (bg) {
        size = bg->getContentSize();
    }
    
    auto logo = dynamic_cast<Sprite*>(_find_node.node->getChildByName("Sprite_1"));
    if (logo) {
        logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        logo->setPositionY(WIN_SIZE.height / 2 - 10 - (WIN_SIZE.height / 2 - 10 - size.height / 2) / 2);
    }
    
    if (_find_node.findBtn) {
        auto bg = dynamic_cast<Sprite*>(_find_node.node->getChildByName("bg"));
        Size size = Size(542, 209);
        if (bg) {
            size = bg->getContentSize();
        }
        _find_node.findBtn->setPositionY((-size.height / 2 + (-WIN_SIZE.height / 2 + 50)) / 2);
    }

    return true;
}

void FindPwCompositeV66::initFindPwUI() {
    _find_node.accountEdit->setString("");
    _find_node.phoneEdit->setString("");
    _find_node.codeEdit->setString("");
}

void FindPwCompositeV66::closeClickCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        btn->setColor(Color3B(155, 155, 155));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        btn->setColor(Color3B(255, 255, 255));
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);

        onCloseCallBack();
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        btn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

//发送验证码
void FindPwCompositeV66::sendCodeCallback(Ref* pSender, Widget::TouchEventType type) {

    if (Widget::TouchEventType::ENDED != type) return;
    std::string phoneNumber = _find_node.phoneEdit->getString();
    if (phoneNumber.empty()) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }

    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
    std::string str("type=sendcode&mobile=");
    str += phoneNumber;

    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(FindPwCompositeV66::onHttpYanZhengResponse, this));

    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

//找回密码
void FindPwCompositeV66::sendFindCallback(Ref* pSender, Widget::TouchEventType type) {
    auto btn = (Button*)pSender;

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        btn->setColor(Color3B(155, 155, 155));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        btn->setColor(Color3B(255, 255, 255));
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

        std::string ass = _find_node.accountEdit->getString();
        std::string yanzhengma = _find_node.codeEdit->getString();
        std::string phoneNumber = _find_node.phoneEdit->getString();
        std::string str = "type=getpwd&mobile=";
        int userID = PlatformLogic()->loginResult.dwUserID;

        if (ass.empty()) {
            GamePromptLayer::create()->showPrompt(GetText("FIND_PASSWORLD_NO_EMPTY"));
            return;
        }

        if (yanzhengma.empty()) {
            GamePromptLayer::create()->showPrompt(GetText("FIND_PASSWORLD_YANZHEMGMA_NOEMPTY"));
            return;
        }

        if (phoneNumber.empty()) {
            GamePromptLayer::create()->showPrompt(GetText("FIND_PHONENUMBER_NO_EMPTY"));
            return;
        }

        if (phoneNumber.length() != 11) {
            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
            return;
        }


        std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
        str += phoneNumber;
        str += "&code=" + yanzhengma;
        str += "&username=" + ass;

        network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
        request->setUrl(URL.c_str());
        request->setRequestType(network::HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(FindPwCompositeV66::onHttpFindResponse, this));

        request->setRequestData(str.c_str(), str.size());
        network::HttpClient::getInstance()->sendImmediate(request);
        LoadingLayer::createLoading(this, gFontConfig_22, "", "");
        request->release();
        break;
    }
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        btn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

void FindPwCompositeV66::onHttpFindResponse(network::HttpClient* client, network::HttpResponse* response) {
    std::vector<char> recv = *response->getResponseData();
    std::string data;
    data.assign(recv.begin(), recv.end());

    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("status")) {
        return;
    }
    LoadingLayer::removeLoading(this);
    int result = doc["status"].GetInt();
    switch (result) {
    case 0: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_ERROR"));
        break;
    }
    case 1: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_PASSWORLD_NO_EMPTY"));
        break;
    }
    case 2: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_PHONENUMBER_NO_EMPTY"));
        break;
    }
    case 3: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_PLEASEWRITE_PHONENUMBER"));
        break;
    }
    case 4: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_SURE_YANZHENGMA_IS_OK"));
        break;
    }
    case 5: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_ASS_IS_NO"));
        break;
    }
    case 6: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_PHONENUMBER_NOBIND_PHONENUMBER"));
        break;
    }
    case 7: {
        GamePromptLayer::create()->showPrompt(GetText("FIND_PASSWORLD_OK"));
        break;
    }
    default:
        break;
    }
}


void FindPwCompositeV66::onHttpYanZhengResponse(network::HttpClient* client, network::HttpResponse* response) {
    std::vector<char> recv = *response->getResponseData();
    std::string data;
    data.assign(recv.begin(), recv.end());

    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("status")) {
        return;
    }

    int result = doc["status"].GetInt();
    if (result == 0) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_SEND_LOSER"));
    } else if (result == 1) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_SEND_OK"));
    }
}


void FindPwCompositeV66::editBoxEditingDidEnd(ui::EditBox* editBox) {

}

void FindPwCompositeV66::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {

}
