//
//  PopularizeOfMy.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "PopularizeOfMy.h"
#include "PopularizeSignSecret.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "Tool/Tools.h"
#include "../pattern/CommandManager.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/WechatMgr.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "base/WeChatAndroid.h"
#endif

PopularizeOfMy::PopularizeOfMy() {
    
}

PopularizeOfMy::~PopularizeOfMy() {
    
}

bool PopularizeOfMy::init() {
    if (!Layer::init()) {
        return false;
    }
    
    resetData();
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(colorLayer);
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        return true;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        closeLayer();
    };
    
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
    
    this->setContentSize(WIN_SIZE);
    this->setIgnoreAnchorPointForPosition(false);
    
    createUI();
    
    requestData();
    
    return true;
}

void PopularizeOfMy::openAddToParent(Layer *parent, cocos2d::Vec2 pos) {
    if (parent) {
        this->setPosition(pos);
        parent->addChild(this);
    }
}

void PopularizeOfMy::resetData() {
    m_bg = nullptr;
    touchListener = nullptr;
    m_shareButton = nullptr;
    m_secretButton = nullptr;
    m_profitTotal = nullptr;
    m_profitToday = nullptr;
    m_profitWeek = nullptr;
    m_profitMonth = nullptr;
    m_peopleLevelTotal = nullptr;
    m_peopleLevelOne = nullptr;
    m_peopleLevelTWO = nullptr;
    m_peopleLevelThree = nullptr;
    m_peopleIncreaseLevelOne = nullptr;
    m_peopleIncreaseLevelTWO = nullptr;
    m_peopleIncreaseLevelThree = nullptr;
    m_profitIncreaseLevelOne = nullptr;
    m_profitIncreaseLevelTWO = nullptr;
    m_profitIncreaseLevelThree = nullptr;
}

void PopularizeOfMy::createUI() {
    m_bg = ImageView::create("v66/module/tuiguangfuli/wodetuiguang.png");
    if (m_bg) {
        m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_bg->setPosition(WIN_SIZE/2);
        m_bg->setTouchEnabled(true);
        m_bg->setSwallowTouches(true);
        addChild(m_bg);
        //
        m_shareButton = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG_1.png");
        if (m_shareButton) {
            m_shareButton->setTitle("v66/button/yellow1/tuiguangfenxiang.png");
            m_shareButton->setTitleOffset(Vec2(-4, 4));
            m_shareButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.07f));
            m_shareButton->addTouchEventListener(CC_CALLBACK_2(PopularizeOfMy::shareButtonCallBack, this));
            m_bg->addChild(m_shareButton);
        }
        m_secretButton = Button::create("v66/module/tuiguangfuli/tuiguangmiji1.png", "v66/module/tuiguangfuli/tuiguangmiji1.png");
        if (m_secretButton) {
            m_secretButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            m_secretButton->setPosition(Vec2(m_bg->getContentSize().width-20,15));
            m_secretButton->addTouchEventListener(CC_CALLBACK_2(PopularizeOfMy::secretButtonCallBack, this));
            m_bg->addChild(m_secretButton);
        }
        //总人数
        m_peopleLevelTotal = Text::create("0", FONT_TTF_FZCY, 26);
        if (m_peopleLevelTotal) {
            m_peopleLevelTotal->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleLevelTotal->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleLevelTotal->setTextColor(Color4B::WHITE);
            m_peopleLevelTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleLevelTotal->setPosition(Vec2(m_bg->getContentSize().width*0.39f, m_bg->getContentSize().height*0.83f));
            m_bg->addChild(m_peopleLevelTotal);
        }
        //总收益
        m_profitTotal = Text::create("0", FONT_TTF_FZCY, 26);
        if (m_profitTotal) {
            m_profitTotal->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitTotal->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitTotal->setTextColor(Color4B(255, 251, 134, 255));
            m_profitTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitTotal->setPosition(Vec2(m_bg->getContentSize().width*0.665f, m_bg->getContentSize().height*0.83f));
            m_bg->addChild(m_profitTotal);
        }
        //三级人数
        m_peopleLevelOne = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_peopleLevelOne) {
            m_peopleLevelOne->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleLevelOne->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleLevelOne->setTextColor(Color4B::WHITE);
            m_peopleLevelOne->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleLevelOne->setPosition(Vec2(m_bg->getContentSize().width*0.19f, m_bg->getContentSize().height*0.75f));
            m_bg->addChild(m_peopleLevelOne);
        }
        m_peopleLevelTWO = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_peopleLevelTWO) {
            m_peopleLevelTWO->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleLevelTWO->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleLevelTWO->setTextColor(Color4B::WHITE);
            m_peopleLevelTWO->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleLevelTWO->setPosition(Vec2(m_bg->getContentSize().width*0.19f, m_bg->getContentSize().height*0.64f));
            m_bg->addChild(m_peopleLevelTWO);
        }
        m_peopleLevelThree = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_peopleLevelThree) {
            m_peopleLevelThree->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleLevelThree->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleLevelThree->setTextColor(Color4B::WHITE);
            m_peopleLevelThree->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleLevelThree->setPosition(Vec2(m_bg->getContentSize().width*0.19f, m_bg->getContentSize().height*0.53f));
            m_bg->addChild(m_peopleLevelThree);
        }
        //三级人数增长
        m_peopleIncreaseLevelOne = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_peopleIncreaseLevelOne) {
            m_peopleIncreaseLevelOne->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleIncreaseLevelOne->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleIncreaseLevelOne->setTextColor(Color4B(156, 247, 104, 255));
            m_peopleIncreaseLevelOne->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleIncreaseLevelOne->setPosition(Vec2(m_bg->getContentSize().width*0.54f, m_bg->getContentSize().height*0.75f));
            m_bg->addChild(m_peopleIncreaseLevelOne);
        }
        m_peopleIncreaseLevelTWO = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_peopleIncreaseLevelTWO) {
            m_peopleIncreaseLevelTWO->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleIncreaseLevelTWO->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleIncreaseLevelTWO->setTextColor(Color4B(156, 247, 104, 255));
            m_peopleIncreaseLevelTWO->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleIncreaseLevelTWO->setPosition(Vec2(m_bg->getContentSize().width*0.54f, m_bg->getContentSize().height*0.64f));
            m_bg->addChild(m_peopleIncreaseLevelTWO);
        }
        m_peopleIncreaseLevelThree = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_peopleIncreaseLevelThree) {
            m_peopleIncreaseLevelThree->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_peopleIncreaseLevelThree->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_peopleIncreaseLevelThree->setTextColor(Color4B(156, 247, 104, 255));
            m_peopleIncreaseLevelThree->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_peopleIncreaseLevelThree->setPosition(Vec2(m_bg->getContentSize().width*0.54f, m_bg->getContentSize().height*0.53f));
            m_bg->addChild(m_peopleIncreaseLevelThree);
        }
        //三级人数收益
        m_profitIncreaseLevelOne = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_profitIncreaseLevelOne) {
            m_profitIncreaseLevelOne->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitIncreaseLevelOne->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitIncreaseLevelOne->setTextColor(Color4B(255, 251, 134, 255));
            m_profitIncreaseLevelOne->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitIncreaseLevelOne->setPosition(Vec2(m_bg->getContentSize().width*0.83f, m_bg->getContentSize().height*0.74f));
            m_bg->addChild(m_profitIncreaseLevelOne);
        }
        m_profitIncreaseLevelTWO = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_profitIncreaseLevelTWO) {
            m_profitIncreaseLevelTWO->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitIncreaseLevelTWO->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitIncreaseLevelTWO->setTextColor(Color4B(255, 251, 134, 255));
            m_profitIncreaseLevelTWO->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitIncreaseLevelTWO->setPosition(Vec2(m_bg->getContentSize().width*0.83f, m_bg->getContentSize().height*0.63f));
            m_bg->addChild(m_profitIncreaseLevelTWO);
        }
        m_profitIncreaseLevelThree = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_profitIncreaseLevelThree) {
            m_profitIncreaseLevelThree->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitIncreaseLevelThree->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitIncreaseLevelThree->setTextColor(Color4B(255, 251, 134, 255));
            m_profitIncreaseLevelThree->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitIncreaseLevelThree->setPosition(Vec2(m_bg->getContentSize().width*0.83f, m_bg->getContentSize().height*0.52f));
            m_bg->addChild(m_profitIncreaseLevelThree);
        }
        //分阶段收益
        m_profitToday = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_profitToday) {
            m_profitToday->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitToday->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitToday->setTextColor(Color4B(255, 251, 134, 255));
            m_profitToday->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitToday->setPosition(Vec2(m_bg->getContentSize().width*0.19f, m_bg->getContentSize().height*0.41f));
            m_bg->addChild(m_profitToday);
        }
        m_profitWeek = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_profitWeek) {
            m_profitWeek->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitWeek->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitWeek->setTextColor(Color4B(255, 251, 134, 255));
            m_profitWeek->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitWeek->setPosition(Vec2(m_bg->getContentSize().width*0.19f, m_bg->getContentSize().height*0.3f));
            m_bg->addChild(m_profitWeek);
        }
        m_profitMonth = Text::create("0", FONT_TTF_FZCY, 30);
        if (m_profitMonth) {
            m_profitMonth->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_profitMonth->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_profitMonth->setTextColor(Color4B(255, 251, 134, 255));
            m_profitMonth->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            m_profitMonth->setPosition(Vec2(m_bg->getContentSize().width*0.19f, m_bg->getContentSize().height*0.19f));
            m_bg->addChild(m_profitMonth);
        }

    }

}

void PopularizeOfMy::closeLayer() {
//    this->removeFromParent();
    if (this->getParent()) {
        this->getParent()->removeFromParent();
    }
}

void PopularizeOfMy::shareButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    createSelectShareTypeLayer();
//    closeLayer();
}

void PopularizeOfMy::secretButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    auto secretLayer = PopularizeSignSecret::create();
    if (secretLayer) {
        secretLayer->openAddToParent(this, WIN_SIZE/2);
    }
}

void PopularizeOfMy::requestData() {
    MSG_GP_Get_Select_TuiGuang_User request;
    request.iUserID = PlatformLogic()->loginResult.dwUserID;
    
    PlatformLogic()->sendData(MDM_GP_SELECT_TUIGUANG_USER, ASS_GP_SELECT_TUIGUANG_USER, &request, sizeof(MSG_GP_Get_Select_TuiGuang_User), HN_SOCKET_CALLBACK(PopularizeOfMy::myDataSelector, this));
}

bool PopularizeOfMy::myDataSelector(HN::HNSocketMessage *socketMessage) {
    CCAssert(sizeof(MSG_GP_Select_TuiGuang_User) == socketMessage->objectSize, "sizeof(MSG_GP_Select_TuiGuang_User) != objectSize");
    if (sizeof(MSG_GP_Select_TuiGuang_User) != socketMessage->objectSize)    return false;
    MSG_GP_Select_TuiGuang_User* msg = (MSG_GP_Select_TuiGuang_User*)socketMessage->object;
    if (msg) {
        char str[64];
        if (m_peopleLevelTotal) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->totalSum).c_str());
            m_peopleLevelTotal->setString(str);
        }
        if (m_profitTotal) {
            m_profitTotal->setString(Tools::addComma(msg->totalProfit).c_str());
        }
        if (m_peopleLevelOne) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->oneSum).c_str());
            m_peopleLevelOne->setString(str);
        }
        if (m_peopleLevelTWO) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->twoSum).c_str());
            m_peopleLevelTWO->setString(str);
        }
        if (m_peopleLevelThree) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->threeSum).c_str());
            m_peopleLevelThree->setString(str);
        }
        if (m_profitToday) {
            m_profitToday->setString(Tools::addComma(msg->dayProfit).c_str());
        }
        if (m_profitWeek) {
            m_profitWeek->setString(Tools::addComma(msg->weekProfit).c_str());
        }
        if (m_profitMonth) {
            m_profitMonth->setString(Tools::addComma(msg->monthProfit).c_str());
        }
        if (m_peopleIncreaseLevelOne) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->oneSumAdd).c_str());
            m_peopleIncreaseLevelOne->setString(str);
        }
        if (m_peopleIncreaseLevelTWO) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->twoSumAdd).c_str());
            m_peopleIncreaseLevelTWO->setString(str);
        }
        if (m_peopleIncreaseLevelThree) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("PEOPLE_COUNT"), Tools::addComma(msg->threeAdd).c_str());
            m_peopleIncreaseLevelThree->setString(str);
        }
        if (m_profitIncreaseLevelOne) {
            m_profitIncreaseLevelOne->setString(Tools::addComma(msg->oneProfit).c_str());
        }
        if (m_profitIncreaseLevelTWO) {
            m_profitIncreaseLevelTWO->setString(Tools::addComma(msg->twoProfit).c_str());
        }
        if (m_profitIncreaseLevelThree) {
            m_profitIncreaseLevelThree->setString(Tools::addComma(msg->threeProfit).c_str());
        }
    }
    
    return true;
}

void PopularizeOfMy::createSelectShareTypeLayer() {
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    colorLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(colorLayer);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        return true;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        colorLayer->removeFromParent();
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
    
    auto bg = ImageView::create("v88/common/tanchuang/bg.png");
    bg->setPosition(WIN_SIZE / 2);
    bg->setTouchEnabled(true);
    colorLayer->addChild(bg);
    bg->setScale(0.5f);
    
    auto bgSize = bg->getContentSize();
    
    auto title = ImageView::create("v88/module/fenxiang/0.png");
    title->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 44));
    bg->addChild(title);
    
    auto friendCircleBtn = Button::create("v88/module/fenxiang/1.png");
    friendCircleBtn->setPosition(Vec2(bgSize.width / 2 - 180, bgSize.height / 2 - 40));
    friendCircleBtn->setTag(0);
    bg->addChild(friendCircleBtn);
    friendCircleBtn->addTouchEventListener(CC_CALLBACK_2(PopularizeOfMy::shareCallback, this));
    
    auto friendBtn = Button::create("v88/module/fenxiang/2.png");
    friendBtn->setPosition(Vec2(bgSize.width / 2 + 180, bgSize.height / 2 - 40));
    friendBtn->setTag(1);
    bg->addChild(friendBtn);
    friendBtn->addTouchEventListener(CC_CALLBACK_2(PopularizeOfMy::shareCallback, this));
}

void PopularizeOfMy::shareCallback(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    
    bool bIsInstallWeChat = false;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bIsInstallWeChat = WechatMgr::isAppInstalled();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    bIsInstallWeChat = WeChatAndroid::getInstance()->isAppInstalled();
#endif
    if (!bIsInstallWeChat) {
        CommandManager::getInstance()->showGamePrompt(GBKToUtf8("请安装最新微信客户端"));
        return;
    }
    
    auto btn = (Button*)pSender;
    int tag = btn->getTag();
    
    char title[128] = { 0 };
    if (tag == 0) {
        sprintf(title, GetText("EXPERIENCE_CODE_INVITE_TITLE1"), PlatformLogic()->loginResult.dwUserID);
    }
    else {
        sprintf(title, "%s", GetText("EXPERIENCE_CODE_INVITE_TITLE2"));
    }
    
    char content[256] = { 0 };
    sprintf(content, GetText("EXPERIENCE_CODE_INVITE_CONTENT"), PlatformLogic()->loginResult.dwUserID);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WechatMgr::shareContentLinkToWechatOrTimeLineSession(title, content, tag);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    WeChatAndroid::getInstance()->shareWebpage(tag, title, content, getWebServerUrl() + "/share.aspx");
#endif
    
    closeLayer();
//    if (btn->getParent()) {
//        btn->getParent()->getParent()->removeFromParent();
//    }
}
