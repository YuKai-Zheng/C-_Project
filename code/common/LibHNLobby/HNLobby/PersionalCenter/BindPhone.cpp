#include "HNLobby/PersionalCenter/BindPhone.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include <string>
#include "network/HttpClient.h"
#include "HNLobby/globel.h"

static const char* BINDPHONE_JSON	= "csb/platform/bindPhone/bindPhoneNode.csb";

BindPhoneLayer::BindPhoneLayer() {
    memset(&_ui, 0x0, sizeof(_ui));
}

BindPhoneLayer::~BindPhoneLayer() {

}

void BindPhoneLayer::show(Node* parent, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    parent->addChild(this, zorder, tag);
}

void BindPhoneLayer::close() {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    this->removeFromParent();
    //_ui.bindLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f),  CCCallFunc::create([&]()
    //{
    //}), nullptr));
}

bool BindPhoneLayer::init() {
    if ( !HNLayer::init()) return false;
    Size winSize = Director::getInstance()->getWinSize();

    LogonResult = PlatformLogic()->loginResult;

    //屏蔽后面的层
    //auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    //addChild(colorLayer,1);

    //auto touchListener = EventListenerTouchOneByOne::create();
    //touchListener->onTouchBegan = [=](Touch* touch ,Event* event){
    //	return true;
    //};
    //touchListener->setSwallowTouches(true);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,colorLayer);

    // 玩家资料框背景
    _ui.bindLoader = CSLoader::createNode(BINDPHONE_JSON);
    //_ui.bindLoader->setScale( 0 );
    //_ui.bindLoader->runAction(ScaleTo::create(0.2f, 1.0f));
    _ui.bindLoader->setPosition(Vec2(0,0));
    addChild(_ui.bindLoader, 2);
    auto panel = (Layout*)_ui.bindLoader->getChildByName("Panel_1");
    _backGroundSize = panel->getContentSize();
    _ui.layout = (ImageView*)panel->getChildByName("Image_1");

    //关闭按钮
    _ui.Button_Cancel = (Button*)panel->getChildByName("Button_close");
    _ui.Button_Cancel->addTouchEventListener(CC_CALLBACK_2(BindPhoneLayer::closeCallBack, this));
    //确定按钮
    _ui.Button_OK = (Button*)panel->getChildByName("Button_sure");
    _ui.Button_OK ->addTouchEventListener(CC_CALLBACK_2(BindPhoneLayer::okEventCallBack, this));

    _ui.Sprite_Titile = (Sprite*)_ui.bindLoader->getChildByName("redOk_1");
    _ui.TextField_PhoneNumberBg = (Sprite*)panel->getChildByName("bg_3");

    _ui.Text_PhoneNumber = (Text*)panel->getChildByName("TextNumber");

    //获取验证码
    _ui.Button_Code = (Button*)panel->getChildByName("Button_getCode");
    _ui.Button_Code ->addTouchEventListener(CC_CALLBACK_2(BindPhoneLayer::verifyCodeUIEventCallBack, this));

    //手机号输入框
    auto PhoneNumber = (TextField*)panel->getChildByName("TextField_phone");
    PhoneNumber->setVisible(false);
    _ui.TextField_PhoneNumber= HNEditBox::createEditBox(PhoneNumber, this);



    //验证码输入框
    auto code = (TextField*)panel->getChildByName("TextField_code");
    code->setVisible(false);
    _ui.TextField_Code = HNEditBox::createEditBox(code, this);
    CCLOG(" Phone number = %s", LogonResult.szMobileNo);

    if (LogonResult.iBindMobile == 0) {
        _ui.Sprite_Titile->setTexture("button/bindOk.png");
        _ui.TextField_PhoneNumber->setVisible(true);
        _ui.TextField_PhoneNumberBg->setVisible(true);
    } else if(LogonResult.iBindMobile == 1) {
        _ui.Sprite_Titile->setTexture("button/bindNo.png");
        _ui.TextField_PhoneNumber->setVisible(false);
        _ui.TextField_PhoneNumberBg->setVisible(false);
        _ui.Text_PhoneNumber->setVisible(true);
        _ui.Text_PhoneNumber->setString(ForceGBKToUtf8(LogonResult.szMobileNo));
    }

    return true;
}

void BindPhoneLayer::okEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;

    std::string phoneNumber;
    std::string verifyCode = _ui.TextField_Code->getString();
    std::string str;
    if (!LogonResult.iBindMobile) {
        phoneNumber = _ui.TextField_PhoneNumber->getString();
        str = "type=verifycode&mobile=";
    } else {
        phoneNumber = _ui.Text_PhoneNumber->getString();
        str = "type=clearbind&mobile=";
    }

    int userID = PlatformLogic()->loginResult.dwUserID;

    if (phoneNumber.empty()) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }

    if (verifyCode.empty()) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        return;
    }

    if (phoneNumber.length() != 11) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        return;
    }


    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
    str += phoneNumber;
    str += "&code=" + verifyCode;
    str += "&userid=" + to_string(userID);

    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(BindPhoneLayer::onHttpBindResponse, this));

    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

void BindPhoneLayer::verifyCodeUIEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type) return;
    std::string phoneNumber;
    if (!LogonResult.iBindMobile) {
        phoneNumber = _ui.TextField_PhoneNumber->getString();
    } else {
        phoneNumber = _ui.Text_PhoneNumber->getString();
    }


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
    request->setResponseCallback(CC_CALLBACK_2(BindPhoneLayer::onHttpCodeResponse, this));

    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

void BindPhoneLayer::onHttpCodeResponse(network::HttpClient* client, network::HttpResponse* response) {
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

void BindPhoneLayer::onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response) {
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
        if (!LogonResult.iBindMobile) {
            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BING_OK"));
        } else {
            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BINGJIE_LOSER"));
        }
    } else if (result == 1) {
        LogonResult.iBindMobile = !LogonResult.iBindMobile;
        PlatformLogic()->loginResult.iBindMobile = LogonResult.iBindMobile;
        if (!LogonResult.iBindMobile) {
            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BINGJIE_OK"));
            strcpy(LogonResult.szMobileNo, _ui.TextField_PhoneNumber->getText());
            //strcpy(PlatformLogic()->loginResult.szMobileNo, Utf8ToGB(_ui.TextField_PhoneNumber->getText()));

            _ui.Sprite_Titile->setTexture("button/bindOk.png");
            _ui.TextField_PhoneNumber->setVisible(true);
            _ui.TextField_PhoneNumberBg->setVisible(true);
            if (_ui.Text_PhoneNumber)
                _ui.Text_PhoneNumber->setVisible(false);
        } else {
            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BING_OK"));
            strcpy(LogonResult.szMobileNo, "");
            _ui.Sprite_Titile->setTexture("button/bindNo.png");

            _ui.TextField_PhoneNumber->setVisible(false);
            _ui.TextField_PhoneNumberBg->setVisible(false);
            strcpy(LogonResult.szMobileNo, _ui.TextField_PhoneNumber->getText());
            strcpy(PlatformLogic()->loginResult.szMobileNo, Utf8ToGB(_ui.TextField_PhoneNumber->getText()));

            _ui.Text_PhoneNumber->setString(_ui.TextField_PhoneNumber->getText());
            _ui.Text_PhoneNumber->setVisible(true);
        }
        _ui.TextField_Code->setString("");

    } else if (result == 2) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BING_REPORT"));
    } else if (result == 3) {
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_VERIFICATION_LOSER"));
    }
}

bool BindPhoneLayer::verifyCodeSelector(HNSocketMessage* socketMessage) {
    CCAssert(sizeof (MSG_GP_SmsVCode) == socketMessage->objectSize, "MSG_GP_SmsVCode is error.");
    if (socketMessage->objectSize != sizeof(MSG_GP_SmsVCode)) return true;

    MSG_GP_SmsVCode *smsVCode = (MSG_GP_SmsVCode *)socketMessage->object;

    _verifyCode = smsVCode->szVCode;

    return true;
}

bool BindPhoneLayer::bindPhoneSelector(HNSocketMessage* socketMessage) {
    CCAssert(sizeof (MSG_GP_R_BindMobile) == socketMessage->objectSize, "MSG_GP_R_BindMobile is error.");
    if (socketMessage->objectSize != sizeof(MSG_GP_R_BindMobile)) return true;

    MSG_GP_R_BindMobile *bindMobile = (MSG_GP_R_BindMobile *)socketMessage->object;

    GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_VERIFICATION_BOUNF_OK"));

    MSG_GP_UserInfo userInfo = PlatformLogic()->loginResult;
    std::string phoneNumber = _ui.TextField_PhoneNumber->getString();
    strcpy(userInfo.szMobileNo, phoneNumber.c_str());
    PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_DETAIL, &userInfo, sizeof(userInfo));

    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
    PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT, HN_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
    return true;
}

bool BindPhoneLayer::modifyUserInfoSelector(HNSocketMessage* socketMessage) {
    if (ASS_GP_USERINFO_ACCEPT == socketMessage->messageHead.bAssistantID) {	// 服务端已接受
        std::string phoneNumber = _ui.TextField_PhoneNumber->getString();
        strcpy(PlatformLogic()->loginResult.szMobileNo, phoneNumber.c_str());

        UserInfoStruct* userinfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
        if (nullptr != userinfo) {
            strcpy(userinfo->szPhoneNum, phoneNumber.c_str());
            UserInfoModule()->updateUser(userinfo);
        }
    } else if (ASS_GP_USERINFO_NOTACCEPT == socketMessage->messageHead.bAssistantID) {	// 服务端未能接受

    } else {

    }
    return true;
}

void BindPhoneLayer::closeCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        close();
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}