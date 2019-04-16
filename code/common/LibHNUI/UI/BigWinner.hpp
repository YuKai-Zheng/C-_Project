//
//  BigWinner.hpp
//  LibHNUI
//
//  Created by mac on 2018/8/4.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef BigWinner_hpp
#define BigWinner_hpp

#include "cocos2d.h"

#include "UI/Base/HNLayer.h"
namespace HN {
    class BigWinnerLayer : public HNLayer {
    public:
        BigWinnerLayer();
        virtual ~BigWinnerLayer();
        CREATE_FUNC(BigWinnerLayer);
        virtual void onExit() override;
        virtual bool init() override;
        void open(Node* parent, Vec2 vec, int zorder,int gameType,int uiType,long long winMoney);
        void close();
        void createUIByType(int _gameType,int _uiType);
        void showXunhuanEffect();
        void showAnimationRotate(int _rotate);
        virtual void update(float dt) override;
    private:
        long long m_winMoney;
        long long m_curMoney;
        TextAtlas* m_moneyText;
        int nMusicEffectId;
    };
}


#endif /* BigWinner_hpp */
