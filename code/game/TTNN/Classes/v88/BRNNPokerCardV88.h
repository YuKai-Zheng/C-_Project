#ifndef __BRNNPokerCardV88_h__
#define __BRNNPokerCardV88_h__



#include "HNNetExport.h"
#include "BRNNResV88.h"

#include "cocos2d.h"
#include "UI/Base/HNSprite.h"
USING_NS_CC;

namespace BRNN {

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
//发明牌
    void FaMingPai(Point pos);
//发暗牌
    void FaAnPai(Point pos);
//翻牌
    void FlipPoke();
//恢复已翻转的牌并回收
    void reverseFlipPokeAndBack(float delay=8.0);

    /*private:*/
    void ChangePoke();

public:
    Point _startPos;
    BYTE _Value;
    std::function<void()> _removeCallback = nullptr;

private:
    void removeCallback();

};

}

#endif // __BRNNPokerCard_h__
