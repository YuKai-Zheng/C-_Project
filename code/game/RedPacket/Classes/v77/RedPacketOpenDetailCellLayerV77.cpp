//
//  RedPacketOpenDetailCellLayerV77.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#include "RedPacketOpenDetailCellLayerV77.h"
#include "HNLobby/vip/PlatformResVIP.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"

//#define RED_WIN_SIZE Size(WIN_SIZE.height, WIN_SIZE.width)
#define Detail_Cell_Size Size(WIN_SIZE.width, 150)

namespace RedPacket {
    
    RedPacketOpenDetailCellLayerV77::RedPacketOpenDetailCellLayerV77() {
        
    }
    
    RedPacketOpenDetailCellLayerV77::~RedPacketOpenDetailCellLayerV77() {
        
    }
    
    bool RedPacketOpenDetailCellLayerV77::init() {
        if (!Layout::init()) {
            return false;
        }
        
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketOpenDetailCellLayerV77::setHead(UserInfoStruct *user) {
        if (!user) {
            if (m_userHead)
                m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD);
            if (m_userName)
                m_userName->setString("");
            return;
        }
        if (m_userHead)
            m_userHead->setHeadWithInfo(user);
        if (m_userName)
            m_userName->setString(ForceGBKToUtf8(user->nickName));
    }
    
    void RedPacketOpenDetailCellLayerV77::setOpenPacketPlayerInfo(RobRedPacketUserInfo* info) {
        if (info) {
            m_openPlayerInfo = *info;
            if (m_userName) {
                m_userName->setString(ForceGBKToUtf8(m_openPlayerInfo.szUserName));
            }
            if (m_packetMoney) {
                m_packetMoney->setString(Tools::addComma(m_openPlayerInfo.i64RobMoney));
            }
            if (m_time) {
                char str[64] = {0};
                time_t timep = m_openPlayerInfo.nRobTime;
                tm time = *localtime(&timep);
                sprintf(str, GetText("OPEN_TIME"), time.tm_hour, time.tm_min);
                m_time->setString(str);
            }
            if (m_userHead) {
                UserInfoStruct userInfo;
                userInfo.dwUserID = m_openPlayerInfo.nUserID;
                userInfo.isVirtual = m_openPlayerInfo.nIsRobot;
                userInfo.bLogoID = m_openPlayerInfo.nLogoID;
                memcpy(userInfo.nickName, m_openPlayerInfo.szUserName, sizeof(m_openPlayerInfo.szUserName));
                memcpy(userInfo.szWXTouXiang, m_openPlayerInfo.szWXTouXiang, sizeof(m_openPlayerInfo.szWXTouXiang));
                m_userHead->setHeadWithInfo(&userInfo);
            }
            if (m_bombTip) {
                m_bombTip->setVisible(m_openPlayerInfo.nIsThunder);
            }
            if (m_luckyTip) {
                m_luckyTip->setVisible(m_openPlayerInfo.nIsLucky);
            }
        }
    }
    
    Size RedPacketOpenDetailCellLayerV77::getContentSize() {
        return m_contentSize;
    }
    
    void RedPacketOpenDetailCellLayerV77::resetData() {
        m_userHead = nullptr;
        m_userName = nullptr;
        m_time = nullptr;
        m_packetMoney = nullptr;
        m_luckyTip = nullptr;
        m_bombTip = nullptr;
        m_contentSize = Size::ZERO;
        m_line = nullptr;
    }
    
    void RedPacketOpenDetailCellLayerV77::createUI() {
        m_contentSize = Detail_Cell_Size;
        this->setContentSize(m_contentSize);
        
        auto bg = ImageView::create("v88/fahongbao/jilu/8.png");
        if (bg) {
            bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            bg->setPosition(Vec2(Detail_Cell_Size.width/2, 20));
            addChild(bg);
        }
        
        m_userHead = UserHead::create("v88/module/gerenxinxi/4.png");
        if (m_userHead) {
            m_userHead->setScale(0.65f);
            m_userHead->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            m_userHead->setPosition(Vec2(10, Detail_Cell_Size.height));
            m_userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD, "v88/module/gerenxinxi/5.png");
            addChild(m_userHead);
            //
            m_userName = Text::create("我的名字是什么", FONT_TTF_MSYH, 30);
            if (m_userName) {
                m_userName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                m_userName->setPosition(Vec2(m_userHead->getPositionX() + m_userName->getContentSize().width*0.7f, Detail_Cell_Size.height-m_userName->getContentSize().height/2));
                m_userName->setColor(Color3B::BLACK);
                addChild(m_userName);
                //
                m_time = Text::create("00:00", FONT_TTF_MSYH, 22);
                if (m_time) {
                    m_time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    m_time->setPosition(Vec2(m_userName->getPositionX(), Detail_Cell_Size.height*0.4f));
                    m_time->setColor(Color3B::GRAY);
                    addChild(m_time);
                    //
                    m_luckyTip = ImageView::create("v88/fahongbao/xiangqing/4.png");
                    if (m_luckyTip) {
                        m_luckyTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                        m_luckyTip->setPosition(Vec2(Detail_Cell_Size.width-10, m_time->getPositionY()));
                        m_luckyTip->setVisible(false);
                        addChild(m_luckyTip);
                    }
                }
                //
                m_packetMoney = Text::create("0", FONT_TTF_MSYH, 36);
                if (m_packetMoney) {
                    m_packetMoney->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                    m_packetMoney->setPosition(Vec2(Detail_Cell_Size.width-10, m_userName->getPositionY()));
                    m_packetMoney->setColor(Color3B::BLACK);
                    addChild(m_packetMoney);
                }
                //
                m_bombTip = ImageView::create("v88/fahongbao/bomb2.png");
                if (m_bombTip) {
                    m_bombTip->setAnchorPoint(Vec2(0.7f,0.37f));
                    m_bombTip->setPosition(m_userHead->getPosition() + Vec2(m_userHead->getContentSize().width/2, -m_userHead->getContentSize().height/2));
                    m_bombTip->setVisible(false);
                    addChild(m_bombTip);
                }
            }
        }
    }
    
}
