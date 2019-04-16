//
//  PopularizeSignSecret.h
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef PopularizeSignSecret_h
#define PopularizeSignSecret_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"

USING_NS_CC;

//推广秘籍
class PopularizeSignSecret : public Layer {
public:
    PopularizeSignSecret();
    ~PopularizeSignSecret();
    CREATE_FUNC(PopularizeSignSecret);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, Vec2 pos);
    
private:
    void resetData();
    void createUI();
    
    void closeLayer();
    void closeButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    
private:
    ImageView*                      m_bg;
    
    EventListenerTouchOneByOne*		touchListener;
    Text*                           m_myID;
    
};

#endif /* PopularizeSignSecret_h */
