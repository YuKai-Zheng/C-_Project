//
//  ToolBarLayer.h
//  LibHNUI
//
//  Created by 黄百超 on 01/07/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef ToolBarLayer_h
#define ToolBarLayer_h

#include "cocos2d.h"
#include <vector>
#include "UI/Base/HNLayer.h"

USING_NS_CC;

namespace HN {
    class ToolBarLayer : public HNLayer {
    public:
        ToolBarLayer();
        ~ToolBarLayer();
        CREATE_FUNC(ToolBarLayer);
        
        typedef std::function<void()> TapCallback;
        
        virtual bool init() override;
        virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
        virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
        
        void addImageAndCallBack(std::string imagePath, TapCallback callBack);
        void create(Node* parent, Vec2 vec, int zorder, int tag);
        //        void changeBgRes(std::string bgRes);
        
    private:
        void open();
        void close();
        Vec2 convertGL(float x, float y);
        
        void showHideButtonCallBack(Ref* pSender, Widget::TouchEventType type);
        
    private:
        Sprite*                     m_bg;
        Button*                     m_hideBtn;
        std::vector<ImageView*>     m_funcImages;
        
        bool                        m_canClose;
    };
}

#endif /* ToolBarLayer_h */

