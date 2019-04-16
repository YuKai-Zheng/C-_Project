//
//  IGameTableUICallback.cpp
//  LibHNLobby
//
//  Created by cocos on 2018/1/18.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "IGameTableUI.h"
#include "HNLobby/GameNotice.h"
#include "HNLobby/GamePrompt.h"
#include "ExtraReward.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "UI/MoneyGetFourLevel.hpp"
#include "UI/BigWinner.hpp"
const int kNoticeZOrder = 200;
const static char* kFreeTurnUI = "csb/v66/Node_freeTurn.csb";//"common/freeTurn/freeTurn.csb"

IGameTableUI::IGameTableUI()
    : m_Notice(nullptr)
    , m_freeTurnUI(nullptr)
    , m_txtFreeTurn(nullptr)
    ,m_loadUI(nullptr)
{
}

void IGameTableUI::onHandleNewMessage(int iType, const std::string &message, int iCount)
{
    if (iType == 0)
    {
        //全服公告
        showNotice(message, 100, iType);
    }
    else
    {
        showNotice(message, iCount, iType);
    }
}

void IGameTableUI::showNotice(const std::string &message, int nCount, int nType, bool bVisible)
{
    if (!bVisible)
    {
        return;
    }
    
    if (m_Notice == nullptr)
    {
        m_Notice = GameNotice::create();
        m_Notice->setLocalZOrder(kNoticeZOrder);
        m_Notice->setName("GameNotice");
    }
    
    m_Notice->openNotic(this, nType, message, nCount);
}

void IGameTableUI::onLuckyMoneyNotify(LuckyMoneyNotify *msg)
{
    if (nullptr == msg)
    {
        return;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto notify = CSLoader::createNode("Reward/ui.csb");
    notify->setName("RewardNotice");
    auto btnOpenReward = dynamic_cast<Button*>(notify->getChildByName("btnOpen"));
    btnOpenReward->addTouchEventListener(std::bind([&](Ref* psender, Widget::TouchEventType type){
        if (Widget::TouchEventType::ENDED != type)
        {
            return;
        }
        
        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_LUCKYMONEY, 0, 0);
        this->removeChildByName("RewardNotice");
    }, std::placeholders::_1, std::placeholders::_2));
    
    addChild(notify, 200);
    notify->setPosition(Vec2(visibleSize)/2);
    
    runAction(Sequence::create(DelayTime::create(msg->nStayTime), CallFunc::create([&](){
        this->removeChildByName("RewardNotice");
    }), NULL));
}

void IGameTableUI::onLuckyMoneyResult(LuckyMoneyResult *msg)
{
    if (nullptr == msg)
    {
        return;
    }
    
    if (msg->nType != 1)
    {
        GamePromptLayer::create()->showPrompt("手慢无！下次快点哦。");
        return;
    }
    
    auto result = CSLoader::createNode("Reward/reward.csb");
    auto action = CSLoader::createTimeline("Reward/reward.csb");
    if (nullptr != result && nullptr != action)
    {
        addChild(result, 200);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        result->setPosition(Vec2(visibleSize)/2);
        result->setName("rewardResult");
        result->getChildByName("show")->runAction(action);
        action->gotoFrameAndPlay(0, false);
        
        auto txtReward = dynamic_cast<Text*>(result->getChildByName("txtReward"));
        txtReward->setString("");
        
        auto luckyMoney = msg->i64LuckyMoney;
        auto retainTime = msg->nShowTime;
        auto i64MyMoney = msg->i64MyMoney;
        runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
            txtReward->setString(Tools::addComma(luckyMoney));
            updateMyMoney(i64MyMoney);
        }), DelayTime::create(retainTime), CallFunc::create([&](){
            this->removeChildByName("rewardResult");
        }), NULL));
    }
}


void IGameTableUI::loadFreeTurnUI()
{
    m_freeTurnUI = nullptr;
    auto ui = CSLoader::createNode(kFreeTurnUI);
    if (nullptr != ui)
    {
        auto sz = Director::getInstance()->getWinSizeInPixels();
        sz = sz / 2;
        sz.height -= 100;
        

        m_freeTurnUI = ui;
        m_freeTurnUI->setPosition(sz);
        m_freeTurnUI->setVisible(true);
        
        this->addChild(m_freeTurnUI, kNoticeZOrder - 1);    // 漂浮金币层下面
        
        auto bg = dynamic_cast<ImageView*>(m_freeTurnUI->getChildByName("Image_1"));
        if (nullptr != bg)
        {
            m_txtFreeTurn = dynamic_cast<Text*>(bg->getChildByName("Text_num"));
        }
    }
}

void IGameTableUI::tryShowFreeTurnUI(int count)
{
    if (count <= 0 && nullptr != m_freeTurnUI)
    {
        m_freeTurnUI->setVisible(false);
    }
    
    if (count > 0)
    {
        if (m_freeTurnUI == nullptr)
        {
            loadFreeTurnUI();
        }
        
        if (nullptr != m_freeTurnUI && nullptr != m_txtFreeTurn)
        {
            m_freeTurnUI->setVisible(true);
            m_txtFreeTurn->setString(Tools::addComma(count));
        }
    }
}
void IGameTableUI::setBackGroundImgFunc(std::string imgPath,Node* parentNode)
{
    auto loadingBG = BackgroundFactory::create(imgPath.c_str());
    loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2));
    parentNode->addChild(loadingBG);
    
}
void IGameTableUI::showLoadingResUI(int gameIdx)
{
    if (m_loadUI == nullptr) {
        m_loadUI = IGameLoadingUI::create(true, gameIdx);
        if (m_loadUI) {
            m_loadUI->loadRes();
            m_loadUI->onCloseCallBack = [this](){
            this->closeLoadingCallBack();
            };
            addChild(m_loadUI);
        }
    }
    
}
void IGameTableUI::showFourLevelMoney(int level)
{
    
    auto m_Layer = MoneyGetFourLevel::create();
    if (m_Layer) {
        m_Layer->open(this,Vec2::ZERO,200,level);
    }
}
void IGameTableUI::showBiggerWinLayer(int _gameType,int _uitype,long long winMoney)
{
    auto bigWinLayer = BigWinnerLayer::create();
    if (bigWinLayer) {
        bigWinLayer->open(this,Vec2::ZERO,200,_gameType,_uitype,winMoney);
    }
    
}

