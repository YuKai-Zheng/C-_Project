//
//  BBGameStateV88.cpp
//  MixProject
//
//  Created by 黄百超 on 24/03/2017.
//
//

#include "BBGameStateV88.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "HNCommon/HNConverCode.h"
#include "Sound/HNAudioEngine.h"


namespace BB {
    
    const static char* Node_CSB = "csb/Games/BB_V66/Node_GameState.csb";
    const static char* Node_State_Label_CSB = "csb/Games/BB_V66/Node_StateLabel.csb";
    const static char* State_Img = "v66/BB/%d.png";


	const static char* Note_Begin_Audio = "v66/music/BJL/tipBet.mp3";
	const static char* Note_End_Audio = "v66/music/BJL/tipStopBet.mp3";
    
    BBGameStateV88::BBGameStateV88() {
        
    }
    
    BBGameStateV88::~BBGameStateV88() {
        
    }
    
    bool BBGameStateV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initValues();
        initStateLayer();
        return true;
    }
    
	void BBGameStateV88::updateGirlState(int state) {
        hideStateLayer();
        if (state == GS_XIAZHU || state == GS_XIAZHU_FINISH) {
            //开始下注、结束下注
            if (_stateGirl.stateNode) {
                char stateTexture[100] = {0};
                int index = 13;
                switch (state) {
                    case GS_XIAZHU:
                        index = 6;
						HNAudioEngine::getInstance()->playEffect(Note_Begin_Audio);
                        break;
                    case GS_XIAZHU_FINISH:
                        index = 5;
						HNAudioEngine::getInstance()->playEffect(Note_End_Audio);
                        break;
                    default:
                        break;
                }
                sprintf(stateTexture, State_Img, index);
                _stateGirl.stateNode->setVisible(true);
                if (_stateGirl.stateBg) {
					//_stateGirl.state->setVisible(true);
					_stateGirl.stateBg->loadTexture(stateTexture);
                }
            }
        }
//        else {
//            //庄家等信息的提示
//            if (_stateContent.stateLabelNode) {
//                char imgPath[100] = {0};
//                int index = 20;
//                switch (state) {
////                    case :
////                        
////                        break;
//                        
//                    default:
//                        break;
//                }
//                sprintf(imgPath, State_Img, index);
//                _stateContent.stateLabelNode->setVisible(true);
//                if (_stateContent.stateLabelContent) {
//                    _stateContent.stateLabelContent->loadTexture(imgPath);
//                }
//                auto move = MoveBy::create(1.0f, Vec2(0, 20));
//                _stateContent.stateLabelNode->runAction(Sequence::create(move, Hide::create(), move->reverse(), nullptr));
//            }
//        }
    }


	void BBGameStateV88::updateGameStateContent(int stateID) {
		hideStateLayer();
		if (_stateContent.stateLabelNode) {
			_stateContent.stateLabelNode->setVisible(true);
			//
			if (stateID <= GSTATE_BE_BANKER_SUCCEED) {
				//文字
				_stateContent.stateLabelContent->setVisible(false);
				_stateContent.stateContent->setVisible(true);
				std::string nameTag = "";
				switch (stateID) {
				case GSTATE_XIAZHU_FAIL_BANKER_LACK_MONEY:
					nameTag = "BB_XIAZHU_FAIL_BANKER";
					break;
				case GSTATE_XIAZHU_FAILE_LACK_MONEY:
					nameTag = "GSTATE_XIAZHU_FAILE_LACK_MONEY";
					break;
				case GSTATE_XIAZHU_FAILE_OVER:
					nameTag = "BB_XIAZHU_FAILE_OVER";
					break;
					//                    case GSTATE_XIAZHU_SUCCEED:
					//                        nameTag = "BB_XIAZHU_SUCCEED";
					//                        break;
				case GSTATE_SHANGZHUANG_FAIL_LACK_MONEY:
					nameTag = "BB_SHANGZHUANG_FAIL_MONEY";
					break;
				case GSTATE_SHANGZHUANG_SUCCEED:
					nameTag = "BB_SHANGZHUANG_SUCCEED";
					break;
				case GSTATE_BE_BANKER_SUCCEED:
					nameTag = "BB_BE_BANKER_SUCCEED";
					break;
				default:
					break;
				}
				if (!nameTag.empty()) {
					_stateContent.stateContent->setString(GetText(nameTag.c_str()));
				}
			}
			else {
				//图片
				_stateContent.stateLabelContent->setVisible(true);
				_stateContent.stateContent->setVisible(false);
				char imgPath[100] = { 0 };
				int index = 0;
				Size contentSize = Size(200, 48);
				switch (stateID) {
				case GSTATE_BE_BANKER_TWO:
					index = 21;
					break;
				case GSTATE_BE_BANKER_THREE:
					index = 22;
					break;
				case GSTATE_BE_BANKER_FOUR:
					index = 23;
					break;
				case GSTATE_BE_BANKER_FIVE:
					index = 24;
					break;
				case GSTATE_XIAZHUANG_OVER_FIVE:
					index = 20;
					contentSize = Size(205, 48);
					break;
				case GSTATE_XIAZHUANG_LACK_MONEY:
					index = 18;
					contentSize = Size(377, 48);
					break;
				default:
					contentSize = Size(200, 48);
					break;
				}
				sprintf(imgPath, State_Img, index);
				if (_stateContent.stateLabelContent) {
					_stateContent.stateLabelContent->loadTexture(imgPath);
					_stateContent.stateLabelContent->setContentSize(contentSize);
				}

			}
			auto move = MoveBy::create(1.0f, Vec2(0, 20));
			_stateContent.stateLabelNode->runAction(Sequence::create(move, Hide::create(), move->reverse(), nullptr));
		}

	}

    
    void BBGameStateV88::hideStateLayer() {
        if (_stateGirl.stateNode) {
            _stateGirl.stateNode->setVisible(false);
        }
        if (_stateContent.stateLabelNode) {
            _stateContent.stateLabelNode->setVisible(false);
			_stateContent.stateContent->setVisible(false);
			_stateContent.stateLabelContent->setVisible(false);
        }
    }
    
//    void BBGameStateV88::setNodeVisible(bool isShow) {
//        if (_stateGirl.stateNode) {
//            _stateGirl.stateNode->setVisible(isShow);
//        }
//        if (_stateContent.stateLabelNode) {
//            _stateContent.stateLabelNode->setVisible(isShow);
//        }
//    }
    
    void BBGameStateV88::setStateGirlPosition(cocos2d::Vec2 pos) {
        if (_stateGirl.stateNode) {
            _stateGirl.stateNode->setPosition(pos);
        }
    }
    
    void BBGameStateV88::setStateLabelPosition(cocos2d::Vec2 pos) {
        if (_stateContent.stateLabelNode) {
            _stateContent.stateLabelNode->setPosition(pos);
        }
    }
    
    void BBGameStateV88::initValues() {
        _gameState = GS_FREE;
    }
    
    void BBGameStateV88::initStateLayer() {
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
                  /*  auto stateImg = dynamic_cast<ImageView*>(stateBg->getChildByName("Image_State"));
                    if (stateImg) {
                        _stateGirl.state = stateImg;
                    }*/
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
                _stateContent.stateLabel = bg;
                //
                auto contentImg = dynamic_cast<ImageView*>(bg->getChildByName("state"));
                if (contentImg) {
                    _stateContent.stateLabelContent = contentImg;
                }
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
    
//    void BBGameStateV88::showStateLayer() {
//        
//    }
    
}
