//
//  HelpLayer.h
//  LibHNUI
//
//  Created by 黄百超 on 26/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef HelpLayer_h
#define HelpLayer_h

#include "cocos2d.h"

#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"

namespace HN {
    class HelpLayer : public HNLayer {
    public:
        HelpLayer();
        virtual ~HelpLayer();
        CREATE_FUNC(HelpLayer);
        
        virtual bool init() override;
        virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
        virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
        void setContentOnView(std::string contentPath);
        void setContentOnViewEx(std::string contentPath);
        void setBgImage(std::string bgPath);
        void setOrignalPosition();
        void moveSelfOutWithAction();
        void moveSelfInWithAction();
        Vec2 convertGL(float x, float y);
        
    private:
        void setContentSprite(Sprite* sprite);
        void closeHelpCallBack(Ref *pSender);
    
    private:
        ImageView*      helpBg;
    };
}

#endif /* HelpLayer_h */
