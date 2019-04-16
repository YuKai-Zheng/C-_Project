//
//  PopularizeSignLayer.h
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef PopularizeSignLayer_h
#define PopularizeSignLayer_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;

//推广申请界面
class PopularizeSignLayer : public Layer {
public:
    PopularizeSignLayer();
    ~PopularizeSignLayer();
    CREATE_FUNC(PopularizeSignLayer);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, Vec2 pos);
    void setIsSigning(bool isSign);
    
private:
    void resetData();
    void createUI();
    
    void closeLayer();
    
    void signButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    void articleButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    void checkBoxCallBack(Ref* sender, CheckBox::EventType type);
    bool signPopularizeSelector(HN::HNSocketMessage *socketMessage);
    
private:
    ImageView*                      m_bg;
    EventListenerTouchOneByOne*		touchListener;
    AUIButton*                      m_signButton;
    Button*                         m_articleButton;
    CheckBox*                       m_agreeCheckBox;
    ImageView*                      m_signState;
};


#endif /* PopularizeSignLayer_h */
