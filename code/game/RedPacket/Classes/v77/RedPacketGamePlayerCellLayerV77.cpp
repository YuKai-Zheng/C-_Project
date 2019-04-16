//
//  RedPacketGamePlayerCellLayerV77.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 02/08/2017.
//
//

#include "RedPacketGamePlayerCellLayerV77.h"
#include "RedPacketOpenPacketLayerV77.h"
#include "RedPacketMyUserInfoV77.h"

#include "HNLobby/vip/PlatformResVIP.h"
#include "HNLobby/GamePrompt.h"
#include "HNCommon/HNConverCode.h"

#define Packet_Cell_Size Size(WIN_SIZE.width, 250)

namespace RedPacket {
    RedPacketGamePlayerCellLayerV77::RedPacketGamePlayerCellLayerV77() {
        
    }
    
    RedPacketGamePlayerCellLayerV77::~RedPacketGamePlayerCellLayerV77() {
        
    }
    
    bool RedPacketGamePlayerCellLayerV77::init() {
        if (!Button::init()) {
            return false;
        }
        
        Widget::ignoreContentAdaptWithSize(false);
        this->setContentSize(Packet_Cell_Size);
        this->setSwallowTouches(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketGamePlayerCellLayerV77::resetData() {
        m_timeBg = nullptr;
        m_packetButton = nullptr;
        m_userHead = nullptr;
        m_userHeadButton = nullptr;
        m_timeText = nullptr;
        m_packetLeftTimeText = nullptr;
        m_parent = nullptr;
        m_isMe = false;
//        m_packetID = 0;
        m_openPacketCallBack = nullptr;
        m_packetLeftTime = 0;
//        m_redPacket = nullptr;
        m_hasRobPacket = false;
        m_requestMyInfo = nullptr;
    }
    
    void RedPacketGamePlayerCellLayerV77::createUI() {
        m_timeBg = ImageView::create("v88/fahongbao/hongbaoqun/shijiandi.png");
        if (m_timeBg) {
            m_timeBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            m_timeBg->setPosition(Vec2(Packet_Cell_Size.width/2, Packet_Cell_Size.height-20));
            addChild(m_timeBg);
            //
            m_timeText = Text::create("2:34", FONT_TTF_MSYH, 22);
            if (m_timeText) {
                m_timeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_timeText->setTextAreaSize(m_timeBg->getContentSize());
                m_timeText->setPosition(m_timeBg->getContentSize()/2);
                m_timeText->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_timeText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                m_timeText->setColor(Color3B::WHITE);
                m_timeBg->addChild(m_timeText);
            }
        }
        m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
        if (m_userHead) {
            m_userHead->setScale(0.75f);
            m_userHead->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            m_userHead->setPosition(Vec2(Packet_Cell_Size.width-10, 172));
            m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD, "v88/module/gerenxinxi/5.png");
            addChild(m_userHead);
            //user head button
            m_userHeadButton = Button::create("v88/fahongbao/open.png", "v88/fahongbao/open.png");
            if (m_userHeadButton) {
                m_userHeadButton->setContentSize(m_userHead->getContentSize());
                m_userHeadButton->setAnchorPoint(m_userHead->getAnchorPoint());
                m_userHeadButton->setPosition(m_userHead->getPosition());
                m_userHeadButton->addTouchEventListener(CC_CALLBACK_2(RedPacketGamePlayerCellLayerV77::userHeadButtonCallBack, this));
                addChild(m_userHeadButton);
            }
        }
        m_packetButton = Button::create("v88/fahongbao/hongbaoqun/hongbaoyou.png", "v88/fahongbao/hongbaoqun/hongbaoyou.png", "v88/fahongbao/hongbaoqun/hongbaoyou.png");
        if (m_packetButton) {
            if (m_userHead) {
                m_userHead->setPositionY(m_packetButton->getPositionY() + m_packetButton->getContentSize().height);
                m_packetButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                m_packetButton->setPosition(Vec2(m_userHead->getPositionX() - m_userHead->getContentSize().width * 0.75f - 5, 0));
                m_packetButton->addTouchEventListener(CC_CALLBACK_2(RedPacketGamePlayerCellLayerV77::openPacketButtonCallBack, this));
                addChild(m_packetButton);
            }
            //
            m_packetLeftTimeText = Text::create("", FONT_TTF_MSYH, 22);
            if (m_packetLeftTimeText) {
                m_packetLeftTimeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                m_packetLeftTimeText->setTextAreaSize(Size(m_packetButton->getContentSize().width-40, 42));
                m_packetLeftTimeText->setPosition(Vec2(m_packetButton->getContentSize().width/2, 0));
                m_packetLeftTimeText->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_packetLeftTimeText->setTextHorizontalAlignment(TextHAlignment::LEFT);
                m_packetLeftTimeText->setColor(Color3B::GRAY);
                m_packetButton->addChild(m_packetLeftTimeText);
                m_packetLeftTimeText->setString(GetText("OPEN_PACKET_TIP"));
            }
        }
    }
    
    void RedPacketGamePlayerCellLayerV77::setHead(UserInfoStruct* user) {
        if (!user) {
            m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD);
            return;
        }
        m_userHead->setHeadWithInfo(user);
    }
    
    void RedPacketGamePlayerCellLayerV77::setPacketUIByMy(bool isMe) {
        m_isMe = isMe;
        if (!isMe) {
            if (m_userHead) {
                m_userHead->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                m_userHead->setPositionX(10);
            }
            if (m_packetButton) {
                m_packetButton->loadTextures("v88/fahongbao/hongbaoqun/hongbaozuo.png", "v88/fahongbao/hongbaoqun/hongbaozuo.png", "v88/fahongbao/hongbaoqun/hongbaozuo.png");
                m_packetButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                m_packetButton->setPositionX(m_userHead->getPositionX() + m_userHead->getContentSize().width * 0.75f + 5);
            }
        }
        if (m_userHeadButton) {
            m_userHeadButton->setVisible(m_isMe);
            m_userHeadButton->setTouchEnabled(m_isMe);            
        }
//        if (m_userHead) {
//            if (m_isMe) {
//                m_userHead->setHeadTouchEnabled(true);
//                m_userHead->onHeadClickEvent = [=](UserHead *pSender, Vec2 touchVec) {
//                    
//                    auto prompt = GamePromptLayer::create();
//                    if (prompt) {
//                        prompt->showPrompt("这里应该显示玩家身上的金币");
//                        prompt->setColorLayerTouchEnabled(false);
//                    }
//                };
//            }
//            else {
//                m_userHead->setHeadTouchEnabled(false);
//            }
//        }
    }
    
    void RedPacketGamePlayerCellLayerV77::setParentUI(HNLayer* parent) {
        if (parent) {
            m_parent = parent;
        }
    }
    
    void RedPacketGamePlayerCellLayerV77::setOpenPacketCallBack(std::function<void(unsigned int)> callBack) {
        m_openPacketCallBack = callBack;
    }
    
    void RedPacketGamePlayerCellLayerV77::setpacketDetailCallBack(std::function<void (unsigned int)> callBack) {
        m_packetDetailCallBack = callBack;
    }
    
    void RedPacketGamePlayerCellLayerV77::setPacketData(tag_s2c_RedPacketListReq* object, UserInfoStruct *user) {
        if (object != nullptr) {
            m_redPacket = *object;
            if (user) {
                m_sendPacketUser = *user;                
            }

            if (m_userHead && user) {
                m_userHead->setHeadWithInfo(user);
            }
            if (m_timeText) {
                char str[64] = {0};
                time_t timep = m_redPacket.nRedPacketBeginTime;
                tm time = *localtime(&timep);
                sprintf(str, GetText("OPEN_TIME"), time.tm_hour, time.tm_min);
                m_timeText->setString(str);
            }
//            if (m_packetLeftTimeText) {
//                char str[64] = {0};
//                time_t timep = m_redPacket.nRedPacketEndTime;
//                tm time = *localtime(&timep);
//                sprintf(str, GetText("OPEN_TIME_SECOND"), time.tm_hour, time.tm_min, time.tm_sec);
//                m_packetLeftTimeText->setString(str);
//            }
            
        }
    }
    
    void RedPacketGamePlayerCellLayerV77::setMyPacketRobInfo(RedPacket::tag_c2s_RobRedPacketReq *packetInfo) {
        if (packetInfo) {
            if (m_myPacketInfo.nResult > 0 || (m_myPacketInfo.nResult == 0 && m_myPacketInfo.i64Money > 0)) {
                //这个红包抢过，不需要更新数据
            }
            else {
                m_myPacketInfo = *packetInfo;
            }
        }
    }
    
//    unsigned int RedPacketGamePlayerCellLayerV77::getPacketID() {
//        return m_packetID;
//    }
    
    void RedPacketGamePlayerCellLayerV77::setHasRobPacket(bool hasRob) {
        m_hasRobPacket = hasRob;
    }
    
    bool RedPacketGamePlayerCellLayerV77::getHasRobPacket() {
        return m_hasRobPacket;
    }
    
    void RedPacketGamePlayerCellLayerV77::setRequestMyInfoCallBack(std::function<void (unsigned int)> callBack) {
        m_requestMyInfo = callBack;
    }
    
    void RedPacketGamePlayerCellLayerV77::updateMyWalletMoney(std::string name, LLONG money) {
        RedPacketMyUserInfoV77* info = (RedPacketMyUserInfoV77*)this->getChildByName("MyInfo");
        if (info) {
            info->updateMyNameAndMoney(name, money);
        }
    }
    
    void RedPacketGamePlayerCellLayerV77::openPacketButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        
        //请求获取红包状态，如果红包已经被领取过，则显示红包详情；如果没有被领取过，显示拆红包界面。
        auto openLayer = RedPacket::RedPacketOpenPacketLayerV77::create();
        if (openLayer && m_parent) {
            openLayer->setName("Open_Layer");
            openLayer->openAddToParent(m_parent, WIN_SIZE/2, m_isMe);
//            openLayer->setShouldPlayAnimation(!m_hasRobPacket && !m_isMe)
            openLayer->setOpenPacketCallBack([this]() {
                if (m_openPacketCallBack) {
                    m_openPacketCallBack(m_redPacket.nRedPacketID);
                    //开红包按钮被点击
                    m_hasRobPacket = true;
                    if (m_packetButton) {
                        if (m_isMe) {
                            m_packetButton->loadTextures("v88/fahongbao/hongbaoqun/hongbaoyou_1.png", "v88/fahongbao/hongbaoqun/hongbaoyou_1.png", "v88/fahongbao/hongbaoqun/hongbaoyou_1.png");
                        }
                        else {
                            m_packetButton->loadTextures("v88/fahongbao/hongbaoqun/hongbaozuo_1.png", "v88/fahongbao/hongbaoqun/hongbaozuo_1.png", "v88/fahongbao/hongbaoqun/hongbaozuo_1.png");
                        }
                    }
                }
            });
            openLayer->setshowPacketDetailCallBack([this]() {
                if (m_isMe) {
                    if (m_openPacketCallBack) {
                        m_openPacketCallBack(m_redPacket.nRedPacketID);
                    }
                    //查看详情按钮被点击
                    m_hasRobPacket = true;
                    if (m_packetButton) {
                        m_packetButton->loadTextures("v88/fahongbao/hongbaoqun/hongbaoyou_1.png", "v88/fahongbao/hongbaoqun/hongbaoyou_1.png", "v88/fahongbao/hongbaoqun/hongbaoyou_1.png");
                    }
                }
                else {
                    if (m_packetDetailCallBack && m_redPacket.nRedPacketID > 0) {
                        m_packetDetailCallBack(m_redPacket.nRedPacketID);
                    }
                }
            });
            //
            openLayer->setMyPacketInfo(&m_myPacketInfo);
            openLayer->setHasRobPacket(m_hasRobPacket);
            openLayer->setSenderHead(&m_sendPacketUser);
        }
    }
    
    void RedPacketGamePlayerCellLayerV77::userHeadButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        switch (type) {
            case Widget::TouchEventType::BEGAN: {
                auto info = RedPacketMyUserInfoV77::create();
                if (info) {
                    if (m_requestMyInfo) {
                        m_requestMyInfo(m_redPacket.nRedPacketID);
                    }
                    info->setName("MyInfo");
                    info->openAddToParent(this, Vec2(getContentSize().width*0.7f, getContentSize().height/2));
                }
            }
                break;
            case Widget::TouchEventType::MOVED: {
            }
                break;
            case Widget::TouchEventType::CANCELED: {
            }
            case Widget::TouchEventType::ENDED: {
                RedPacketMyUserInfoV77* info = (RedPacketMyUserInfoV77*)this->getChildByName("MyInfo");
                if (info) {
                    info->closeLayer();
                }
            }
                break;
        }
    }
    
    void RedPacketGamePlayerCellLayerV77::updateLeftTime(float dt) {
        
    }
    
}
