//
//  RedPacketAllPlayersListLayer.h
//  MixProject_VIP
//
//  Created by 黄百超 on 03/08/2017.
//
//

#ifndef RedPacketAllPlayersListLayer_h
#define RedPacketAllPlayersListLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "RedPacketAllPlayerCellLayer.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketAllPlayersListLayer : public Layer {
    public:
        RedPacketAllPlayersListLayer();
        ~RedPacketAllPlayersListLayer();
        CREATE_FUNC(RedPacketAllPlayersListLayer);
        virtual bool init() override;
        
        void openAddToParent(HNLayer* parent, Vec2 pos);
        void addCellIntoList(RedPacketAllPlayerCellLayer* cell);
        
    public:
        void updateAllPlayers();
        
    private:
        void resetData();
        void createUI();
        
        void closeLayer();
        
    //按钮回调
    private:
        void backButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        ImageView*                  m_topBar;
        Button*                     m_toolBtn;
        Text*                       m_title;
        cocos2d::ui::ScrollView*    m_packetCells;
        
        float                       m_offsetH;
        Size                        m_cellSize;
        
        std::vector<RedPacketAllPlayerCellLayer*>   m_allPlayers;
    };
}

#endif /* RedPacketAllPlayersListLayer_h */
