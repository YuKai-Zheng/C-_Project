#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "HNGame/HNGameUIBase.h"
#include "../pattern/BaseState.h"

/*
游戏状态类
*/
class GameState : public BaseState {
public:
    void enter() ;
    void exit() ;
    void execute();

    bool init(HNGameUIBase* game);
    static GameState* create(HNGameUIBase* game);
private:
};

#endif //_GAME_STATE_H_