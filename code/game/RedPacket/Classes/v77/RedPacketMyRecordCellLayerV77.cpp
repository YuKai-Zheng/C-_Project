//
//  RedPacketMyRecordCellLayerV77.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#include "RedPacketMyRecordCellLayerV77.h"
#include "HNLobby/vip/PlatformResVIP.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "HNPlatform/HNPlatformLogic.h"

//#define RED_WIN_SIZE Size(WIN_SIZE.height, WIN_SIZE.width)
#define Detail_Cell_Size Size(WIN_SIZE.width, 150)

namespace RedPacket {
    
    RedPacketMyRecordCellLayerV77::RedPacketMyRecordCellLayerV77() {
        
    }
    
    RedPacketMyRecordCellLayerV77::~RedPacketMyRecordCellLayerV77() {
        
    }
    
    bool RedPacketMyRecordCellLayerV77::init() {
        if (!Layout::init()) {
            return false;
        }
        
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    Size RedPacketMyRecordCellLayerV77::getContentSize() {
        return m_contentSize;
    }
    
    void RedPacketMyRecordCellLayerV77::setHistoryCellData(RedPacket::tag_s2c_MyHistoryRobLogReq *data) {
        if (data) {
            m_historyCellData = *data;
            if (m_time) {
                char str[64];
                memset(str, 0, sizeof(str));
                time_t timep = m_historyCellData.tLootTime;
                tm time = *localtime(&timep);
                sprintf(str, GetText("OPEN_TIME"), time.tm_hour, time.tm_min);
                m_time->setString(str);
            }
            if (m_description) {
                char str[256];
                memset(str, 0, sizeof(str));
                if (m_historyCellData.nUserID == PlatformLogic()->loginResult.dwUserID) {
                    //红包是自己发的
                    sprintf(str, GetText("OPEN_PACKET_HISTORY_MY"), Tools::addComma(m_historyCellData.i64RedPacketMoney).c_str(), m_historyCellData.nThunderCount, moneyWithCharactor(m_historyCellData.i64GainMoney).c_str());
                }
                else {
                    //红包是别人发的
                    sprintf(str, GetText("OPEN_PACKET_HISTORY_OTHER"), ForceGBKToUtf8(m_historyCellData.szUserName), moneyWithCharactor(m_historyCellData.i64GainMoney).c_str());
                }
                //红包信息
                m_description->setString(str);
            }
        }
    }
    
    void RedPacketMyRecordCellLayerV77::resetData() {
        m_description = nullptr;
        m_time = nullptr;
        m_contentSize = Size::ZERO;
        m_line = nullptr;
    }
   
    void RedPacketMyRecordCellLayerV77::createUI() {
        m_contentSize = Detail_Cell_Size;
        this->setContentSize(m_contentSize);
        
        auto bg = ImageView::create("v88/fahongbao/jilu/8.png");
        if (bg) {
            bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            bg->setPosition(Vec2(Detail_Cell_Size.width/2, 20));
            addChild(bg);
        }
        
        m_description = Text::create("", FONT_TTF_MSYH, 30);
        if (m_description) {
            m_description->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            m_description->setPosition(Vec2(Detail_Cell_Size.width/2, Detail_Cell_Size.height));
            m_description->setColor(Color3B::BLACK);
            m_description->setTextVerticalAlignment(TextVAlignment::TOP);
            m_description->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_description->setTextAreaSize(Size(Detail_Cell_Size.width-20, Detail_Cell_Size.height*0.7f));
            addChild(m_description);
            m_time = Text::create("23:21", FONT_TTF_MSYH, 22);
            if (m_time) {
                m_time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_time->setPosition(Vec2(10, Detail_Cell_Size.height*0.35f));
                m_time->setColor(Color3B::GRAY);
                addChild(m_time);
            }
        }
    }
    
    std::string RedPacketMyRecordCellLayerV77::moneyWithCharactor(LLONG money) {
        char str[64];
        memset(str, 0, sizeof(str));
        if (money > 0) {
            sprintf(str, "+%s", Tools::addComma(money).c_str());
        }
        else {
            sprintf(str, "%s", Tools::addComma(money).c_str());
        }
        return str;
    }
    
}
