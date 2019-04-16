//
//  RedPacketSetPacketBombLayerV77.h
//  MixProject_189
//
//  Created by 黄百超 on 08/08/2017.
//
//

#ifndef RedPacketSetPacketBombLayerV77_h
#define RedPacketSetPacketBombLayerV77_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "RedPacketSendPacketLayerV77.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketSetPacketBombLayerV77 : public Layer {
        
    private:
        typedef std::function<void()>   SelectBombNumberCallback;
        
    public:
        RedPacketSetPacketBombLayerV77();
        ~RedPacketSetPacketBombLayerV77();
        CREATE_FUNC(RedPacketSetPacketBombLayerV77);
        virtual bool init() override;
        
        void openAddToParent(RedPacketSendPacketLayerV77* parent, Vec2 pos, int bombNumber);
        
    private:
        void resetData();
        void createUI();
        
        void closeLayer();
        
        void setListViewSelectedItem(int selectedItem);
        void closeButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        EventListenerTouchOneByOne*		touchListener;
        
    private:
        ImageView*                      m_bg;
        RedPacketSendPacketLayerV77*       m_sendPacketLayer;
        ListView*                       m_selectBombList;
        Button*                         m_closeButton;
//        bool                            m_canMove;
        int                             m_bombNumber;
    };
}

#endif /* RedPacketSetPacketBombLayerV77_h */
