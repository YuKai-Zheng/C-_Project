//
//  BBHelpLayerV99.cpp
//  MixProject
//
//  Created by 黄百超 on 06/04/2017.
//
//

#include "BBHelpLayerV99.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
//#include "../../TTNN/Classes/GameTable/BRNNRes.h"
#include "../../../game/MixProject/Classes/CommonRes.h"
#include "Sound/HNAudioEngine.h"

namespace BB {
    
    const static char* Help_Res = "v99/common/help/BBshuoming.png";
    
    BBHelpLayerV99::BBHelpLayerV99() {
    }
    
    BBHelpLayerV99::~BBHelpLayerV99() {
    }
    
    bool BBHelpLayerV99::init() {
        if (!Layer::init()) {
            return false;
        }
        this->setIgnoreAnchorPointForPosition(false);
        
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(BBHelpLayerV99::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(BBHelpLayerV99::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        auto helpBg = Sprite::create(Help_Res);
        if (helpBg) {
            helpBg->setPosition(Vec2(helpBg->getContentSize().width/2,helpBg->getContentSize().height/2));
            addChild(helpBg);
            this->setContentSize(helpBg->getContentSize());
        }
        
        return true;
    }
    
    bool BBHelpLayerV99::onTouchBegan(Touch *touch, Event *unused_event) {
        Point point = touch->getLocation();
        if (!this->getBoundingBox().containsPoint(point)) {
            this->moveSelfWithAction();
        }
        return true;
    }
    
    void BBHelpLayerV99::onTouchEnded(Touch *touch, Event *unused_event) {
    }
    
    void BBHelpLayerV99::moveSelfWithAction() {
        HN::HNAudioEngine::getInstance()->playEffect("platform/sound/sound_close.mp3");
        //BRNN::Actions::actionOut(this);
    }
    
    Vec2 BBHelpLayerV99::convertGL(float x, float y) {
        return Director::getInstance()->convertToGL(Vec2(x, y));
    }
    
}
