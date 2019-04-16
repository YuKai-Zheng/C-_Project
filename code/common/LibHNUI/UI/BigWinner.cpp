//
//  BigWinner.cpp
//  LibHNUI
//
//  Created by mac on 2018/8/4.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "BigWinner.hpp"
#include "cocostudio/CocoStudio.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "Sound/HNAudioEngine.h"

namespace HN {
    const static int CSLL_GAME_BIGWIN = 1;
    const static int JSXS_GAME_BIGWIN = 2;
    
    const static int CSLL_GAME_BIGWIN_UITYPE_1 = 1;
    const static int CSLL_GAME_BIGWIN_UITYPE_2 = 2;
    
    const static int JSXS_GAME_BIGWIN_UITYPE_1 = 1;
    const static int JSXS_GAME_BIGWIN_UITYPE_2 = 2;
    

BigWinnerLayer::BigWinnerLayer()
{
    m_winMoney  = 0;
    m_curMoney  = 0;
    m_moneyText = nullptr;
    nMusicEffectId = -1;
}
BigWinnerLayer::~BigWinnerLayer()
{
    
    
}
void BigWinnerLayer::onExit()
{
    HNLayer::onExit();
    unscheduleUpdate();
    if (nMusicEffectId != -1) {
        HNAudioEngine::getInstance()->stopEffectWithID(nMusicEffectId);
    }
        
}
bool BigWinnerLayer::init()
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
    
    auto layer = LayerColor::create(Color4B(0, 0, 0, 100));
    layer->setPosition(Vec2::ZERO);
    this->addChild(layer);
    
    return true;
}
void BigWinnerLayer::open(Node* parent, Vec2 vec, int zorder,int gameType,int uiType,long long winMoney)
{
    m_winMoney = winMoney;
    if (parent) {
        this->setPosition(vec);
        parent->addChild(this,zorder);
        
        createUIByType(gameType,uiType);

        
    }
    
    
}
void BigWinnerLayer::close()
{
    this->removeFromParent();
    
}
void BigWinnerLayer::createUIByType(int _gameType,int _uiType)
{
    vector<std::string> pathVec;
    pathVec.clear();
    
    pathVec.push_back("Games/31000507/animation/ui/di.png");
    pathVec.push_back("Games/31000507/animation/ui/jinbikuang.png");
    pathVec.push_back("Games/31000507/starNode.csb");
    
    for (int m = 1; m < pathVec.size(); m++) {
        std::string path = pathVec.at(m);
        if (m == pathVec.size() - 1) {
            auto starNode = CSLoader::createNode(path);
            if (starNode) {
                starNode->setPosition(WIN_SIZE/2);
                addChild(starNode,1);
                auto action = CSLoader::createTimeline(path);
                if (action) {
                    starNode->runAction(action);
                    action->gotoFrameAndPlay(0);
                }
            }
        }
        else
        {
            auto boxImg = ImageView::create(path);
            if (boxImg) {
                boxImg->setPosition(WIN_SIZE/2);
                this->addChild(boxImg,1);
                
                if (path.compare("Games/31000507/animation/ui/jinbikuang.png") == 0) {
                    m_moneyText = TextAtlas::create("","Games/31000507/ui/shuzi1.png",70,80,"0");
                    if (m_moneyText) {
                        m_moneyText->setPosition(Vec2(760,280));
                        boxImg->addChild(m_moneyText);
                        m_moneyText->setString("0");
                        
                        unscheduleUpdate();
                        scheduleUpdate();
                    }
                    
                }
            }
           
            
        }
        
    }
    
    std::string imgPath1 = "";
    std::string imgPath2 = "";
    
    std::string backGroundEffect = "Games/31000508/music/bigWin.mp3";;
    if (_gameType == CSLL_GAME_BIGWIN) {
        imgPath1 = "Games/31000507/animation/ui/cs_dyj.png";
        auto img1 = ImageView::create(imgPath1);
        if (img1) {
            img1->setPosition(WIN_SIZE/2);
            this->addChild(img1,1);
            
            std::string pathStr = "Games/31000507/catNode.csb";
            auto handNode = CSLoader::createNode(pathStr);
            if (handNode) {
                handNode->setPosition(Vec2(1000,450));
                img1->addChild(handNode);
                auto action = CSLoader::createTimeline(pathStr);
                if (action) {
                    handNode->runAction(action);
                    action->gotoFrameAndPlay(0);
                }
            }
            
        }
        
        if (CSLL_GAME_BIGWIN_UITYPE_1 == _uiType) {
            
            imgPath2 = "Games/31000507/animation/ui/cs_dyj_mao.png";
           
        }
        else if(CSLL_GAME_BIGWIN_UITYPE_2 == _uiType)
        {
            imgPath2 = "Games/31000507/animation/ui/cs_cjdyj.png";
            backGroundEffect = "Games/31000508/music/superWin.mp3";
        }
        
        if (!imgPath2.empty()) {
            auto img2 = ImageView::create(imgPath2);
            if (img2) {
                img2->setPosition(WIN_SIZE/2);
                this->addChild(img2,1);
            }
        }
        
        
    }
    else if(_gameType == JSXS_GAME_BIGWIN)
    {
        imgPath1 = "Games/31000507/animation/ui/js_dyj.png";
        auto img1 = ImageView::create(imgPath1);
        if (img1) {
            img1->setPosition(WIN_SIZE/2);
            this->addChild(img1,1);
            
            std::string pathStr = "Games/31000507/lightNode.csb";
            auto lightNode = CSLoader::createNode(pathStr);
            if (lightNode) {
                lightNode->setPosition(WIN_SIZE/2);
                img1->addChild(lightNode);
                auto action = CSLoader::createTimeline(pathStr);
                if (action) {
                    lightNode->runAction(action);
                    action->gotoFrameAndPlay(0);
                }
            }
            
        }
        
        if (JSXS_GAME_BIGWIN_UITYPE_2 == _uiType) {
            
            auto img2 = ImageView::create("Games/31000507/animation/ui/js_cjdyj.png");
            if (img2) {
                img2->setPosition(WIN_SIZE/2);
                addChild(img2,1);
            }
            backGroundEffect = "Games/31000508/music/superWin.mp3";
        }
        
    }
    
    if (!backGroundEffect.empty()) {
        nMusicEffectId = HNAudioEngine::getInstance()->playEffect(backGroundEffect.c_str());
    }
    
    auto jsonPath      = "v66/animation/dyj/dayingjia_jinbi.json";
    auto atlasPath     = "v66/animation/dyj/dayingjia_jinbi.atlas";
    
    
    int maxSize = 10;
    for (int i = 0; i <= maxSize; i++) {
        runAction(Sequence::create(DelayTime::create(i*0.2),CallFunc::create([=](){
            auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
            if (skeletonNode) {
                
                if (i >= maxSize/2) {
                    skeletonNode->setScaleY(-1);
                    skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 640,WIN_SIZE.height));
                }
                else
                {
                    skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 640,0));
                }
                skeletonNode->setAnimation(0,"dayingjia_jinbi",false);
                skeletonNode->setEndListener([=](int trackIndex){
                    
                });
                addChild(skeletonNode,1);
                
                
            }
        }),NULL));
        if (i == maxSize) {
            runAction(Sequence::create( DelayTime::create(3.0f),RemoveSelf::create(),NULL));
        }
        
    }
  }
    
  void BigWinnerLayer::showXunhuanEffect()
  {
     /* std::string pathMoney = "Games/CSLL/money2Node.csb";
      auto moneyNode = CSLoader::createNode(pathMoney);
      if (moneyNode) {
          moneyNode->setPosition(WIN_SIZE/2);
          addChild(moneyNode);
          auto action = CSLoader::createTimeline(pathMoney);
          if (action) {
              moneyNode->runAction(action);
              action->gotoFrameAndPlay(0);
          }
      }
      
      runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
          
          std::string pathMoney2 = "Games/31000507/money2Node.csb";
          auto moneyNode2 = CSLoader::createNode(pathMoney2);
          if (moneyNode2) {
              moneyNode2->setPosition(WIN_SIZE/2);
              moneyNode2->setRotation(180);
              addChild(moneyNode2);
              auto action = CSLoader::createTimeline(pathMoney2);
              if (action) {
                  moneyNode2->runAction(action);
                  action->gotoFrameAndPlay(0);
              }
          }
          
      }),DelayTime::create(2.0f),RemoveSelf::create(),NULL));*/
      
  }
    void BigWinnerLayer::update(float dt)
    {
        m_curMoney += (m_winMoney/70);
        m_curMoney = m_curMoney >= m_winMoney ? m_winMoney : m_curMoney;
       
        m_moneyText->setString(StringUtils::format("%lld",m_curMoney));
        if (m_curMoney >= m_winMoney) {
            unscheduleUpdate();
        }
        
    }
    
};
