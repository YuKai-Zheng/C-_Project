//
//  FruitGameStateV88.cpp
//  MixProject
//
//  Created by 黄百超 on 24/03/2017.
//
//

#include "FruitGameStateV88.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "HNCommon/HNConverCode.h"
#include "Sound/HNAudioEngine.h"

namespace Fruit {
    
    const static char* Node_CSB = "csb/Games/BB_V66/Node_GameState.csb";
    const static char* Node_State_Label_CSB = "csb/Games/BB_V66/Node_StateLabel.csb";

    
	FruitGameStateV88::FruitGameStateV88() {
        
    }
    
	FruitGameStateV88::~FruitGameStateV88() {
        
    }
    
	bool FruitGameStateV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initValues();
        initStateLayer();
        return true;
    }
    
    void FruitGameStateV88::updateGirlState(int state) {
        hideStateLayer();
    }
    
    void FruitGameStateV88::updateGameStateContent(int stateID) {
        hideStateLayer();
       
    }
    
    void FruitGameStateV88::hideStateLayer() {
        if (_stateGirl.stateNode) {
            _stateGirl.stateNode->setVisible(false);
        }
        if (_stateContent.stateLabelNode) {
            _stateContent.stateLabelNode->setVisible(false);
            _stateContent.stateContent->setVisible(false);
            _stateContent.stateLabelContent->setVisible(false);
        }
    }
    
    
    void FruitGameStateV88::setStateGirlPosition(cocos2d::Vec2 pos) {
        if (_stateGirl.stateNode) {
            _stateGirl.stateNode->setPosition(pos);
        }
    }
    
    void FruitGameStateV88::setStateLabelPosition(cocos2d::Vec2 pos) {
        if (_stateContent.stateLabelNode) {
            _stateContent.stateLabelNode->setPosition(pos);
        }
    }
    
    void FruitGameStateV88::initValues() {
        _gameState = GS_FREE_NOTE;
    }
    
    void FruitGameStateV88::initStateLayer() {
        auto node = CSLoader::createNode(Node_CSB);
        if (node) {
            _stateGirl.stateNode = node;
            //
            auto girlBg = dynamic_cast<ImageView*>(node->getChildByName("Image_Girl_Bg"));
            if (girlBg) {
                _stateGirl.girlBg = girlBg;
                //
                auto stateBg = dynamic_cast<ImageView*>(girlBg->getChildByName("Image_State_Bg"));
                if (stateBg) {
                    _stateGirl.stateBg = stateBg;
                    //
                    auto stateImg = dynamic_cast<ImageView*>(stateBg->getChildByName("Image_State"));
                    if (stateImg) {
                        stateImg->setVisible(false);
                        _stateGirl.state = stateImg;
                    }
                }
            }
            addChild(node);
            node->setVisible(false);
        }
        //
        auto nodeContent = CSLoader::createNode(Node_State_Label_CSB);
        if (nodeContent) {
            _stateContent.stateLabelNode = nodeContent;
            auto bg = dynamic_cast<ImageView*>(nodeContent->getChildByName("Image_State"));
            if (bg) {
                _stateContent.bg = bg;
                //
                auto contentImg = dynamic_cast<ImageView*>(bg->getChildByName("state"));
                if (contentImg) {
                    contentImg->setVisible(false);
                    _stateContent.stateLabelContent = contentImg;
                }
                //
                auto stateText = dynamic_cast<Text*>(bg->getChildByName("text"));
                if (stateText) {
                    stateText->setVisible(false);
                    _stateContent.stateContent = stateText;
                }
            }
            addChild(nodeContent);
            nodeContent->setVisible(false);
        }
    }
    
//    void FruitGameStateV88::showStateLayer() {
//        
//    }
    
}
