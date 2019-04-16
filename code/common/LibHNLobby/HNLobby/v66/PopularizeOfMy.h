//
//  PopularizeOfMy.h
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef PopularizeOfMy_h
#define PopularizeOfMy_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;

//我的推广
class PopularizeOfMy : public Layer {
public:
    PopularizeOfMy();
    ~PopularizeOfMy();
    CREATE_FUNC(PopularizeOfMy);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, Vec2 pos);
    
private:
    void resetData();
    void createUI();
    
    void closeLayer();
    void shareButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    void secretButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    
    void requestData();
    bool myDataSelector(HN::HNSocketMessage *socketMessage);
    
    void createSelectShareTypeLayer();
    void shareCallback(cocos2d::Ref *pSender, Widget::TouchEventType type);
    
private:
    ImageView*                  m_bg;
    AUIButton*                  m_shareButton;
    Button*                     m_secretButton;
    
    EventListenerTouchOneByOne*		touchListener;
    
    Text*                       m_profitTotal;  //总收益
    Text*                       m_profitToday;  //当日收益
    Text*                       m_profitWeek;   //本周收益
    Text*                       m_profitMonth;  //本月收益
    
    Text*                       m_peopleLevelTotal; //总人数
    Text*                       m_peopleLevelOne;   //一级人数
    Text*                       m_peopleLevelTWO;   //二级人数
    Text*                       m_peopleLevelThree; //三级人数

    Text*                       m_peopleIncreaseLevelOne;   //一级新增人数
    Text*                       m_peopleIncreaseLevelTWO;   //二级新增人数
    Text*                       m_peopleIncreaseLevelThree; //三级新增人数

    Text*                       m_profitIncreaseLevelOne;   //一级新增收益
    Text*                       m_profitIncreaseLevelTWO;   //二级新增收益
    Text*                       m_profitIncreaseLevelThree; //三级新增收益
    
};

#endif /* PopularizeOfMy_h */
