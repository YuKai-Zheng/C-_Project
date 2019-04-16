//
//  RedPacketOpenDetailsLayer.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#include "RedPacketOpenDetailsLayer.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "RedPacketOpenDetailCellLayer.h"
#include "Tool/Tools.h"
#include "cocostudio/CocoStudio.h"


namespace RedPacket {
    
    RedPacketOpenDetailsLayer::RedPacketOpenDetailsLayer() {
        
    }
    
    RedPacketOpenDetailsLayer::~RedPacketOpenDetailsLayer() {
        
    }
    
    bool RedPacketOpenDetailsLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketOpenDetailsLayer::setSenderHead(UserInfoStruct *user) {
        if (!user) {
            if (m_userHead)
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88);
            if (m_userName)
                m_userName->setString("");
            return;
        }
        if (m_userHead)
            m_userHead->setHeadWithInfo(user);
        if (m_userName)
            m_userName->setString(ForceGBKToUtf8(user->nickName));
    }
    
    void RedPacketOpenDetailsLayer::openAddToParent(Layer *parent, cocos2d::Vec2 pos) {
        if (parent) {
            this->setPosition(pos);
            parent->addChild(this);
        }
    }
    
    void RedPacketOpenDetailsLayer::setPacketInfo(RedPacket::tag_c2s_RobRedPacketReq *info) {
        if (info) {
            m_packetInfo = *info;
            if (m_packetMoney) {
                if (m_packetInfo.nResult == 0) {
                    //抢红包成功
                    m_packetMoney->setString(Tools::addComma(m_packetInfo.i64Money));
                    //播放特效动画
                    std::string path = m_packetInfo.nIsThunder == 1 ? "v88/animation/redbag/redbag_bomb.csb" : "v88/animation/redbag/redbag_win.csb";
                    auto node = CSLoader::createNode(path);
                    if (node) {
                        node->setPosition(WIN_SIZE/2);
                        addChild(node);
                        auto action = CSLoader::createTimeline(path);
                        if (action) {
                            node->runAction(action);
                            action->play(m_packetInfo.nIsThunder == 1 ? "redbag_bomb" : "redbag_win", false);
                        }
                    }
                }
                else {
                    char str[32];
                    memset(str, 0, sizeof(str));
                    sprintf(str, "OPEN_PACKET_ERROR_%d", m_packetInfo.nResult);
                    m_packetMoney->setString(GetText(str));
                }
            }
        }
    }
    
    //领取红包详情基础信息
    void RedPacketOpenDetailsLayer::setOpenPacketDetailInfo(RedPacket::tag_c2s_RedPacketRobLogReq *info) {
        if (info) {
            m_openPacketDetailInfo = *info;
            if (m_userName) {
                m_userName->setString(ForceGBKToUtf8(m_openPacketDetailInfo.szUserName));
            }
            if (m_userHead) {
                UserInfoStruct userInfo;
                userInfo.dwUserID = m_openPacketDetailInfo.nUserID;
                userInfo.bLogoID = m_openPacketDetailInfo.nLogoID;
                userInfo.isVirtual = m_openPacketDetailInfo.nIsRobot;
                memcpy(userInfo.nickName, m_openPacketDetailInfo.szUserName, sizeof(m_openPacketDetailInfo.szUserName));
                memcpy(userInfo.szWXTouXiang, m_openPacketDetailInfo.szWXTouXiang, sizeof(m_openPacketDetailInfo.szWXTouXiang));
                m_userHead->setHeadWithInfo(&userInfo);
            }
            if (m_packetResult) {
                char str[256];
                if (m_openPacketDetailInfo.nHaveRobPart == m_openPacketDetailInfo.nRedPacketPart) {
                    //红包已经被抢完
                    sprintf(str, GetText("OPEN_PACKET_DETAIL_OVER"), m_openPacketDetailInfo.nHaveRobPart, m_openPacketDetailInfo.nRedPacketPart, Tools::addComma(m_openPacketDetailInfo.i64RedPacketMoney).c_str());
                }
                else {
                    //红包没有被领完
                    sprintf(str, GetText("OPEN_PACKET_DETAIL_ING"), m_openPacketDetailInfo.nHaveRobPart, m_openPacketDetailInfo.nRedPacketPart);
                }
                m_packetResult->setString(str);
            }
            if (m_bombNumber) {
                char str[32];
                memset(str, 0, sizeof(str));
                sprintf(str, GetText("OPEN_PACKET_BOMB"), m_openPacketDetailInfo.nThunderNumber);
                m_bombNumber->setString(str);
            }
        }
    }
    
    //更新红包详情领取界面单个领取玩家的领取信息
    void RedPacketOpenDetailsLayer::setOpenPacketPlayerDetail(RobRedPacketUserInfo* userInfo) {
        if (m_detailsList) {
            //说明有玩家抢到红包，这个数据有效
            auto cell = RedPacketOpenDetailCellLayer::create();
            if (cell) {
                cell->setOpenPacketPlayerInfo(userInfo);
                m_detailsList->pushBackCustomItem(cell);
            }
        }

    }
    
    void RedPacketOpenDetailsLayer::setMyHistoryBtnCallBack(std::function<void ()> callBack) {
        m_myHistoryCallBack = callBack;
    }
    
    void RedPacketOpenDetailsLayer::resetData() {
        m_bg = nullptr;
        m_userHead = nullptr;
        m_userName = nullptr;
        m_closeButton = nullptr;
        m_title = nullptr;
        m_tips = nullptr;
        m_packetResult = nullptr;
        m_packetMoney = nullptr;
        m_bombNumber = nullptr;
        m_bombImage = nullptr;
        m_detailsList = nullptr;
        m_myHistoryButton = nullptr;
    }
    
    void RedPacketOpenDetailsLayer::createUI() {
        m_bg = ImageView::create("v88/fahongbao/xiangqing/5.png");
        if (m_bg) {
			m_bg->setContentSize(WIN_SIZE);
			m_bg->setScale9Enabled(true);
            m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_bg->setPosition(WIN_SIZE/2);
            m_bg->setTouchEnabled(true);
            addChild(m_bg);
            //
            m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
            if (m_userHead) {
                m_userHead->setScale(0.85f);
                m_userHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_userHead->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.78f));
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "v88/module/gerenxinxi/5.png");
                m_bg->addChild(m_userHead);
                //
                m_userName = Text::create("我的名字是什么", FONT_TTF_MSYH, 26);
                if (m_userName) {
                    m_userName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    m_userName->setPosition(Vec2(m_userHead->getPositionX(), m_userHead->getPositionY() - m_userHead->getContentSize().height * 0.85f * 0.6f));
                    m_userName->setColor(Color3B::BLACK);
                    m_bg->addChild(m_userName);
                }
            }
            //
            m_closeButton = Button::create("v88/fahongbao/1.png", "v88/fahongbao/0.png");
            if (m_closeButton) {
                m_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                m_closeButton->setPosition(Vec2(0, m_bg->getContentSize().height));
                m_closeButton->addTouchEventListener(CC_CALLBACK_2(RedPacketOpenDetailsLayer::backButtonCallBack, this));
                addChild(m_closeButton);
                //
                m_title = Text::create("红包详情", FONT_TTF_MSYH, 32);
                if (m_title) {
                    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    m_title->setPosition(Vec2(m_closeButton->getPositionX()+m_closeButton->getContentSize().width, m_closeButton->getPositionY()-m_closeButton->getContentSize().height/2));
                    m_title->setColor(Color3B(255, 225, 176));
                    addChild(m_title);
                }
            }
            m_tips = Text::create("已存入背包，可用于发包", FONT_TTF_MSYH, 22);
            if (m_tips) {
                m_tips->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_tips->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.5f));
                m_tips->setColor(Color3B(59, 98, 176));
                m_bg->addChild(m_tips);
                m_packetMoney = Text::create("0", FONT_TTF_MSYH, 70);
                if (m_packetMoney) {
                    m_packetMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    m_packetMoney->setPosition(Vec2(m_tips->getPositionX(), m_tips->getPositionY() + m_tips->getContentSize().height * 2.2f));
                    m_packetMoney->setColor(Color3B::BLACK);
                    m_bg->addChild(m_packetMoney);
                }
            }
            m_bombNumber = Text::create("", FONT_TTF_MSYH, 30);
            if (m_bombNumber) {
                m_bombNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_bombNumber->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.45f));
                m_bombNumber->setColor(Color3B(59, 98, 176));
                m_bg->addChild(m_bombNumber);
                //
                m_bombImage = ImageView::create("v88/fahongbao/bomb.png");
                if (m_bombImage) {
                    m_bombImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    m_bombImage->setScale(0.15f);
                    m_bombImage->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.45f));
                    m_bg->addChild(m_bombImage);
                }
            }
            m_packetResult = Text::create("", FONT_TTF_MSYH, 22);
            if (m_packetResult) {
                m_packetResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_packetResult->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_packetResult->setTextHorizontalAlignment(TextHAlignment::LEFT);
                m_packetResult->setPosition(Vec2(10, m_bg->getContentSize().height*0.4f));
                m_packetResult->setColor(Color3B::GRAY);
                addChild(m_packetResult);
            }
            m_myHistoryButton = Button::create("v88/fahongbao/xiangqing/7.png", "v88/fahongbao/xiangqing/7.png");
            if (m_myHistoryButton) {
                m_myHistoryButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                m_myHistoryButton->setPosition(Vec2(WIN_SIZE.width/2, 0));
                m_myHistoryButton->addTouchEventListener(CC_CALLBACK_2(RedPacketOpenDetailsLayer::myHistoryButtonCallBack, this));
                addChild(m_myHistoryButton);
                //
                m_detailsList = ListView::create();
                if (m_detailsList) {
                    m_detailsList->setGravity(cocos2d::ui::ListView::Gravity::TOP);
                    m_detailsList->setDirection(ListView::Direction::VERTICAL);
                    m_detailsList->setContentSize(Size(WIN_SIZE.width, WIN_SIZE.height*0.3f));
                    m_detailsList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    m_detailsList->setPosition(Vec2(WIN_SIZE.width/2, m_myHistoryButton->getContentSize().height));
                    m_detailsList->setScrollBarAutoHideEnabled(false);
                    m_detailsList->setBounceEnabled(true);
                    addChild(m_detailsList);
                }
            }
            
        }
    }
    
    void RedPacketOpenDetailsLayer::closeLayer() {
        this->removeFromParent();
    }
    
    void RedPacketOpenDetailsLayer::backButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        closeLayer();
    }
    
    void RedPacketOpenDetailsLayer::myHistoryButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        if (m_myHistoryCallBack) {
			m_myHistoryCallBack();
            closeLayer();
        }
    }
    
}
