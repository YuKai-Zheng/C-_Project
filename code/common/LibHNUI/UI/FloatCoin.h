//
//  FloatCoin.h
//  LibHNUI
//
//  Created by 黄百超 on 25/10/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef FloatCoin_h
#define FloatCoin_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
USING_NS_CC;

/************************************************************************/
/* 通用结算金币漂浮                                                        */
/************************************************************************/
namespace HN {
    
    class FloatCoin : public HNLayer {
    public:
        CREATE_FUNC(FloatCoin);
        FloatCoin();
        ~FloatCoin();
        virtual bool init();
        
    public:
        void openParent(Node* parent, Vec2 vec, int zOrder);
        void updateCoin(long long money);
        
    private:
        void resetData();
        void createUI();
        
    private:
        ImageView* m_bg;
        TextAtlas* m_moneyText;
        
    };
    
}

#endif /* FloatCoin_h */
