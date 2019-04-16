//
//  JSXSMary.cpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/16.
//

#include "JSXSMary.hpp"
#include "cocostudio/CocoStudio.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "Tool/Tools.h"
#include "Sound/HNAudioEngine.h"
namespace JSXS {
    const static char* JSXS_MARY_START = "Games/31000508/music/smallGameStart.mp3";
    const static char* JSXS_MARY_BGM   = "Games/31000508/music/smallGameBgm.mp3";
    const static char* JSXS_MARY_YELLOW = "Games/31000508/music/huangse.mp3";
    const static char* JSXS_MARY_LAN = "Games/31000508/music/lanlvse.mp3";
    const static char* JSXS_MARY_SCORE = "Games/31000508/music/smallGameScore.mp3";
    const static char* JSXS_MARY_SUCCESS = "Games/31000508/music/xiangfuchenggong.mp3";
    const static char* JSXS_MARY_LOSE = "Games/31000508/music/lose.mp3";
    JSXSMary::JSXSMary()
    {
        _getReward = nullptr;
        btnBoxVec.clear();
        curSelectBox   = nullptr;
        
        maryNode       = nullptr;
        m_panelStart   = nullptr;
        m_panelGame    = nullptr;
        m_panelEnd     = nullptr;
        m_panelBlack   = nullptr;
        m_endBottom    = nullptr;
        m_rewardBottom = nullptr;
        
        endGouYu.clear();
        rewardGouYu.clear();
        
        nMaryCount   = 0;            //剩余打开次数
        nOpenType    = 0;            //本次打开的类型
        i64OpenMoney = 0;            //本次打开的金额
        i64AllMoney  = 0;            //累计的总金额
        
        isTouch      = false;
        
        i64OpenMoney_end = 0;            //宝箱打开的总金额
        i64NoteMoney_end = 0;            //玩家本局的总押注
        nLineCount_end   = 0;
        i64AllMoney_end  = 0;            //小游戏最后总奖金
        
        myEndFunc  = NULL;
        myOpenFunc = NULL;
        
        exitGouYuCount   = 0;
        rewardGouYuCount = 0;
        
        vecFzHasOpen.clear();
        memset(nOpenTypes,0,sizeof(nOpenTypes));
        memset(i64BoxMoney,0,sizeof(i64BoxMoney));
        
        isMaryEnd = false;
    }
    JSXSMary::~JSXSMary()
    {
        
    }
    bool JSXSMary::init()
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
        HNAudioEngine::getInstance()->playBackgroundMusic(JSXS_MARY_START);
        maryNode = CSLoader::createNode("Games/31000508/Node_jsxsMary.csb");
        if (maryNode) {
            maryNode->setPosition(WIN_SIZE/2);
            addChild(maryNode,1);
            
            
            auto panel_game = dynamic_cast<Layout*>(maryNode->getChildByName("Panel_game"));
            if (panel_game) {
                m_panelGame = panel_game;
                m_panelGame->setVisible(false);
               
            }
            
            auto panel_black = dynamic_cast<Layout*>(maryNode->getChildByName("Panel_black"));
            if (panel_black) {
                m_panelBlack = panel_black;
                m_panelBlack->setVisible(false);
            }
            
            auto panel_end = dynamic_cast<Layout*>(maryNode->getChildByName("Panel_end"));
            if (panel_end) {
                m_panelEnd = panel_end;
                m_panelEnd->setVisible(false);
            }
            
            auto panel_start = dynamic_cast<Layout*>(maryNode->getChildByName("Panel_start"));
            if (panel_start) {
                m_panelStart = panel_start;
                m_panelStart->setVisible(true);
                auto btnStart = dynamic_cast<Button*>(panel_start->getChildByName("Button_1"));
                if (btnStart) {
                    btnStart->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
                        if (type != Widget::TouchEventType::ENDED) {
                            return;
                        }
                        
                        this->createBoxUI();
                        m_panelStart->removeFromParent();
                    
                    });
                }
                
                auto jsonPath      = "Games/31000508/animation/jsw/jiangshiwang.json";
                auto atlasPath     = "Games/31000508/animation/jsw/jiangshiwang.atlas";
                
                auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                if (skeletonNode) {
                    skeletonNode->setPosition(Vec2(360,-100));
                    skeletonNode->setAnimation(0,"jiangshiwang",true);
                    m_panelStart->addChild(skeletonNode);
                    skeletonNode->setScale(0.7f);
                }
                
                
            }
            
        }
        
        
        return true;
    }
    void JSXSMary::createBoxUI()
    {
        HNAudioEngine::getInstance()->stopBackgroundMusic();
        HNAudioEngine::getInstance()->playBackgroundMusic(JSXS_MARY_BGM);
        if (m_panelGame == nullptr) {
            return;
        }
        m_panelGame->setVisible(true);
        
        for (int i = 1; i <= 15; i++) {
            std::string btnName = StringUtils::format("Button_%d",i);
            auto btn            = dynamic_cast<Button*>(m_panelGame->getChildByName(btnName));
            if (btn) {
                btn->addTouchEventListener(CC_CALLBACK_2(JSXSMary::openBoxCallBack,this));
                btnBoxVec.push_back(btn);
            }
        }
        
        vector<int> team1 = {1,5,9};
        vector<int> team2 = {2,6,10,13};
        vector<int> team3 = {3,7,11,14};
        vector<int> team4 = {4,8,12,15};
        
        vector<vector<int>> vecTeam;
        vecTeam.push_back(team1);
        vecTeam.push_back(team2);
        vecTeam.push_back(team3);
        vecTeam.push_back(team4);
        
        for (int i = 0; i < vecTeam.size(); i++) {
            auto team = vecTeam.at(i);
            for (int j = 0; j < team.size(); j++) {
                auto btnFz = btnBoxVec.at(team.at(j) - 1);
                btnFz->runAction(Sequence::create(DelayTime::create(i * 0.3f),ScaleTo::create(0.2f,1.5f),ScaleTo::create(0.2f,0.8f), NULL));
            }
        }
        
        
        auto jsonPath      = "Games/31000508/animation/jsw/jiangshiwang.json";
        auto atlasPath     = "Games/31000508/animation/jsw/jiangshiwang.atlas";
        
        auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
        if (skeletonNode) {
            skeletonNode->setPosition(Vec2(-100,-1100));
            skeletonNode->setAnimation(0,"jiangshiwang",true);
            m_panelGame->addChild(skeletonNode,-1);
            
            skeletonNode->runAction(MoveBy::create(0.5f,Vec2(0,700)));
        }
        
        
        auto img = dynamic_cast<ImageView*>(m_panelGame->getChildByName("Image_middle"));
        if (img) {
            _getReward = dynamic_cast<Text*>(img->getChildByName("Text_score"));
            _getReward->setString("0");
        }
        
        auto imgEnd = dynamic_cast<ImageView*>(m_panelGame->getChildByName("Image_end"));
        if (imgEnd) {
            m_endBottom = imgEnd;
            for (int i = 1; i <= 3; i++) {
                std::string gyName = StringUtils::format("Image_gy%d",i);
                auto gyImg         = dynamic_cast<ImageView*>(imgEnd->getChildByName(gyName));
                if (gyImg) {
                    endGouYu.push_back(gyImg);
                }
            }
            
        }
        
        auto imgReward = dynamic_cast<ImageView*>(m_panelGame->getChildByName("Image_reward"));
        if (imgReward) {
            m_rewardBottom = imgReward;
            for (int i = 1; i <= 3; i++) {
                std::string gyName = StringUtils::format("Image_gy%d",i);
                auto gyImg         = dynamic_cast<ImageView*>(imgReward->getChildByName(gyName));
                if (gyImg) {
                    rewardGouYu.push_back(gyImg);
                }
            }
            
        }
        
    }
    void JSXSMary::playYuanbao()
    {
       
    }
    void JSXSMary::playBoxEffect(Button* btnNode,bool realOpen,int showType,long long showMoney)
    {
        if (btnNode) {
            btnNode->setTouchEnabled(false);
            btnNode->setBright(false);
            auto btnName = btnNode->getName();
            auto btnType = btnName.substr(7);
            
            int _type    = atoi(btnType.c_str());
            
            std::string fontPngPath = realOpen ? "v66/fontpng/jiesuan2.png":"v66/fontpng/jiesuan3.png";
            auto m_moneyText = TextAtlas::create("", fontPngPath, 72, 96, "+");
            m_moneyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_moneyText->setPosition(btnNode->getPosition());
            btnNode->getParent()->addChild(m_moneyText,2);
            m_moneyText->setScale(0.5f);
            m_moneyText->setName("HAS_OPEN_BOX_TEXT");
            
            char moneyValue[32];
            sprintf(moneyValue, "%s", Tools::addComma(showMoney).c_str());
            m_moneyText->setString(moneyValue);
            
            btnNode->setVisible(showType == OT_MONEY);
            std::string animationTypeEx;
            if (showType == OT_GREEN) {
                animationTypeEx = StringUtils::format("1%02d",_type);
                auto greenImg = ImageView::create("Games/31000508/special/036.png");
                if (greenImg) {
                    greenImg->setPosition(btnNode->getPosition());
                    btnNode->getParent()->addChild(greenImg);
                    greenImg->setRotation(-90.0f);
                }
            }
            else if (showType == OT_YELLOW)
            {
                animationTypeEx = StringUtils::format("2%02d",_type);
                auto yellowImg = ImageView::create("Games/31000508/special/gy.png");
                if (yellowImg) {
                    yellowImg->setPosition(btnNode->getPosition());
                    btnNode->getParent()->addChild(yellowImg);
                    yellowImg->setRotation(-90.0f);
                }
            }
            
            if (realOpen) {
                auto jsonPath      = "Games/31000508/animation/smallGame/jiangshi_gs.json";
                auto atlasPath     = "Games/31000508/animation/smallGame/jiangshi_gs.atlas";
                auto animationName = StringUtils::format("%03d",_type);
                auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                if (skeletonNode) {
                    skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 640,0));
                    skeletonNode->setAnimation(0, animationName,false);
                    skeletonNode->setEndListener([=](int trackIndex){
                        this->playYuanbao();
                        this->setMaryCountAndAllMoney(true);
                        this->showGameEndGouYuNum();
                        this->showGameRewardGouYuNum();
                        isTouch = false;
                        
                        if (isMaryEnd) {
                            this->showNotOpenFuZhou();
                            this->showMaryEnd();
                        }
                        
                        
                        skeletonNode->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
                    });
                    addChild(skeletonNode,1000);
                }
                
                if (!animationTypeEx.empty()) {
                    auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                    if (skeletonNode) {
                        skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 640,0));
                        skeletonNode->setAnimation(0, animationTypeEx,false);
                        skeletonNode->setEndListener([=](int trackIndex){
                            skeletonNode->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
                        });
                        addChild(skeletonNode,1000);
                    }
                }
            }
            
            
            
            
        }
        
    }
    void JSXSMary::openBoxCallBack(Ref* pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        
        if (isTouch) {
            return;
        }
        
        auto btnNode = (Button*)pSender;
        curSelectBox = btnNode;
        
        isTouch = true;
        auto btnName = btnNode->getName();
        auto btnType = btnName.substr(7);
        int _type    = atoi(btnType.c_str());
        
        vecFzHasOpen.push_back(_type - 1);
        if (myOpenFunc) {
            myOpenFunc(_type - 1);
        }
        
    }
    void JSXSMary::open(Node* parent, Vec2 vec, int zorder)
    {
        if (parent) {
            this->setPosition(vec);
            parent->addChild(this,zorder);
        }
    }
    void JSXSMary::close()
    {
        this->removeFromParent();
    }
    void JSXSMary::dealBoxOpenResult(struct_s2c_open_result* msg)
    {
        nOpenType    = msg->nOpenType;
        i64OpenMoney = msg->i64OpenMoney;
        i64AllMoney  = msg->i64AllMoney;
        
        if (nOpenType == OT_GREEN) {
            exitGouYuCount++;
            HNAudioEngine::getInstance()->playEffect(JSXS_MARY_LAN);
        }
        else if (nOpenType == OT_YELLOW)
        {
            rewardGouYuCount++;
            HNAudioEngine::getInstance()->playEffect(JSXS_MARY_YELLOW);
        }
        else if (nOpenType == OT_MONEY)
        {
            HNAudioEngine::getInstance()->playEffect(JSXS_MARY_SCORE);
        }
        
        if (curSelectBox) {
            playBoxEffect(curSelectBox,true,nOpenType,i64OpenMoney);
        }
        
        
    }
    void JSXSMary::setMaryCountAndAllMoney(bool show)
    {
        if (_getReward) {
            std::string numMoney = StringUtils::format("%lld",i64AllMoney);
            _getReward->setString(numMoney);
            if (show) {
                for (int i = 0; i < 2; i++) {
                    auto img = ImageView::create("Games/31000508/special/kuang.png");
                    img->setPosition(Vec2(_getReward->getContentSize().width/2,_getReward->getContentSize().height/2 + 20));
                    _getReward->addChild(img);
                    
                    img->setVisible(false);
                    img->runAction(Sequence::create(DelayTime::create((i+1)*0.3),CallFunc::create([=](){
                        img->setVisible(true);
                    }),ScaleTo::create(0.4f,1.3f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
                    
                }
            }
        }
       
    }
    void JSXSMary::dealMaryEnd(struct_s2c_mary_result* msg)
    {
        i64OpenMoney_end = msg->i64OpenMoney;
        i64NoteMoney_end = msg->i64NoteMoney;
        nLineCount_end   = msg->nLineCount;
        i64AllMoney_end  = msg->i64AllMoney;
        
        memcpy(nOpenTypes, msg->nOpenTypes, sizeof(nOpenTypes));
        memcpy(i64BoxMoney, msg->i64BoxMoney, sizeof(i64BoxMoney));
        
        isMaryEnd = true;
        
    }
    void JSXSMary::showMaryEnd()
    {
        if (m_panelBlack) {
            m_panelBlack->setVisible(true);
        }

        if (m_panelEnd) {
            m_panelEnd->setVisible(true);
            m_panelEnd->setScale(0.1f);
            m_panelEnd->runAction(ScaleTo::create(0.4f,1.0f));
            
            
            
            if (exitGouYuCount == 3 && rewardGouYuCount != 3) {
                auto titleEnd = dynamic_cast<ImageView*>(m_panelEnd->getChildByName("Image_69"));
                if (titleEnd)
                {
                    titleEnd->loadTexture("Games/31000508/special/sb.png");
                    titleEnd->ignoreContentAdaptWithSize(false);
                    
                }
                
                auto getEnd = dynamic_cast<ImageView*>(m_panelEnd->getChildByName("Image_70"));
                if (getEnd)
                {
                    getEnd->loadTexture("Games/31000508/special/jb2.png");
                    getEnd->ignoreContentAdaptWithSize(false);
                    
                }
                HNAudioEngine::getInstance()->playEffect(JSXS_MARY_LOSE);
            }
            else
            {
                HNAudioEngine::getInstance()->playEffect(JSXS_MARY_SUCCESS);
            }
           
            
            auto backBtn = dynamic_cast<Button*>(m_panelEnd->getChildByName("Button_end"));
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
                
            auto score = dynamic_cast<Text*>(m_panelEnd->getChildByName("Text_score"));
            if (score)
            {
                score->setString(StringUtils::format("%lld X %lld X %d = %lld",i64OpenMoney_end,i64NoteMoney_end,nLineCount_end,i64AllMoney_end));
            }
                
            auto m_moneyText = TextAtlas::create("", "v66/fontpng/jiesuan2.png", 72, 96, "+");
            m_moneyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_moneyText->setPosition(Vec2(800,312));
            m_panelEnd->addChild(m_moneyText);
            
            char moneyValue[32];
            sprintf(moneyValue, "%s", Tools::addComma(i64AllMoney_end).c_str());
            m_moneyText->setString(moneyValue);
            m_moneyText->setScale(0.70f);
            
        }
    }
    void JSXSMary::setMaryEndCallBack(EndCallback func)
    {
        myEndFunc = func;
        
    }
    void JSXSMary::setMaryMsgToServerCallBack(OpenCallback func)
    {
        myOpenFunc = func;
    }
    void JSXSMary::showGameEndGouYuNum()
    {
        if (nOpenType == OT_GREEN) {
            for (int i = 0; i < 2; i++) {
                auto img = ImageView::create("Games/31000508/special/kuang.png");
                img->setPosition(m_endBottom->getContentSize()/2);
                m_endBottom->addChild(img);
                
                img->setVisible(false);
                img->runAction(Sequence::create(DelayTime::create((i+1)*0.3),CallFunc::create([=](){
                    img->setVisible(true);
                }),ScaleTo::create(0.4f,1.3f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
                
            }
        }
        
        for (int i = 0; i < endGouYu.size(); i++) {
            auto imgGouYu = endGouYu.at(i);
            imgGouYu->setVisible(i <= exitGouYuCount - 1);
        }
    }
    void JSXSMary::showGameRewardGouYuNum()
    {
        if (nOpenType == OT_YELLOW) {
            for (int i = 0; i < 2; i++) {
                auto img = ImageView::create("Games/31000508/special/kuang.png");
                img->setPosition(m_rewardBottom->getContentSize()/2);
                m_rewardBottom->addChild(img);
                
                img->setVisible(false);
                img->runAction(Sequence::create(DelayTime::create((i+1)*0.3),CallFunc::create([=](){
                    img->setVisible(true);
                }),ScaleTo::create(0.4f,1.3f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
                
            }
        }
        for (int i = 0; i < rewardGouYu.size(); i++) {
            auto imgGouYu = rewardGouYu.at(i);
            imgGouYu->setVisible(i <= rewardGouYuCount - 1);
        }
    }
    void JSXSMary::showNotOpenFuZhou()
    {
        for (int i = 0; i < TREASURE_FZ_COUNT; i++) {
            long long boxMoney = i64BoxMoney[i];
            bool notOpen = true;
            for (int j = 0; j < vecFzHasOpen.size(); j++) {
                if (i == vecFzHasOpen.at(j)) {
                    notOpen = false;
                    break;
                }
            }
            if (notOpen) {
                int tempnOpenType = nOpenTypes[i];
                for (int m = 0; m < btnBoxVec.size(); m++) {
                    auto btnNode = btnBoxVec.at(m);
                    if (btnNode->isTouchEnabled())
                    {
                        playBoxEffect(btnNode,false,tempnOpenType,boxMoney);
                        break;
                    }
                }
                
            }
        }
        
    }
}
