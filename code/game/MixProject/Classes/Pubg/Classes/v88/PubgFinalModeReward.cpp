//
//  PubgFinalModeReward.cpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/5/26.
//

#include "PubgFinalModeReward.hpp"
PubgFinalRewardLayer::PubgFinalRewardLayer()
: topTen(nullptr)
, numberOne(nullptr)
, rank1Text(nullptr)
, rank2Text(nullptr)
, killNum(nullptr)
, getMoney(nullptr)
, buttonOK(nullptr)
{
}

PubgFinalRewardLayer::~PubgFinalRewardLayer() {
}

void PubgFinalRewardLayer::showData(int rank,int killPeoPleNum,long long getMoneyNum,Node* parent) {
    if (rank) {
        topTen->setVisible(rank > 1);
        numberOne->setVisible(rank == 1);
        char rankValue[32] = {0};
        sprintf(rankValue,"%d",rank);
        rank1Text->setString(rankValue);
        rank2Text->setString(rankValue);
        
       
    }
    if (killPeoPleNum) {
        char killValue[32] = {0};
        sprintf(killValue,"%d",killPeoPleNum);
        killNum->setString(killValue);
    }
    
    if (getMoneyNum > 0) {
        char moneyValue[32] = {0};
        sprintf(moneyValue, "+%s", Tools::addComma(getMoneyNum).c_str());
        getMoney->setString(moneyValue);
    }
    
    if (parent) {
        
        if (parent ->getChildByTag(10000) == NULL) {
            //this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            this->setPosition(Vec2::ZERO);
            parent->addChild(this,10000);
            
            
            this->setScale(0.01f);
            auto delayTime = DelayTime::create(0.001f);
            auto pScaleTo1 = ScaleTo::create(0.2f,1.15f);
            auto pScaleTo2 = ScaleTo::create(0.1f,1);
            this->runAction(Sequence::create(delayTime,pScaleTo1,pScaleTo2,NULL));
        }
       
    }
    

}

bool PubgFinalRewardLayer::init() {
    if (!HNLayer::init())
        return false;
    
    //屏蔽后面的层
    

    this->setContentSize(WIN_SIZE);
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
       
        return true;
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Node* middle = CSLoader::createNode("Games/31000511/RewardNode.csb");
    if (middle) {
        middle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        middle->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
        addChild(middle);
        auto imgBg = middle->getChildByName("Image_1");
        if (imgBg) {
            topTen = dynamic_cast<ImageView*>(imgBg->getChildByName("Image_3"));
            numberOne = dynamic_cast<ImageView*>(imgBg->getChildByName("Image_2"));
            rank1Text= dynamic_cast<Text*>(imgBg->getChildByName("Text_rank1"));
            rank2Text = dynamic_cast<Text*>(imgBg->getChildByName("Text_rank2"));
            killNum = dynamic_cast<Text*>(imgBg->getChildByName("Text_killNum"));
            getMoney = dynamic_cast<Text*>(imgBg->getChildByName("Text_getMoney"));
            buttonOK = dynamic_cast<Button*>(imgBg->getChildByName("Button_1"));
            if (buttonOK) {
                buttonOK->addTouchEventListener(CC_CALLBACK_2(PubgFinalRewardLayer::setOkayCallBack, this));
            }
        }
    }


    return true;
}



void PubgFinalRewardLayer::setOkayCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    
    this->removeFromParent();
    
}

