//
//  RedPacketOpenPacketLayer.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#include "RedPacketOpenPacketLayer.h"

#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "Tool/Tools.h"
#include "cocostudio/CocoStudio.h"

namespace RedPacket {
    
    RedPacketOpenPacketLayer::RedPacketOpenPacketLayer() {
        
    }
    
    RedPacketOpenPacketLayer::~RedPacketOpenPacketLayer() {
        
    }
    
    bool RedPacketOpenPacketLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketOpenPacketLayer::setSenderHead(UserInfoStruct *user) {
        if (!user) {
            if (m_userHead)
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88);
            if (m_userName)
                m_userName->setString("");
            return;
        }
//        if (PlatformLogic()->loginResult.dwUserID == user->dwUserID) {
//            //这个红包是自己发送的
//            if (m_bg) {
//                m_bg->loadTexture("v88/fahongbao/hongbaokaiqi/4.png");
//            }
//            if (m_showDetailButton) {
//                m_showDetailButton->setVisible(true);
//            }
//            if (m_openButton) {
//                m_openButton->setVisible(false);
//            }
//        }
//        else
//        {
//            if (m_bg) {
//                m_bg->loadTexture("v88/fahongbao/hongbaokaiqi/2.png");
//            }
//            if (m_showDetailButton) {
//                m_showDetailButton->setVisible(false);
//            }
//            if (m_openButton) {
//                m_openButton->setVisible(true);
//            }
//        }
        if (m_userHead)
            m_userHead->setHeadWithInfo(user);
        if (m_userName)
            m_userName->setString(ForceGBKToUtf8(user->nickName));
    }
    
    void RedPacketOpenPacketLayer::openAddToParent(HNLayer *parent, cocos2d::Vec2 pos, bool isMe) {
        if (parent) {
            m_parentLayer = parent;
            this->setPosition(pos);
            parent->addChild(this);
            m_isMe = isMe;
        }
    }
    
    void RedPacketOpenPacketLayer::setOpenPacketCallBack(std::function<void ()> callBack) {
        m_openPacketCallBack = callBack;
    }
    
    void RedPacketOpenPacketLayer::setshowPacketDetailCallBack(std::function<void ()> callBack) {
        m_showPacketDetailCallBack = callBack;
    }
    
    void RedPacketOpenPacketLayer::setHasRobPacket(bool hasRob) {
        if (m_bg) {
            if (m_isMe) {
                m_bg->loadTexture("v88/fahongbao/hongbaokaiqi/4.png");
            }
            else {
                m_bg->loadTexture(hasRob == true ? "v88/fahongbao/hongbaokaiqi/4.png" : "v88/fahongbao/hongbaokaiqi/2.png");
            }
        }
        if (m_isMe || hasRob) {
            if (m_showDetailButton) {
                m_showDetailButton->setVisible(true);
            }
            if (m_openButton) {
                m_openButton->setVisible(false);
            }
            if (m_packetInfo) {
                if (m_myPacketInfo.nResult == 0 && m_myPacketInfo.i64Money > 0) {
                    //抢红包成功
                    m_packetInfo->setString(Tools::addComma(m_myPacketInfo.i64Money));
                    m_packetInfo->setVisible(true);
                }
                //                else {
                //                    char str[32];
                //                    memset(str, 0, sizeof(str));
                //                    sprintf(str, "OPEN_PACKET_ERROR_%d", m_myPacketInfo.nResult);
                //                    m_packetInfo->setString(GetText(str));
                //                }
            }
        }
        else {
            if (m_showDetailButton) {
                m_showDetailButton->setVisible(false);
            }
            if (m_openButton) {
                m_openButton->setVisible(true);
            }
            if (m_packetInfo) {
                m_packetInfo->setVisible(false);
            }
        }
    }
    
    void RedPacketOpenPacketLayer::setMyPacketInfo(RedPacket::tag_c2s_RobRedPacketReq *packetInfo) {
        m_myPacketInfo = *packetInfo;
    }
    
//    void RedPacketOpenPacketLayer::setShouldPlayAnimation(bool playAnimation) {
//        m_canPlayAnimation = playAnimation;
//    }
    
    void RedPacketOpenPacketLayer::resetData() {
        m_bg = nullptr;
        m_userHead = nullptr;
        m_userName = nullptr;
        m_packetInfo = nullptr;
        m_closeButton = nullptr;
        m_openButton = nullptr;
        m_showDetailButton = nullptr;
        touchListener = nullptr;
        m_parentLayer = nullptr;
        m_openPacketCallBack = nullptr;
        m_showPacketDetailCallBack = nullptr;
        m_isMe = false;
//        m_canPlayAnimation = false;
    }
    
    void RedPacketOpenPacketLayer::createUI() {
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
        if (colorLayer) {
            addChild(colorLayer);
        }
        //
        touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            return true;
        };
        touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
            closeLayer();
        };
        touchListener->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
        //
        m_bg = ImageView::create("v88/fahongbao/hongbaokaiqi/2.png");
        if (m_bg) {
            m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_bg->setPosition(WIN_SIZE/2);
            m_bg->setTouchEnabled(true);
            addChild(m_bg);
            //
            m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
            if (m_userHead) {
                m_userHead->setScale(0.85f);
                m_userHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_userHead->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.85f));
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "v88/module/gerenxinxi/5.png");
                m_bg->addChild(m_userHead);
                //
                m_userName = Text::create("我的名字是什么", FONT_TTF_MSYH, 30);
                if (m_userName) {
                    m_userName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    m_userName->setPosition(Vec2(m_userHead->getPositionX(), m_userHead->getPositionY() - m_userHead->getContentSize().height * 0.85f * 0.5f));
                    m_userName->setColor(Color3B(244, 211, 155));
                    m_bg->addChild(m_userName);
                }
            }
            //
            m_openButton = Button::create("v88/fahongbao/hongbaokaiqi/1.png", "v88/fahongbao/hongbaokaiqi/1.png");
            if (m_openButton) {
                m_openButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.37f));
                m_openButton->addTouchEventListener(CC_CALLBACK_2(RedPacketOpenPacketLayer::openButtonCallBack, this));
                m_bg->addChild(m_openButton);
                //
                m_packetInfo = Text::create("", FONT_TTF_MSYH, 50);
                if (m_packetInfo) {
                    m_packetInfo->setPosition(Vec2(m_openButton->getPositionX(), m_openButton->getPositionY() + m_openButton->getContentSize().height * 0.3f));
                    m_packetInfo->setColor(Color3B(244, 211, 155));
                    m_packetInfo->setVisible(false);
                    m_bg->addChild(m_packetInfo);
                }
            }
            m_closeButton = Button::create("v88/fahongbao/7.png", "v88/fahongbao/7.png");
            if (m_closeButton) {
                m_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                m_closeButton->setPosition(Vec2(0, m_bg->getContentSize().height));
                m_closeButton->addTouchEventListener(CC_CALLBACK_2(RedPacketOpenPacketLayer::backButtonCallBack, this));
                m_bg->addChild(m_closeButton);
            }
            m_showDetailButton = Button::create("v88/fahongbao/hongbaokaiqi/5.png", "v88/fahongbao/hongbaokaiqi/5.png");
            if (m_showDetailButton) {
                m_showDetailButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.14f));
                m_showDetailButton->addTouchEventListener(CC_CALLBACK_2(RedPacketOpenPacketLayer::showDetailButtonCallBack, this));
                m_showDetailButton->setVisible(false);
                m_bg->addChild(m_showDetailButton);
            }
        }
    }
    
    void RedPacketOpenPacketLayer::closeLayer() {
        this->removeFromParent();
    }
    
    void RedPacketOpenPacketLayer::backButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        closeLayer();
    }
    
    void RedPacketOpenPacketLayer::openButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        //拆红包请求
        //play open packet animation
        this->runAction(Sequence::create(CallFunc::create([=](){
            //play ainmation
            if (m_bg) {
                if (m_openButton) {
                    m_openButton->setVisible(false);
                    std::string path = "v88/animation/redbag/redbag_coin.csb";
                    auto node = CSLoader::createNode(path);
                    if (node) {
                        node->setName("Packet_Coin");
                        node->setPosition(m_openButton->getPosition());
                        m_bg->addChild(node);
                        auto action = CSLoader::createTimeline(path);
                        if (action) {
                            node->runAction(action);
                            action->play("redbag_coin", true);
                        }
                    }
                }
            }
        }), DelayTime::create(1.0f), CallFunc::create([=]() {
            if (m_openPacketCallBack) {
                m_openPacketCallBack();
            }
            if (m_bg) {
                auto node = m_bg->getChildByName("Packet_Coin");
                if (node) {
                    node->removeFromParent();
                }
            }
            closeLayer();
        }), nullptr));
    }
    
    void RedPacketOpenPacketLayer::showDetailButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        if (m_showPacketDetailCallBack) {
            m_showPacketDetailCallBack();
			closeLayer();
        }
    }
    
}
