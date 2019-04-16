//
//  RedPacketAllPlayersListLayerV77.h
//  MixProject_VIP
//
//  Created by 黄百超 on 03/08/2017.
//
//

#ifndef RedPacketAllPlayersListLayerV77_h
#define RedPacketAllPlayersListLayerV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "RedPacketAllPlayerCellLayerV77.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketAllPlayersListLayerV77 : public Layer {
    public:
        RedPacketAllPlayersListLayerV77();
        ~RedPacketAllPlayersListLayerV77();
        CREATE_FUNC(RedPacketAllPlayersListLayerV77);
        virtual bool init() override;
        
        void openAddToParent(HNLayer* parent, Vec2 pos);
        void addCellIntoList(RedPacketAllPlayerCellLayerV77* cell);
        
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
        
        std::vector<RedPacketAllPlayerCellLayerV77*>   m_allPlayers;
    };
}

#endif /* RedPacketAllPlayersListLayerV77_h */
