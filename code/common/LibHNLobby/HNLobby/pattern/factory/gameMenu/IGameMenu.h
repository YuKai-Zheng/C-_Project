#ifndef _IGAMEMENU_H_
#define _IGAMEMENU_H_

#include "cocos2d.h"
#include "HNLobby/component/delegate/LoginDelegate.h"
#include "HNLobby/component/delegate/RegisterDelegate.h"
USING_NS_CC;

/*

*/
class IGameMenu :public cocos2d::Node {
    CC_SYNTHESIZE(LoginDelegate*, m_loginDelegate, LoginDelegate);
    CC_SYNTHESIZE(RegisterDelegate*, m_registerDelegate, RegisterDelegate);

public:
    virtual void handleKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)=0;
};

#endif //_IGAMEMENU_H_
