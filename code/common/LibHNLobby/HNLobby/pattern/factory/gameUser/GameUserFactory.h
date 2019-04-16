#ifndef _GAME_USER_FACTORY_H_
#define _GAME_USER_FACTORY_H_

#include "IGameUser.h"

/*

*/
class GameUserFactory {
public:
    static IGameUser* createWithVersion();
};
#endif //_GAME_USER_FACTORY_H_