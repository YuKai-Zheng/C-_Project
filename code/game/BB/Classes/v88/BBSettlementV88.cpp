//
//  BBSettlementV88.cpp
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#include "BBSettlementV88.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"

namespace BB {
    
    enum {
        ZOrder_Color_Layer = 10,
        ZOrder_Animation_Layer = 12,
        ZOrder_Kuang_Layer = 13,
        ZOrder_Content_Layer = 15,
		ZOrder_Logo_Layer = 16,
    };
    
    static const char* Settlement_CSB = "csb/Games/BB_V66/Node_Settlement.csb";
    const static char* Open_Logo_Res = "v66/BB/CHEBIAO-BIG/icon_%d.png";

	static const char* WIN_ANIMATION_CSB_V88 = "v88/animation/settle/shengli/shengli.csb";
	static const char* LOSE_ANIMATION_CSB_V88 = "v88/animation/settle/shibai/shibai.csb";
	static const char* SETTLEMENT_LOST_BG_PATH_V88 = "v66/common/jiesuan/loseBg.png";
	static const char* SETTLEMENT_WING_BG_PATH_V88 = "v66/common/jiesuan/winBg.png";
	
    
    BBSettlementV88::BBSettlementV88() {
        
    }
    
    BBSettlementV88::~BBSettlementV88() {
        
    }
    
    bool BBSettlementV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initValues();
        initSettleLayer();
        return true;
    }
    
    void BBSettlementV88::setSettlementLayerPosition(cocos2d::Vec2 pos) {
        if (_root.settlementNode) {
            _root.settlementNode->setPosition(pos);
        }
    }
    
    void BBSettlementV88::updateOpenLogo(int openLogo) {
		if (openLogo < 0 || openLogo >= Loop_Sprite_Count) {
			return;
		}
		int logo = g_Logos[openLogo];
		int mul = g_iBetRate[logo];
		int resIndex = logo < 4 ? logo : LoopK_A_BetA_Count - logo - 1;    //0123,3210
		if (_root.openLogo) {
			char path[100] = { 0 };
			sprintf(path, Open_Logo_Res, resIndex);
			_root.openLogo->loadTexture(path);
		}
		char str[64];
		if (_root.openLogoMul) {
			_root.openLogoMul->setVisible(true);
			sprintf(str, "×%d", mul);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			_root.openLogoMul->setString(ForceGBKToUtf8(str));
#else
			_root.openLogoMul->setString(GBKToUtf8(str));
#endif
		}
    }
    
    void BBSettlementV88::updateMyName(char *name) {
        if (_root.Self_name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _root.Self_name->setString(ForceGBKToUtf8(name));
#else
            _root.Self_name->setString(GBKToUtf8(name));
#endif
        }
    }
    
    void BBSettlementV88::updateSettlementLayer(long long winMoney) {
        if (winMoney >= 0) {
//            _root.bg_kuang->loadTexture(SETTLEMENT_WING_BG_PATH_V88);
            if (_root.loseNode) {
                _root.loseNode->stopAllActions();
                _root.loseAction = nullptr;
                _root.loseNode->setVisible(false);
				
		
            }
            if (_root.winNode) {
                _root.winNode->setVisible(true);
                if (_root.winAction == nullptr) {
                    _root.winAction = CSLoader::createTimeline(WIN_ANIMATION_CSB_V88);
                }
                _root.winAction->play("shengli", false);
                _root.winNode->runAction(_root.winAction);
            }
        }
        else {
//            _root.bg_kuang->loadTexture(SETTLEMENT_LOST_BG_PATH_V88);
            if (_root.winNode) {
                _root.winNode->stopAllActions();
                _root.winAction = nullptr;
                _root.winNode->setVisible(false);
            }
            if (_root.loseNode) {
                _root.loseNode->setVisible(true);
                if (_root.loseAction == nullptr) {
                    _root.loseAction = CSLoader::createTimeline(LOSE_ANIMATION_CSB_V88);
                }
                _root.loseAction->play("shibai", false);
                _root.loseNode->runAction(_root.loseAction);
            }
        }
        //
        if (_root.Self_settlement) {
            ostringstream ss;
            ss << (winMoney >= 0 ? "+" : "") << winMoney;
            _root.Self_settlement->setString(ss.str().c_str());
        }
        this->setVisible(true);
    }
    
    void BBSettlementV88::hideSettlementLayer() {
        this->setVisible(false);
        if (_root.winNode) {
            _root.winNode->stopAllActions();
            _root.winAction = nullptr;
            _root.winNode->setVisible(false);
        }
        if (_root.loseNode) {
            _root.loseNode->stopAllActions();
            _root.loseAction = nullptr;
            _root.loseNode->setVisible(false);
        }
    }
    
    void BBSettlementV88::initValues() {
        
    }
    
    void BBSettlementV88::initSettleLayer() {
        //
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
        if (colorLayer) {
            colorLayer->setContentSize(WIN_SIZE);
            this->addChild(colorLayer, ZOrder_Color_Layer);
        }
        //
//        Vec2 orgPos = Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.5f);
//        ImageView* kuang = ImageView::create(SETTLEMENT_LOST_BG_PATH_V88);
//        if (kuang) {
//            _root.bg_kuang = kuang;
//            kuang->setPosition(orgPos);
//            addChild(kuang, ZOrder_Kuang_Layer);
//        }
        //
        Vec2 pos = Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.5f);
//        if (kuang) {
//            pos += Vec2(0, kuang->getContentSize().height / 2);
//        }
//        else {
//            pos += Vec2(0, 202.5f);
//        }
        auto winNode = CSLoader::createNode(WIN_ANIMATION_CSB_V88);
        if (winNode) {
            winNode->setPosition(pos);
            winNode->setVisible(false);
            _root.winNode = winNode;
            addChild(_root.winNode, ZOrder_Animation_Layer);
            auto winAction = CSLoader::createTimeline(WIN_ANIMATION_CSB_V88);
            if (winAction) {
                _root.winAction = winAction;
//                winAction->play("victory", false);
                _root.winNode->runAction(winAction);
            }
        }
        auto loseNode = CSLoader::createNode(LOSE_ANIMATION_CSB_V88);
        if (loseNode) {
			loseNode->setPosition(pos);
            loseNode->setVisible(false);
            _root.loseNode = loseNode;
            addChild(_root.loseNode, ZOrder_Animation_Layer);
            auto loseAction = CSLoader::createTimeline(LOSE_ANIMATION_CSB_V88);
            if (loseAction) {
                _root.loseAction = loseAction;
//                loseAction->play("failure", false);
                _root.loseNode->runAction(loseAction);
            }
        }
        //
        auto root = CSLoader::createNode(Settlement_CSB);
        if (root) {
            //root->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//            pos -= Vec2(0, WIN_SIZE.height * 0.1f);
            root->setPosition(pos+Vec2(0, -20));
            _root.settlementNode = root;
            addChild(root, ZOrder_Animation_Layer-1);
            //
            auto logo = dynamic_cast<ImageView*>(root->getChildByName("Image_Logo"));
            if (logo) {
                _root.openLogo = logo;
                auto mul = dynamic_cast<Text*>(logo->getChildByName("Text_Mul"));
                if (mul) {
                    _root.openLogoMul = mul;
                    mul->setString("");
                }
            }
            //
            auto self = dynamic_cast<ImageView*>(root->getChildByName("Image_Self"));
            if (self) {
                _root.Self_Img = self;
                auto icon = dynamic_cast<ImageView*>(self->getChildByName("self"));
                if (icon) {
                    icon->setVisible(false);
                    _root.Self_Label = icon;
                }
//                auto state = dynamic_cast<ImageView*>(self->getChildByName("winState"));
//                if (state) {
////                    state->setVisible(false);
//                    _root.Self_winState = state;
//                }
//                auto name = dynamic_cast<Text*>(self->getChildByName("name"));
//                if (name) {
//                    name->setString("");
//                    _root.Self_name = name;
//                }
                auto money = dynamic_cast<Text*>(self->getChildByName("settlement"));
                if (money) {
                    money->setString("");
                    _root.Self_settlement = money;
                }
            }
            //
            auto banker = dynamic_cast<ImageView*>(root->getChildByName("Image_Banker"));
            if (banker) {
                _root.Banker_Img = banker;
                auto icon = dynamic_cast<ImageView*>(banker->getChildByName("banker"));
                if (icon) {
//                    icon->setVisible(false);
                    _root.Banker_Label = icon;
                }
                auto money = dynamic_cast<Text*>(banker->getChildByName("settlement"));
                if (money) {
                    money->setString("");
                    _root.Banker_settlement = money;
                }
            }
        }
    }


	void BBSettlementV88::updateBankerInfo(long long bankerWinMoney) {
		if (_root.Banker_settlement) {
			ostringstream ss;
			ss << (bankerWinMoney >= 0 ? "+" : "") << bankerWinMoney;
			_root.Banker_settlement->setString(ss.str().c_str());
		}
	}

	void BBSettlementV88::updateBankerName(char *name) {
		if (_root.Banker_name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			_root.Banker_name->setString(ForceGBKToUtf8(name));
#else
			_root.Banker_name->setString(GBKToUtf8(name));
#endif
		}
	}
    
}
