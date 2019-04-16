#ifndef __NNPokerCardV88_h__
#define __NNPokerCardV88_h__


#include "HNNetExport.h"
#include "UI/Base/HNSprite.h"

#include "cocos2d.h"
#include "UI/Base/HNSprite.h"
USING_NS_CC;

namespace NN {

class PokerCardV88 : public HNSprite {
public:
    PokerCardV88(Point startPos);
    ~PokerCardV88();

    static PokerCardV88* create(Point startPos, Point curPos, BYTE val);

    static PokerCardV88* create(Point startPos);

    void standUP();
    void sitDown();
    void setCardFileName(const std::string& filename);

    void setCardValue(BYTE val);
    BYTE getCardValue();

    //actions
    void FaMingPai(Point pos);
    void FaAnPai(Point pos);
    void FlipPoke();
    void reverseFlipPokeAndBack(float delay=8.0);

    void removeCallback();
    /*private:*/
    void ChangePoke();

    void clickCallback(Ref* pSender, Widget::TouchEventType type);
    void setCardTouchEnable(bool bTouch);
public:
    Point		_startPos;
    BYTE		_Value;
    std::function<void()> _removeCallback = nullptr;

    bool		_bIsMove;
};

}

#endif // __NNPokerCardV88_h__
