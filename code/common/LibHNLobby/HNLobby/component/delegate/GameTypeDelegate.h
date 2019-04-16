#ifndef _GAME_TYPE_DELEGATE_H_
#define _GAME_TYPE_DELEGATE_H_
#include "BaseDelegate.h"
#include "HNBaseType.h"
#include "PlatformDelegate.h"
#include "MenuDelegate.h"
/*

*/
class GameTypeDelegate:public BaseDelegate {
    CC_SYNTHESIZE(PlatformDelegate*, m_delegate, PlatformDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
public:
    virtual void typeNoticeList(const std::string message) {}   //only for "GameTypeCompositeV99.h"
};

#endif //_GAME_TYPE_DELEGATE_H_
