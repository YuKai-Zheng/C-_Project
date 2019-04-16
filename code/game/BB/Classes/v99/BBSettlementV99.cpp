//
//  BBSettlementV99.cpp
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#include "BBSettlementV99.h"
#include "HNLobby/v99/PlatformResV99.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "Sound/HNAudioEngine.h"

namespace BB {
    
    enum {
        ZOrder_Color_Layer = 10,
        ZOrder_Animation_Layer = 12,
        ZOrder_Kuang_Layer = 13,
        ZOrder_Content_Layer = 15,
    };
    
    static const char* Settlement_CSB = "Games/31000501/Node_Settlement.csb";
    const static char* Open_Logo_Settlement_Res = "Games/31000501/BB/CHEBIAO-BIG/icon_%d.png";
    
    BBSettlementV99::BBSettlementV99() {
        
    }
    
    BBSettlementV99::~BBSettlementV99() {
        
    }
    
    bool BBSettlementV99::init() {
        if (!Layer::init()) {
            return false;
        }
        initValues();
        initSettleLayer();
        return true;
    }
    
    void BBSettlementV99::setSettlementLayerPosition(cocos2d::Vec2 pos) {
        if (_root.settlementNode) {
            _root.settlementNode->setPosition(pos);
        }
    }
    
    void BBSettlementV99::updateOpenLogo(int openLogo) {
        if (openLogo < 0 || openLogo >= Loop_Sprite_Count) {
            return;
        }
        int logo = g_Logos[openLogo];
        int mul = g_iBetRate[logo];
        int resIndex = logo < 4 ? logo : LoopK_A_BetA_Count - logo - 1;    //0123,3210
        if (_root.openLogo) {
            char path[100] = {0};
            sprintf(path, Open_Logo_Settlement_Res, resIndex);
            _root.openLogo->loadTexture(path);
        }
        if (_root.openLogoMul) {
            ostringstream ss;
            ss << mul << "倍";
            _root.openLogoMul->setString(ss.str());
        }
        if (_root.logoMulImg) {
            _root.logoMulImg->setVisible(true);
            ostringstream ss;
            ss << "Games/31000501/BB/beishu/b_" << mul << ".png";
            _root.logoMulImg->loadTexture(ss.str());
        }
    }
    
    void BBSettlementV99::updateMyName(char *name) {
        if (_root.Self_name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _root.Self_name->setString(ForceGBKToUtf8(name));
#else
            _root.Self_name->setString(GBKToUtf8(name));
#endif
        }
    }
    
    void BBSettlementV99::updateBankerName(char *name) {
        if (_root.Banker_name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _root.Banker_name->setString(ForceGBKToUtf8(name));
#else
            _root.Banker_name->setString(GBKToUtf8(name));
#endif
        }
    }
    
    void BBSettlementV99::updateSettlementLayer(long long winMoney) {
        if (winMoney >= 0) {
            HNAudioEngine::getInstance()->playEffect(GAME_WIN_AUDIO_V99);
            if (_root.loseNode) {
                _root.loseNode->stopAllActions();
                _root.loseAction = nullptr;
                _root.loseNode->setVisible(false);
            }
            if (_root.winNode) {
                _root.winNode->setVisible(true);
                if (_root.winAction == nullptr) {
                    _root.winAction = CSLoader::createTimeline(WIN_ANIMATION_CSB_V99);
                }
                _root.winAction->play("victory", false);
                _root.winNode->runAction(_root.winAction);
            }
        }
        else {
            HNAudioEngine::getInstance()->playEffect(GAME_LOSE_AUDIO_V99);
            if (_root.winNode) {
                _root.winNode->stopAllActions();
                _root.winAction = nullptr;
                _root.winNode->setVisible(false);
            }
            if (_root.loseNode) {
                _root.loseNode->setVisible(true);
                if (_root.loseAction == nullptr) {
                    _root.loseAction = CSLoader::createTimeline(LOSE_ANIMATION_CSB_V99);
                }
                _root.loseAction->play("failure", false);
                _root.loseNode->runAction(_root.loseAction);
            }
        }
        //
        if (_root.Self_winState) {
            char path[100] = {0};
            sprintf(path, SETTLEMENT_WIN_LABEL_V99, winMoney >= 0 ? 9 : 10);
            _root.Self_winState->loadTexture(path);
        }
        if (_root.Self_settlement) {
            ostringstream ss;
            ss << (winMoney >= 0 ? "+" : "") << Tools::addComma(winMoney);
            _root.Self_settlement->setString(ss.str().c_str());
        }
        this->setVisible(true);
    }
    
    void BBSettlementV99::updateBankerInfo(long long bankerWinMoney) {
        if (_root.Banker_settlement) {
            ostringstream ss;
            ss << (bankerWinMoney >= 0 ? "+" : "") << Tools::addComma(bankerWinMoney);
            _root.Banker_settlement->setString(ss.str().c_str());
        }
        if (_root.Banker_winState) {
            char path[100] = {0};
            sprintf(path, SETTLEMENT_WIN_LABEL_V99, bankerWinMoney >= 0 ? 9 : 10);
            _root.Banker_winState->loadTexture(path);
        }
    }
    
    void BBSettlementV99::hideSettlementLayer() {
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
    
    void BBSettlementV99::initValues() {
        
    }
    
    void BBSettlementV99::initSettleLayer() {
        //
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
        if (colorLayer) {
            colorLayer->setContentSize(WIN_SIZE);
            this->addChild(colorLayer, ZOrder_Color_Layer);
        }
        //
        Vec2 orgPos = Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.4f);
        ImageView* kuang = ImageView::create(SETTLEMENT_BG_PATH_V99);
        if (kuang) {
            _root.bg_kuang = kuang;
            kuang->setPosition(orgPos);
            addChild(kuang, ZOrder_Kuang_Layer);
        }
        //
        Vec2 pos = orgPos;
        if (kuang) {
            pos += Vec2(0, kuang->getContentSize().height * 0.5f);
        }
        else {
            pos += Vec2(0, 202.5f);
        }
        auto winNode = CSLoader::createNode(WIN_ANIMATION_CSB_V99);
        if (winNode) {
            winNode->setPosition(pos);
            winNode->setVisible(false);
            _root.winNode = winNode;
            addChild(_root.winNode, ZOrder_Animation_Layer);
            auto winAction = CSLoader::createTimeline(WIN_ANIMATION_CSB_V99);
            if (winAction) {
                _root.winAction = winAction;
                _root.winNode->runAction(winAction);
            }
        }
        auto loseNode = CSLoader::createNode(LOSE_ANIMATION_CSB_V99);
        if (loseNode) {
            loseNode->setPosition(pos);
            loseNode->setVisible(false);
            _root.loseNode = loseNode;
            addChild(_root.loseNode, ZOrder_Animation_Layer);
            auto loseAction = CSLoader::createTimeline(LOSE_ANIMATION_CSB_V99);
            if (loseAction) {
                _root.loseAction = loseAction;
                _root.loseNode->runAction(loseAction);
            }
        }
        //
        auto root = CSLoader::createNode(Settlement_CSB);
        if (root) {
            root->setPosition(pos.x, WIN_SIZE.height * 0.5f);
            _root.settlementNode = root;
            addChild(root, ZOrder_Content_Layer);
            //
            auto logo = dynamic_cast<ImageView*>(root->getChildByName("Image_Logo"));
            if (logo) {
                _root.openLogo = logo;
                auto mul = dynamic_cast<Text*>(logo->getChildByName("Text_Mul"));
                if (mul) {
                    mul->setVisible(false);
                    _root.openLogoMul = mul;
                    mul->setString("");
                }
                auto mulImg = dynamic_cast<ImageView*>(logo->getChildByName("Image_Mul"));
                if (mulImg) {
                    mulImg->setVisible(false);
                    _root.logoMulImg = mulImg;
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
                auto state = dynamic_cast<ImageView*>(self->getChildByName("winState"));
                if (state) {
//                    state->setVisible(false);
                    _root.Self_winState = state;
                }
                auto name = dynamic_cast<Text*>(self->getChildByName("name"));
                if (name) {
                    name->setString("");
                    _root.Self_name = name;
                }
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
                auto state = dynamic_cast<ImageView*>(banker->getChildByName("winState"));
                if (state) {
//                    state->setVisible(false);
                    _root.Banker_winState = state;
                }
                auto name = dynamic_cast<Text*>(banker->getChildByName("name"));
                if (name) {
                    name->setString("");
                    _root.Banker_name = name;
                }
                auto money = dynamic_cast<Text*>(banker->getChildByName("settlement"));
                if (money) {
                    money->setString("");
                    _root.Banker_settlement = money;
                }
            }
        }
    }
    
}
