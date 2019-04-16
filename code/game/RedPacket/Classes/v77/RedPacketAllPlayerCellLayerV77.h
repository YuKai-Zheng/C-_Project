//
//  RedPacketAllPlayerCellLayerV77.h
//  MixProject_VIP
//
//  Created by 黄百超 on 03/08/2017.
//
//

#ifndef RedPacketAllPlayerCellLayerV77_h
#define RedPacketAllPlayerCellLayerV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/UserHead.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketAllPlayerCellLayerV77 : public Button {
    public:
        RedPacketAllPlayerCellLayerV77();
        ~RedPacketAllPlayerCellLayerV77();
        CREATE_FUNC(RedPacketAllPlayerCellLayerV77);
        virtual bool init() override;
        
        void setHead(UserInfoStruct* user);
        int getPlayerID();
        Size getContentSize();
                
    private:
        void resetData();
        void createUI();
        
    private:
        UserHead*       m_userHead;
        Text*           m_userName;
        
        int             m_playerID;
        Size            m_contentSize;
    };
}

#endif /* RedPacketAllPlayerCellLayerV77_h */
