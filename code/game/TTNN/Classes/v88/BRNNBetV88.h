#ifndef __BRNNBetV88_h__
#define __BRNNBetV88_h__



#include "HNNetExport.h"
#include "BRNNResV88.h"

#include "cocos2d.h"
#include "UI/Base/HNSprite.h"
USING_NS_CC;

namespace BRNN {

class BetV88 : public HNSprite {
public:
    static BetV88* create(INT typeBet, INT typeZone, Point startPos);

public:
    BetV88(INT typeBet, INT typeZone, Point startPos);
    ~BetV88();

public:
    INT _typeBet;                    //即百，千，万，十万，百万 0-4
    INT _typeZone;                    //东南西北 0-3
    Point _startPos;                  //赢钱时飞回原位置
    Vec2 _xiazhuAreaPos = Vec2(0, 0);                   //筹码到达下注区域的位置

public:
    virtual bool init();

public:
    void moveToBetZone(Point pos);
    void moveToBetZone2(Rect rect);
    void moveBack();    //闲家赢钱，移动回初始位置
    void moveToBanker(Point pos);    //庄家赢钱，移动到庄家位置
};

}

#endif // __BRNNBet_h__
