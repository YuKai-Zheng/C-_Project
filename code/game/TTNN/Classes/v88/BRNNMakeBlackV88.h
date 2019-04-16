#ifndef __HNMakeBlackV88_h__
#define __HNMakeBlackV88_h__

/*
   为了和网络交互方便，该类继承了HNGameUIBase
   使用时在使用的对象类里定义该类的成员变量，在移除的时候方便移除
   这个类同时封装了弹出动作和关闭动作
   还做了是否滑动的判断（_bMoved）
 */


#include "HNNetExport.h"
#include "BRNNActionsV88.h"
#include "cocos2d.h"

USING_NS_CC;

class MakeBlackV88 : public HNGameUIBase {
public:
MakeBlackV88();
virtual ~MakeBlackV88();
public:
virtual bool init() override;
virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
virtual void onTouchCancelled(Touch *touch, Event *unused_event) override;

public:
void moveSelfWithAction();

private:
//LayerColor*	_black;
Point _beginPos;
private:
//void actionIn();
//void actionOut();

protected:
bool _bMoved;                                   //是否滑动
bool _bSelfMoved;                       //是否已经被移除
};

#endif // __HNMakeBlackV88_h__