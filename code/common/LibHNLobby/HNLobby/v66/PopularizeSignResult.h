//
//  PopularizeSignResult.h
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef PopularizeSignResult_h
#define PopularizeSignResult_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"

USING_NS_CC;

//推广审核结果
class PopularizeSignResult : public Layer {
public:
    PopularizeSignResult();
    ~PopularizeSignResult();
    CREATE_FUNC(PopularizeSignResult);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, Vec2 pos);
    void setRefuseInfo(std::string reason);
    
private:
    void resetData();
    void createUI();
    
    void closeLayer();
    
    void okayButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    
    void setArticleAchievementAtIndex(unsigned int index, bool hasAchieved);
    
private:
    ImageView*                  m_bg;
    AUIButton*                  m_okButton;
    
    std::vector<ImageView*>     m_articles;
    Text*                       m_refuseReason;  //拒绝原因
    
    EventListenerTouchOneByOne*		touchListener;
    
};

#endif /* PopularizeSignResult_h */
