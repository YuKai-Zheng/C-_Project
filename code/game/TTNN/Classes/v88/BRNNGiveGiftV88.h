#ifndef __BRNNPlayerGiftV88_h__
#define __BRNNPlayerGiftV88_h__


#include "HNNetExport.h"
#include "BRNNMakeBlackV88.h"
#include "BRNNResV88.h"

#include "cocos2d.h"

USING_NS_CC;

namespace BRNN {
class GiveGiftV88 : public MakeBlackV88 {
private:
UserInfoStruct*             _user;
Vector<Sprite*>             _vecGiftSprite;

int _giftPriceArr[6];

public:
static GiveGiftV88* create(UserInfoStruct* user);

public:
GiveGiftV88();
virtual ~GiveGiftV88();

public:
virtual bool init(UserInfoStruct* user);

virtual void onTouchEnded(Touch *touch, Event *unused_event);
};

}

#endif // __BRNNPlayerGift_h__