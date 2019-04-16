#ifndef __BJLPokerCardV88_h__
#define __BJLPokerCardV88_h__


#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/Base/HNSprite.h"
USING_NS_CC;

namespace BJL {
class PokerCardV88 : public HNSprite {
public:
	PokerCardV88();
	~PokerCardV88();
	static PokerCardV88* create();
    void setStartPositin(Vec2 position);
    void standUP();
    void sitDown();
    void setCardFileName(const std::string& filename);
    void setCardFrameName(const std::string& filename);

    //获取对应的扑克图片
    std::string getCardFileName();
    void setCardValue(BYTE val);
    BYTE getCardValue();
private:
    BYTE _Value;
};
}

#endif // __BJLPokerCardV88_h__
