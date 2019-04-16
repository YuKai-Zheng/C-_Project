#ifndef _GAME_LIST_DELEGATE_H_
#define _GAME_LIST_DELEGATE_H_
#include "BaseDelegate.h"
#include "HNBaseType.h"
/*

*/
class GameListDelegate:public BaseDelegate {
public:
    //更新游戏在线人数
    virtual void updateGameUserCount(UINT Id, UINT count) {};

    virtual void closeGameList() {};
    virtual void openGameList() {};
    virtual void openGameTypeList() {}
};

#endif //_GAME_LIST_DELEGATE_H_
