//
//  FindPwCompositeV2.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "FindPwCompositeV2.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformDefine.h"
#include "VersionInfo/VersionInfo.h"
#include "../FontConfig.h"

static const char* FIND_NODE_CSB = "csb/platform/loginUI/findPwNode.csb";

FindPwCompositeV2::FindPwCompositeV2() {
    memset(&_find_node, 0x0, sizeof(_find_node));
}

FindPwCompositeV2::~FindPwCompositeV2() {
    
}

bool FindPwCompositeV2::init() {
    if (!HNLayer::init()) return false;
    
    //Size winSize = Director::getInstance()->getWinSize();
    
    //加载csb
    _find_node.node = CSLoader::createNode(FIND_NODE_CSB);
    //auto pos = GD_NEED_OPEN != true ? Vec2(winSize.width*0.75f, winSize.height*0.5f) : Vec2(winSize / 2);
    //_find_node.node->setPosition(pos);
    addChild(_find_node.node);
    
    //返回按钮
    _find_node.backBtn = (Button*)_find_node.node->getChildByName("Button_back");
    _find_node.backBtn->setShowEffect(false);
    _find_node.backBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV2::closeClickCallback, this));
    
    //账号输入
    auto accountText = (TextField*)_find_node.node->getChildByName("TextField_account");
    accountText->setVisible(false);
    _find_node.accountEdit = HNEditBox::createEditBox(accountText, this);
    
    //手机号输入
    auto phoneText = (TextField*)_find_node.node->getChildByName("TextField_phone");
    phoneText->setVisible(false);
    _find_node.phoneEdit = HNEditBox::createEditBox(phoneText, this);
    
    //验证码输入
    auto codeText = (TextField*)_find_node.node->getChildByName("TextField_code");
    codeText->setVisible(false);
    _find_node.codeEdit = HNEditBox::createEditBox(codeText, this);
    
    //发送验证码按钮
    _find_node.sendCodeBtn = (Button*)_find_node.node->getChildByName("Button_sendCode");
    _find_node.sendCodeBtn->setShowEffect(false);
    _find_node.sendCodeBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV2::sendCodeCallback, this));
    
    //find按钮
    auto btn = (Button*)_find_node.node->getChildByName("Button_find");
    btn->setVisible(false);
    
    _find_node.findBtn = AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
    _find_node.findBtn->setName("Button_sendCode");
    _find_node.findBtn->setTitle("button/redFind.png");
    _find_node.findBtn->setTitleOffset(Vec2(-4, 4));
    _find_node.findBtn->setPosition(btn->getPosition());
    _find_node.findBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV2::sendFindCallback, this));
    _find_node.node->addChild(_find_node.findBtn);
    
    return true;
}

void FindPwCompositeV2::initFindPwUI() {
    _find_node.accountEdit->setString("");
    _find_node.phoneEdit->setString("");
    _find_node.codeEdit->setString("");
}

void FindPwCompositeV2::closeClickCallback(Ref* pSender, Widget::TouchEventType type) {
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
void FindPwCompositeV2::sendCodeCallback(Ref* pSender, Widget::TouchEventType type) {
    
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
    request->setResponseCallback(CC_CALLBACK_2(FindPwCompositeV2::onHttpYanZhengResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
    
    request->release();
}

//找回密码
void FindPwCompositeV2::sendFindCallback(Ref* pSender, Widget::TouchEventType type) {
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
            request->setResponseCallback(CC_CALLBACK_2(FindPwCompositeV2::onHttpFindResponse, this));
            
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

void FindPwCompositeV2::onHttpFindResponse(network::HttpClient* client, network::HttpResponse* response) {
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


void FindPwCompositeV2::onHttpYanZhengResponse(network::HttpClient* client, network::HttpResponse* response) {
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


void FindPwCompositeV2::editBoxEditingDidEnd(ui::EditBox* editBox) {
    
}

void FindPwCompositeV2::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {
    
}
