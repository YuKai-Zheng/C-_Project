//
//  EightSet.hpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/2.
//

#ifndef EightSet_hpp
#define EightSet_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "UI/Base/HNLayer.h"
using namespace cocos2d::ui;
using namespace cocos2d::extension;

USING_NS_CC;


class EightSet:public HNLayer  {
public:
    EightSet();
    ~EightSet();
    virtual bool init() override;
    CREATE_FUNC(EightSet);
    
public:
    void open(Node* parent, Vec2 vec, int zorder, int tag);
    void close();
    typedef std::function<void(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)> ruleCallback;
    ruleCallback _rCallBack;
    typedef std::function<void(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)> exitCallback;
    exitCallback _eCallBack;
    typedef std::function<void(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)> musicCallback;
    musicCallback _mCallBack;
    void handleBackBtnEvent(Ref* pSender, Widget::TouchEventType type);
private:
    ImageView* imgBg;
};

#endif /* EightSet_hpp */
