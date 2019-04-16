//
//  RedPacketSetPacketBombLayer.h
//  MixProject_189
//
//  Created by 黄百超 on 08/08/2017.
//
//

#ifndef RedPacketSetPacketBombLayer_h
#define RedPacketSetPacketBombLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "RedPacketSendPacketLayer.h"

USING_NS_CC;

namespace RedPacket {
    class RedPacketSetPacketBombLayer : public Layer {
        
    private:
        typedef std::function<void()>   SelectBombNumberCallback;
        
    public:
        RedPacketSetPacketBombLayer();
        ~RedPacketSetPacketBombLayer();
        CREATE_FUNC(RedPacketSetPacketBombLayer);
        virtual bool init() override;
        
        void openAddToParent(RedPacketSendPacketLayer* parent, Vec2 pos, int bombNumber);
        
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
        RedPacketSendPacketLayer*       m_sendPacketLayer;
        ListView*                       m_selectBombList;
        Button*                         m_closeButton;
//        bool                            m_canMove;
        int                             m_bombNumber;
    };
}

#endif /* RedPacketSetPacketBombLayer_h */
