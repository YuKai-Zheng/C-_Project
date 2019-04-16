//
//  NoticeLayerV66.cpp
//  LibHNLobby
//
//  Created by mac on 2018/8/7.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "NoticeLayerV66.hpp"
#include "json/document.h"
#include "json/rapidjson.h"
NoticeLayer::NoticeLayer()
{
    _labelMessage = nullptr;
    promptText    = nullptr;
    BG            = nullptr;
}
NoticeLayer::~NoticeLayer()
{
    
    
}
bool NoticeLayer::init()
{
    if (!HNLayer::init())
        return false;
    
    //屏蔽后面的层
    srand(unsigned(time(0)));
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    addChild(colorLayer);
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
       
        return true;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        
        
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
    
    _labelMessage = Label::createWithTTF("", DEFAULT_FONT, 30);
    _labelMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelMessage->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60);
    _labelMessage->setString("获取公告中");
    
    this->addChild(_labelMessage);
    
    BG = ImageView::create("hallimg/main/notice.png");
    if (BG) {
        BG->setPosition(WIN_SIZE/2);
        this->addChild(BG);
        
        auto bgSize = BG->getContentSize();
        promptText = Text::create("", "platform/fonts/simhei.ttf", 32);
        if (promptText) {
            promptText->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.5f));
            promptText->setColor(Color3B(255, 234, 179));
            promptText->setContentSize(Size(bgSize.width * 0.9f, bgSize.height * 0.5f));
            promptText->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
            promptText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            promptText->ignoreContentAdaptWithSize(false);
            BG->addChild(promptText);
        }
        
        auto closeButton = Button::create("v66/common/16.png", "v66/common/16.png");
        if (closeButton) {
            closeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            closeButton->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.85f));
            closeButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                if (type != Widget::TouchEventType::ENDED) {
                    return;
                }
                close();
                
            });
          
            BG->addChild(closeButton);
        }
        
        BG->setVisible(false);
    }
    
    
    //std::string imgHeadUrl = "http://sh1.game5186.net/notice.aspx";
	std::string imgHeadUrl = "testsh1.game5186.net";
    
    cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
    request->setUrl(imgHeadUrl);
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(NoticeLayer::onHttpUpdate,this));
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
    
    return true;
}
void NoticeLayer::showNotice(const std::string& prompt)
{
    if (_labelMessage) {
        _labelMessage->setVisible(false);
    }
    if (BG) {
        BG->setVisible(true);
        BG->setScale(0.1f);
        BG->runAction(ScaleTo::create(0.2f,1.0f));
        if (promptText) {
            promptText->setString(prompt);
        }
    }
}
void NoticeLayer::close()
{
    this->removeFromParent();
}

void NoticeLayer::onHttpUpdate(cocos2d::network::HttpClient *sender,
                           cocos2d::network::HttpResponse *response) {
    std::string tag = response->getHttpRequest()->getTag();
    
    if (!response->isSucceed()) {
        close();
        return;
    }
    
    int m_code = 0;
    std::string m_message;

    std::vector<char> *respData = response->getResponseData();
    std::string json;
    json.assign(respData->begin(), respData->end());
    
        
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
        if (doc.HasParseError() || !doc.IsObject()) {
            close();
            return;
        }
        do {
            if (doc.HasMember("code")) {
                m_code = doc["code"].GetInt();
            }
            if (doc.HasMember("msg")) {
                m_message = doc["msg"].GetString();
            }
            
       
        } while (0);
    if (m_code == 0) {
        close();
    }
    else
    {
        if (!m_message.empty()) {
            showNotice(m_message);
        }
        
    }
        
}
