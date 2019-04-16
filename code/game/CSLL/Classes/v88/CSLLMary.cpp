//
//  CSLLMary.cpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/14.
//

#include "CSLLMary.hpp"
#include "cocostudio/CocoStudio.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "Tool/Tools.h"
#include "Sound/HNAudioEngine.h"
namespace CSLL {
    const static char* CSLL_MARY_START  = "Games/31000507/music/smallGameStart.mp3";
    const static char* CSLL_MARY_BGM    = "Games/31000507/music/smallGameBGM.mp3";
    const static char* CSLL_MARY_YAOSHI = "Games/31000507/music/yaoshi.mp3";
    const static char* CSLL_MARY_FEN    = "Games/31000507/music/chufen.mp3";
    const static char* CSLL_MARY_END    = "Games/31000507/music/smallGameEnd.mp3";
    const static int BOX_TYPE_LIST[TREASURE_BOX_COUNT] = {
        2, 2, 2, 1,
        1, 3, 3, 3, 2,
        2, 3, 1, 3, 3,
        3, 1, 3, 1, 2,
        3, 1, 3, 1, 2,
    };
    const static Vec2 BOX_TYPE_EFFECT_POS[3] = {
        Vec2(94,106),
        Vec2(92,126),
        Vec2(110,150)
    };
    
    CSLLMary::CSLLMary()
    {
        yuanbaoNode= nullptr;
        _getReward = nullptr;
        _freeNum   = nullptr;
        btnBoxVec.clear();
        curSelectBox = nullptr;
        maryStart    = nullptr;
        
        nMaryCount   = 0;            //剩余打开次数
        nOpenType    = 0;            //本次打开的类型
        i64OpenMoney = 0;            //本次打开的金额
        i64AllMoney  = 0;            //累计的总金额
        
        isTouch      = false;
        
        i64OpenMoney_end = 0;            //宝箱打开的总金额
        i64NoteMoney_end = 0;            //玩家本局的总押注
        i64AllMoney_end  = 0;            //小游戏最后总奖金
        
        myEndFunc  = NULL;
        myOpenFunc = NULL;
        
        vecBoxHasOpen.clear();
        memset(nOpenTypes,0,sizeof(nOpenTypes));
        memset(i64BoxMoney,0,sizeof(i64BoxMoney));
    }
    CSLLMary::~CSLLMary()
    {
        
    }
    bool CSLLMary::init()
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
        
        HNAudioEngine::getInstance()->stopBackgroundMusic();
        HNAudioEngine::getInstance()->playBackgroundMusic(CSLL_MARY_START);
        maryStart = CSLoader::createNode("Games/31000507/Node_maryStart.csb");
        if (maryStart) {
            maryStart->setPosition(WIN_SIZE/2);
            addChild(maryStart,1);
            
            auto panel = dynamic_cast<Layout*>(maryStart->getChildByName("Panel_1"));
            if (panel) {
                auto leftDoor  = dynamic_cast<ImageView*>(panel->getChildByName("Image_1"));
                auto rightDoor = dynamic_cast<ImageView*>(panel->getChildByName("Image_2"));
                
                auto btnBg = dynamic_cast<ImageView*>(panel->getChildByName("Image_3"));
                if (btnBg) {
                    auto btnStart = dynamic_cast<Button*>(btnBg->getChildByName("Button_1"));
                    if (btnStart) {
                        btnStart->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                            if (type != Widget::TouchEventType::ENDED) {
                                return;
                            }
                            
                            this->createBoxUI();
                            if (leftDoor) {
                                leftDoor->runAction(MoveBy::create(1.0f,Vec2(-800,0)));
                            }
                            if (rightDoor) {
                                rightDoor->runAction(MoveBy::create(1.0f,Vec2(800,0)));
                            }
                            btnBg->runAction(Sequence::create(ScaleTo::create(0.7f, 0.1f),CallFunc::create([=](){
                                maryStart->removeFromParent();
                            }),NULL));
                            
                        });
                    }
                }
            }
            
        }
       
        
        return true;
    }
    void CSLLMary::createBoxUI()
    {
        HNAudioEngine::getInstance()->stopBackgroundMusic();
        HNAudioEngine::getInstance()->playBackgroundMusic(CSLL_MARY_BGM);
        auto imgBg = ImageView::create("Games/31000507/special/cs_bj.png");
        if (imgBg) {
            imgBg->setPosition(WIN_SIZE/2);
            addChild(imgBg);
        }
        
        
        auto jsonPath      = "Games/31000507/animation/csy/caishenye.json";
        auto atlasPath     = "Games/31000507/animation/csy/caishenye.atlas";
        
        auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
        if (skeletonNode) {
            skeletonNode->setPosition(Vec2(WIN_SIZE.width/2-660,30));
            skeletonNode->setAnimation(0,"caishenye",true);
            addChild(skeletonNode);
        }
        
        auto rootNode = CSLoader::createNode("Games/31000507/Node_mary.csb");
        if (rootNode) {
            rootNode->setPosition(WIN_SIZE/2);
            addChild(rootNode);
            
            auto panelBg = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_7"));
            if (panelBg)
            {
                yuanbaoNode = dynamic_cast<Node*>(panelBg->getChildByName("Node_yuanbao"));
                auto img = dynamic_cast<ImageView*>(panelBg->getChildByName("Image_2"));
                if (img) {
                    _getReward = dynamic_cast<Text*>(img->getChildByName("Text_money"));
                    _freeNum   = dynamic_cast<Text*>(img->getChildByName("Text_num"));
                }
                
                for (int i = 1; i <= TREASURE_BOX_COUNT; i++) {
                    std::string btnName = StringUtils::format("Button_%d",i);
                    auto btn            = dynamic_cast<Button*>(panelBg->getChildByName(btnName));
                    if (btn) {
                        btn->addTouchEventListener(CC_CALLBACK_2(CSLLMary::openBoxCallBack,this));
                        btnBoxVec.push_back(btn);
                        
                        btn->setPositionY(btn->getPositionY() + 1000);
                        btn->runAction(MoveBy::create(0.5f,Vec2(0,-1000)));
                    }
                }
                
            }
        }
        setMaryCountAndAllMoney(false);
        
    }
    void CSLLMary::playYuanbao()
    {
        if (yuanbaoNode) {
            auto actionNode = CSLoader::createNode("Games/31000507/Node_yuanbao.csb");
            if (actionNode) {
                yuanbaoNode->addChild(actionNode);
                auto action = CSLoader::createTimeline("Games/31000507/Node_yuanbao.csb");
                if (action) {
                    actionNode->runAction(action);
                    action->gotoFrameAndPlay(0,false);
                }
            }
        }
    }
    void CSLLMary::playBoxEffect(Button* btnNode,bool realOpen,int showType,long long showMoney)
    {
        if (btnNode) {
            btnNode->setTouchEnabled(false);
            btnNode->setBright(false);
            auto btnName = btnNode->getName();
            auto btnType = btnName.substr(7);
            
            int _type    = atoi(btnType.c_str());
            if (_type >= 1 && _type <= TREASURE_BOX_COUNT) {
                int boxType = BOX_TYPE_LIST[_type - 1];
                if (boxType >= 1 && boxType <= 3) {
                    Vec2 posEffect = BOX_TYPE_EFFECT_POS[boxType - 1];
                    Node* parNode  = Node::create();
                    parNode->setPosition(posEffect);
                    btnNode->addChild(parNode);
                    auto actionNode = CSLoader::createNode("Games/31000507/Node_boxLight.csb");
                    if (actionNode) {
                        actionNode->setPosition(Vec2(0,0));
                        parNode->addChild(actionNode);
                        auto action = CSLoader::createTimeline("Games/31000507/Node_boxLight.csb");
                        if (action) {
                            actionNode->runAction(action);
                            action->gotoFrameAndPlay(0,true);
                        }
                    }
                    
                    std::string fontPngPath = realOpen ? "v66/fontpng/jiesuan2.png":"v66/fontpng/jiesuan3.png";
                    auto m_moneyText = TextAtlas::create("", fontPngPath, 72, 96, "+");
                    m_moneyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    m_moneyText->setPosition(posEffect);
                    btnNode->addChild(m_moneyText);
                    m_moneyText->setScale(0.5f);
                    m_moneyText->setName("HAS_OPEN_BOX_TEXT");
                    
                    char moneyValue[32];
                    int animationType = _type;
                    if (showType == OT_MONEY) {
                        sprintf(moneyValue, "%s", Tools::addComma(showMoney).c_str());
                        HNAudioEngine::getInstance()->playEffect(CSLL_MARY_FEN);
                    }
                    else if (showType == OT_TIMES)
                    {
                        sprintf(moneyValue, "+%s", Tools::addComma(showMoney).c_str());
                        animationType = _type + TREASURE_BOX_COUNT;
                        HNAudioEngine::getInstance()->playEffect(CSLL_MARY_YAOSHI);
                    }
                    
                    m_moneyText->setString(moneyValue);
                    
                    
                    if (realOpen)
                    {
                        auto jsonPath      = "Games/31000507/animation/caishen_guangshu/caishen_guangshu.json";
                        auto atlasPath     = "Games/31000507/animation/caishen_guangshu/caishen_guangshu.atlas";
                        auto animationName = StringUtils::format("%d",animationType);
                        auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                        if (skeletonNode) {
                            skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 640,0));
                            skeletonNode->setAnimation(0, animationName,false);
                            skeletonNode->setEndListener([=](int trackIndex){
                                this->playYuanbao();
                                this->setMaryCountAndAllMoney(nOpenType == OT_MONEY);
                                isTouch = false;
                                
                                if (nMaryCount == 0) {
                                    this->showNotOpenBox();
                                    this->showMaryEnd();
                                }
                                skeletonNode->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
                            });
                            addChild(skeletonNode,1000);
                        }
                    }
                }
                
               
                
            }
        }
        
    }
    void CSLLMary::openBoxCallBack(Ref* pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
       
        if (isTouch || nMaryCount == 0) {
            return;
        }
        
        auto btnNode = (Button*)pSender;
        curSelectBox = btnNode;
        
        isTouch = true;
        
        auto btnName = btnNode->getName();
        auto btnType = btnName.substr(7);
        int _type    = atoi(btnType.c_str());
        
        vecBoxHasOpen.push_back(_type - 1);
        if (myOpenFunc) {
            myOpenFunc(_type - 1);
        }
        
        
        
    }
    void CSLLMary::open(Node* parent, Vec2 vec, int zorder)
    {
        if (parent) {
            this->setPosition(vec);
            parent->addChild(this,zorder);
        }
    }
    void CSLLMary::close()
    {
        this->removeFromParent();
    }
    void CSLLMary::dealBoxOpenResult(struct_s2c_open_result* msg)
    {
        nMaryCount   = msg->nMaryCount;
        nOpenType    = msg->nOpenType;
        i64OpenMoney = msg->i64OpenMoney;
        i64AllMoney  = msg->i64AllMoney;
        
        if (curSelectBox) {
            playBoxEffect(curSelectBox,true,nOpenType,i64OpenMoney);
        }
        
        
    }
    void CSLLMary::setMaryCountAndAllMoney(bool show)
    {
        if (_getReward) {
            std::string numMoney = StringUtils::format("%lld",i64AllMoney);
            _getReward->setString(numMoney);
            if (show) {
                for (int i = 0; i < 2; i++) {
                     auto img = ImageView::create("Games/31000507/special/kuang.png");
                     img->setPosition(_getReward->getContentSize()/2);
                     _getReward->addChild(img);
                    
                    img->setVisible(false);
                    img->runAction(Sequence::create(DelayTime::create((i+1)*0.3),CallFunc::create([=](){
                        img->setVisible(true);
                    }),ScaleTo::create(0.4f,1.3f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
                    
                }
            }
        }
        if (_freeNum) {
            std::string numFree = StringUtils::format("%d",nMaryCount);
            _freeNum->setString(numFree);
        }
    }
    void CSLLMary::dealMaryEnd(struct_s2c_mary_result* msg)
    {
        i64OpenMoney_end = msg->i64OpenMoney;
        i64NoteMoney_end = msg->i64NoteMoney;
        i64AllMoney_end  = msg->i64AllMoney;
        
        memcpy(nOpenTypes, msg->nOpenTypes, sizeof(nOpenTypes));
        memcpy(i64BoxMoney, msg->i64BoxMoney, sizeof(i64BoxMoney));
    }
    void CSLLMary::showMaryEnd()
    {
        HNAudioEngine::getInstance()->playEffect(CSLL_MARY_END);
        auto endNode = CSLoader::createNode("Games/31000507/Node_maryEnd.csb");
        if (endNode) {
            endNode->setScale(0.1f);
            endNode->runAction(ScaleTo::create(0.4f,1.0f));
            endNode->setPosition(WIN_SIZE/2);
            addChild(endNode);
            
            auto panelBg = dynamic_cast<Layout*>(endNode->getChildByName("Panel_1"));
            if (panelBg) {
                auto backBtn = dynamic_cast<Button*>(panelBg->getChildByName("Button_1"));
                if (backBtn)
                {
                    backBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                        if (type != Widget::TouchEventType::ENDED) {
                            return;
                        }
                        if (myEndFunc) {
                            myEndFunc(i64AllMoney_end);
                        }
                        close();
                        
                    });
                    
                }
                
                auto score = dynamic_cast<Text*>(panelBg->getChildByName("Text_score"));
                if (score) {
                    score->setString(StringUtils::format("%lld X %lld = %lld",i64OpenMoney_end,i64NoteMoney_end,i64AllMoney_end));
                }
                
                auto m_moneyText = TextAtlas::create("", "v66/fontpng/jiesuan2.png", 72, 96, "+");
                m_moneyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_moneyText->setPosition(Vec2(640,350));
                panelBg->addChild(m_moneyText);
                
                char moneyValue[32];
                sprintf(moneyValue, "%s", Tools::addComma(i64AllMoney_end).c_str());
                m_moneyText->setString(moneyValue);
                m_moneyText->setScale(0.70f);
            }
            
            
            
        }
    }
    void CSLLMary::setMaryEndCallBack(EndCallback func)
    {
        myEndFunc = func;
        
    }
    void CSLLMary::setMaryMsgToServerCallBack(OpenCallback func)
    {
        myOpenFunc = func;
    }
    void CSLLMary::setStartMaryCount(int _maryCount)
    {
        nMaryCount = _maryCount;
        
    }
    void CSLLMary::showNotOpenBox()
    {
        for (int i = 0; i < TREASURE_BOX_COUNT; i++) {
            long long boxMoney = i64BoxMoney[i];
            bool notOpen = true;
            for (int j = 0; j < vecBoxHasOpen.size(); j++) {
                if (i == vecBoxHasOpen.at(j)) {
                    notOpen = false;
                    break;
                }
            }
            if (notOpen) {
                int tempnOpenType = nOpenTypes[i];
                for (int m = 0; m < btnBoxVec.size(); m++) {
                     auto btnNode = btnBoxVec.at(m);
                     if (btnNode->getChildByName("HAS_OPEN_BOX_TEXT") == nullptr)
                     {
                         playBoxEffect(btnNode,false,tempnOpenType,boxMoney);
                         break;
                     }
                }
    
            }
        }
        
    }
    
}
