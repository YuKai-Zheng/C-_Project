//
//  BindPhone.cpp
//  LibHNLobby
//
//  Created by cocos on 2018/3/4.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "BindPhone.hpp"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "UI/HNEditBox.h"
#include "UI/Base/AUIButton.h"
#include "PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "../pattern/CommandManager.h"
#include "../GamePrompt.h"
#include "HNPlatform/HNPlatformLogic.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace HN;

const char* SET_BUTTON_UNBIND_TITLE = "v66/button/yellow1/11.png";
const char* SET_BUTTON_BIND_TITLE = "v66/button/yellow1/11.png";

bool BindPhoneGui::init()
{
    _url = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
    
    _opVec.clear();
    _opVec.push_back("sendcode");
    _opVec.push_back("verifycode");
    _opVec.push_back("clearbind");
    _opVec.push_back("changebind");
    
    
    
    
    return true;
}

void BindPhoneGui::onEnter()
{   Layer::onEnter();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(BindPhoneGui::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}
bool BindPhoneGui::onTouchBegan(Touch * touch, Event *unused_event)
{
    
    return true;
    
}

void BindPhoneGui::onExit()
{
    _eventDispatcher->removeEventListener(listener);
    Layer::onExit();
}



void BindPhoneGui::Open(cocos2d::Node *parent, const std::string& title,
                        cocos2d::Vec2 vec, int zorder, int tag)
{
    auto csb = CSLoader::createNode("csb/v66/userData/bindPhone.csb");
    if (nullptr == csb)
    {
        return;
    }
    
    parent->addChild(this, zorder);
    
    this->addChild(csb, zorder);
    this->setTag(tag);
    csb->setPosition(vec);
    csb->setName("bindPhoneNode");
    
    auto txtTitle = (Text*)csb->getChildByName("txtTitle");
    if (nullptr != txtTitle)
    {
        txtTitle->setString(title);
    }
    
    auto phoneTemp = (TextField*)csb->getChildByName("textField_phone");
    phoneTemp->setColor(FONT_CLOLOR_YELLOW_V66);
    phoneTemp->setVisible(false);
    phone = HNEditBox::createEditBox(phoneTemp, this);
    phone->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    
    auto vertifyCodeTemp = (TextField*)csb->getChildByName("textField_code");
    vertifyCodeTemp->setColor(FONT_CLOLOR_YELLOW_V66);
    vertifyCodeTemp->setVisible(false);
    vertifyCode = HNEditBox::createEditBox(vertifyCodeTemp, this);
    vertifyCode->setInput(HNEditBox::InputMode::PHONE_NUMBER);
    
    auto tempGetBtn = (Button*)csb->getChildByName("Button_getCode");
    tempGetBtn->setVisible(false);
    auto getBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
    getBtn->setTitle("v66/button/yellow1/yanzheng.png");
    getBtn->setPosition(tempGetBtn->getPosition());
    csb->addChild(getBtn);
    getBtn->addTouchEventListener(CC_CALLBACK_2(BindPhoneGui::handleGetBtn, this));
    
    auto tempSendBtn = (Button*)csb->getChildByName("Button_bind");
    tempSendBtn->setVisible(false);
    sendBtn = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
    sendBtn->setPosition(tempSendBtn->getPosition());
    csb->addChild(sendBtn);
    sendBtn->addTouchEventListener(CC_CALLBACK_2(BindPhoneGui::handleSendBtn, this));
    
    auto btnClose = (Button*)csb->getChildByName("btnClose");
    btnClose->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
        if (type != Widget::TouchEventType::ENDED) return;
        Close();
    });
    
    _loginResult = PlatformLogic()->loginResult;
    updateBindInfo(_loginResult.iBindMobile, true);
}

void BindPhoneGui::Close()
{
    removeFromParent();
}

void BindPhoneGui::SetOpType(BindPhoneOpType type, bool bHasLogin, int id, const std::string& mobileNum)
{
    _type = type;
    _bHasLogin = bHasLogin;
    _id = id;
    _mobileNum = mobileNum;
}

void BindPhoneGui::handleGetBtn(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
    return;
    
//    std::string phoneNumber = phone->getText();
    std::string phoneNumber = phone->getText();
    if (phoneNumber.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }
    
    _type = BindPhoneOpType::SendCode;
    std::string str; //("type=sendcode&mobile=");
    str = "type=" + _opVec.at(_type) + "&mobile=" + phoneNumber;
//    str += phoneNumber;
    
    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(_url.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(BindPhoneGui::onHttpCodeResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
}

void BindPhoneGui::onHttpCodeResponse(HttpClient* client, HttpResponse* response)
{
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
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_SEND_LOSER"));
    }
    else if (result == 1) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_SEND_OK"));
    }
}

void BindPhoneGui::updateBindInfo(bool isBind, bool isSuccess)
{
    vertifyCode->setText("");

    if (isBind && isSuccess)
    {
        sendBtn->removeChildByTag(99);
        sendBtn->setTitle(SET_BUTTON_UNBIND_TITLE);
        
        phone->setString(ForceGBKToUtf8(_loginResult.szMobileNo));
        phone->setTouchEnabled(false);

    }
    
    if (!isBind && isSuccess)
    {
        sendBtn->removeChildByTag(99);
        sendBtn->setTitle(SET_BUTTON_BIND_TITLE);
        
        phone->setTouchEnabled(true);
    }

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("setBindStatus");
}

void BindPhoneGui::handleSendBtn(Ref* pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type)
    return;
    
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    
    std::string phoneNumber = phone->getString();
    std::string verifyCode = vertifyCode->getString();
    std::string str;
    
    if (_bHasLogin)
    {
        if (!_loginResult.iBindMobile) {
            //        str = "type=verifycode";
            _type = BindPhoneOpType::VerifyCode;
        }
        else {
            //        str = "type=clearbind";
            _type = BindPhoneOpType::ClearBind;
        }
    }
    else
    {
        _type = BindPhoneOpType::ChangeBind;
    }
    
    
    char UserID[32] ={0};
    int userID = _loginResult.dwUserID;
    sprintf(UserID, "%d", userID);
    
    if (phoneNumber.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
        return;
    }
    
    if (verifyCode.empty()) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
        return;
    }
    
    if (phoneNumber.length() != 11) {
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
        return;
    }
    
    str = "type=" + _opVec.at(_type);
    str += "&mobile=" + phoneNumber;
    str += "&code=" + verifyCode;
    str += string("&userid=") + string(UserID);
    
    if (BindPhoneOpType::ChangeBind == _type)
    {
        str += "&udid=" + UserDefault::getInstance()->getStringForKey("UUID");
    }
    
    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(_url.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(BindPhoneGui::onHttpBindResponse, this));
    
    request->setRequestData(str.c_str(), str.size());
    network::HttpClient::getInstance()->sendImmediate(request);
    
    request->release();
}

void BindPhoneGui::onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response) {
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
        if (!_loginResult.iBindMobile) {//绑定成功
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BING_OK"));
            updateBindInfo(true, true);
            Close();
        }
        else {//解绑失败
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BINGJIE_LOSER"));
            updateBindInfo(false, false);
        }
    }
    else if (result == 1) {
        _loginResult.iBindMobile = !_loginResult.iBindMobile;
        PlatformLogic()->loginResult.iBindMobile = _loginResult.iBindMobile;
        if (!_bHasLogin)
        {
            CommandManager::getInstance()->showGamePrompt("验证成功！请重新登陆。");
            strcpy(_loginResult.szMobileNo, phone->getText());
            updateBindInfo(false, true);
        } else if (!_loginResult.iBindMobile) {//解绑成功
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BINGJIE_OK"));
            strcpy(_loginResult.szMobileNo, phone->getText());
            updateBindInfo(false, true);
            Close();
        }
        else {//绑定成功
            CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BING_OK"));
            strcpy(_loginResult.szMobileNo, "");
            strcpy(_loginResult.szMobileNo, phone->getText());
            strcpy(PlatformLogic()->loginResult.szMobileNo, Utf8ToGB(phone->getText()));
            updateBindInfo(true, true);
            Close();
        }
        
    }
    else if (result == 2) {//重复绑定
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_BING_REPORT"));
    }
    else if (result == 3) {//验证码验证失败
        CommandManager::getInstance()->showGamePrompt(GetText("BINDPHONE_VERIFICATION_LOSER"));
    }
    
    if (BindPhoneOpType::ChangeBind == _type)
    {
        // 更新绑定！
        auto promp = GamePromptLayer::create();
        promp->showPrompt("绑定更新成功。请重新登陆！");
        Close();
    }
}


void BindPhoneGui::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox) {
//    if (editBox->getName().compare("nickname") == 0) {
//        m_canClose = false;
//    }
}

void BindPhoneGui::editBoxReturn(cocos2d::ui::EditBox* editBox) {
//    if (editBox->getName().compare("nickname") == 0) {
//        isNickSave = true;
//        modifyNickname(editBox->getText());
//    }
    //    m_canClose = true;
}
