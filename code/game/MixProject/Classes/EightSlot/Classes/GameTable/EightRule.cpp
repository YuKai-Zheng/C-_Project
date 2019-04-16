//
//  EightRule.cpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/2.
//

#include "EightRule.hpp"
#include "cocostudio/CocoStudio.h"
EightRule::EightRule()
{
    m_pageView = nullptr;
    m_select   = nullptr;
    imgBg      = nullptr;
    m_curIndex = 0;
    memset(m_nLotteryRate, 0, sizeof(m_nLotteryRate));
    m_Note = 0;
    
}
EightRule::~EightRule()
{
    
    
}
bool EightRule::init()
{
    if (!HNLayer::init()) {
        return false;
    }
    
    
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [this](Touch *touch, Event *unused_event){
        
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(colorLayer);
    
    auto nodeMiddle = CSLoader::createNode("Games/31000512/RuleNode.csb");
    if (nodeMiddle) {
        imgBg = dynamic_cast<ImageView*>(nodeMiddle->getChildByName("Image_1"));
        if (imgBg) {
            auto closeBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_close"));
            if (closeBtn) {
                closeBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    close();
                });
                
            }
            
            auto leftBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_left"));
            if (leftBtn) {
                leftBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    if (m_pageView->getCurrentPageIndex() == 1) {
                        m_pageView->scrollToPage(0);
                        m_select->setPositionX(480);
                    }
                });
            }
            auto rightBtn = dynamic_cast<Button*>(imgBg->getChildByName("Button_right"));
            if (rightBtn) {
                rightBtn->addTouchEventListener([this](cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
                    if (type != Widget::TouchEventType::ENDED) {
                        return;
                    }
                    if (m_pageView->getCurrentPageIndex() == 0) {
                        m_pageView->scrollToPage(1);
                        m_select->setPositionX(553);
                    }
                    
                });
            }
            m_select   = dynamic_cast<ImageView*>(imgBg->getChildByName("Image_select"));
            m_pageView = dynamic_cast<PageView*>(imgBg->getChildByName("PageView_1"));
            
            
        }
        
    }
    nodeMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nodeMiddle->setPosition(WIN_SIZE/2);
    addChild(nodeMiddle);
    return true;
}
void EightRule::setData(int data[7],long long note)
{
    memcpy(m_nLotteryRate, data, sizeof(m_nLotteryRate));
    m_Note = note;
    
    if (m_pageView) {
        createPage();
    }
}
void EightRule::createPage()
{
    auto panel_One  = Layout::create();
    panel_One->setContentSize(m_pageView->getContentSize());
    m_pageView->addPage(panel_One);
    auto contentImg = ImageView::create("Games/31000512/rule/img_Bangzhu01.png");
    if (contentImg) {
        contentImg->setPosition(panel_One->getContentSize()/2);
        panel_One->addChild(contentImg);
    }
    
    auto panel_Two  = Layout::create();
    panel_Two->setContentSize(m_pageView->getContentSize());
    m_pageView->addPage(panel_Two);
    auto contentImgEx = ImageView::create("Games/31000512/rule/img_Bangzhu02.png");
    if (contentImgEx) {
        contentImgEx->setPosition(panel_Two->getContentSize()/2);
        panel_Two->addChild(contentImgEx);
        
        
        vector<Vec2> posVec;
        posVec.push_back(Vec2(602,55));
        posVec.push_back(Vec2(368,55));
        posVec.push_back(Vec2(133,55));
        
        posVec.push_back(Vec2(839,310));
        posVec.push_back(Vec2(602,310));
        posVec.push_back(Vec2(368,310));
        posVec.push_back(Vec2(134,310));
        
        
        for (int m = 0; m < 7; m++) {
            auto atlasText = TextAtlas::create("", "Games/31000512/ui/jackpot.png", 72, 96, "+");
            if (atlasText) {
                atlasText->setScale(0.25f);
                atlasText->setPosition(posVec.at(m));
                contentImgEx->addChild(atlasText);
                
                long long value = m_Note * (float)m_nLotteryRate[m]/100;
                atlasText->setString(StringUtils::format("%lld",value));
            }
        }
        
    }
    
    m_pageView->addEventListener(CC_CALLBACK_2(EightRule::selectItemCallback,this));
    m_pageView->setClippingEnabled(true);
    m_pageView->scrollToPage(0);
}

void EightRule::open(Node* parent, Vec2 vec, int zorder, int tag)
{
    this->setPosition(vec);
    parent->addChild(this,zorder);
    if (imgBg) {
        imgBg->setScale(0.01f);
        auto delayTime = DelayTime::create(0.001f);
        auto pScaleTo1 = ScaleTo::create(0.2f,1.15f);
        auto pScaleTo2 = ScaleTo::create(0.1f,1);
        imgBg->runAction(Sequence::create(delayTime,pScaleTo1,pScaleTo2,NULL));
        
    }
    
    
}
void EightRule::close()
{
    this->removeFromParent();
    
}
void EightRule::selectItemCallback(Ref* pSender, PageView::EventType type)
{
    switch (type) {
        case PageView::EventType::TURNING :
            if (m_pageView->getCurrentPageIndex() == 0) {
                m_select->setPositionX(480);
            }
            else
            {
                m_select->setPositionX(553);
            }
            break;
            
        default:
            break;
    }
    
}

void EightRule::handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type)
{
    
    
}

