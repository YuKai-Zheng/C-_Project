//
//  PopularizeSignArticle.h
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef PopularizeSignArticle_h
#define PopularizeSignArticle_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"

USING_NS_CC;

//推广申请条款
class PopularizeSignArticle : public Layer {
public:
    PopularizeSignArticle();
    ~PopularizeSignArticle();
    CREATE_FUNC(PopularizeSignArticle);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, Vec2 pos);
      
private:
    void resetData();
    void createUI();
    
    void closeLayer();
    
    void okayButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    void closeButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    
private:
    ImageView*                  m_bg;
    AUIButton*                  m_okButton;
    
    EventListenerTouchOneByOne*		touchListener;
    
};

#endif /* PopularizeSignArticle_h */
