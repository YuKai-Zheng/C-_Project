//
//  EightRule.hpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/2.
//

#ifndef EightRule_hpp
#define EightRule_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "UI/Base/HNLayer.h"
using namespace cocos2d::ui;
using namespace cocos2d::extension;

USING_NS_CC;


class EightRule:public HNLayer  {
public:
    EightRule();
    ~EightRule();
    virtual bool init() override;
    CREATE_FUNC(EightRule);
    
public:
    void open(Node* parent, Vec2 vec, int zorder, int tag);
    void close();
    void selectItemCallback(Ref* pSender, PageView::EventType type);
    
    void handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type);
    void setData(int data[7],long long note);
    void createPage();
    
private:
    PageView* m_pageView;
    ImageView* m_select;
    ImageView* imgBg;
    int m_curIndex;
    int m_nLotteryRate[7];
    long long m_Note;
};

#endif /* EightRule_hpp */
