#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "cocos2d.h"
#include "network/HttpClient.h"

#include "HNLobby/pattern/BaseState.h"
USING_NS_CC;
using namespace cocos2d::network;

/*
包含注册和登录的菜单状态类
*/
class MenuState : public BaseState {
public:
    void enter()override;
    void exit()override;
    void execute()override;

    virtual bool init()override;
    CREATE_FUNC(MenuState);
    
};

#endif //_MENU_STATE_H_
