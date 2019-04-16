//
//  RedPacketMyRecordCellLayerV77.h
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#ifndef RedPacketMyRecordCellLayerV77_h
#define RedPacketMyRecordCellLayerV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"
#include "RedPacketCommonDef.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketMyRecordCellLayerV77 : public Layout {
    public:
        RedPacketMyRecordCellLayerV77();
        ~RedPacketMyRecordCellLayerV77();
        CREATE_FUNC(RedPacketMyRecordCellLayerV77);
        virtual bool init() override;
        
        Size getContentSize();
        void setHistoryCellData(tag_s2c_MyHistoryRobLogReq* data);

    private:
        void resetData();
        void createUI();
        
        std::string moneyWithCharactor(LLONG money);
        
    private:
        Text*           m_description;
        Text*           m_time;
        ImageView*      m_line;
        Size            m_contentSize;
        
        tag_s2c_MyHistoryRobLogReq m_historyCellData;
    };
}

#endif /* RedPacketMyRecordCellLayerV77_h */
