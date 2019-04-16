//
//  MoneyGetFourLevel.hpp
//  LibHNUI
//
//  Created by mac on 2018/8/8.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef MoneyGetFourLevel_hpp
#define MoneyGetFourLevel_hpp

#include "cocos2d.h"

#include "UI/Base/HNLayer.h"
namespace HN {
    class MoneyGetFourLevel : public HNLayer {
    public:
        MoneyGetFourLevel();
        virtual ~MoneyGetFourLevel();
        CREATE_FUNC(MoneyGetFourLevel);
        
        virtual bool init() override;
        void open(Node* parent, Vec2 vec, int zorder,int uiType);
        void close();
        void createUIByType(int _uiType);
    };
}

#endif /* MoneyGetFourLevel_hpp */
