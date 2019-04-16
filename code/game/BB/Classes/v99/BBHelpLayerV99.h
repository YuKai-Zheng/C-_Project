//
//  BBHelpLayerV99.h
//  MixProject
//
//  Created by 黄百超 on 06/04/2017.
//
//

#ifndef BBHelpLayerV99_h
#define BBHelpLayerV99_h

#include "UI/Base/HNLayer.h"

namespace BB {
    //百人牛牛帮助界面
    class BBHelpLayerV99 : public HNLayer {
    public:
        BBHelpLayerV99();
        virtual ~BBHelpLayerV99();
        CREATE_FUNC(BBHelpLayerV99);
        
        virtual bool init() override;
        virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
        virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
        void moveSelfWithAction();
        Vec2 convertGL(float x, float y);
        
    };
}

#endif /* BBHelpLayerV99_h */
