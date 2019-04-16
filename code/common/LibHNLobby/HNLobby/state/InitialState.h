#ifndef _INITIAL_STATE_H_
#define _INITIAL_STATE_H_

#include "cocos2d.h"
#include "HNLobby/pattern/BaseState.h"
USING_NS_CC;

/*
初始状态类
*/
class InitialState : public BaseState {
public:
    void enter() override;
    void exit() override;
    void execute() override;

    virtual bool init()override;

    CREATE_FUNC(InitialState);
private:
    bool	checkPVR;
    bool	checkCSB;
};

#endif //_INITIAL_STATE_H_
