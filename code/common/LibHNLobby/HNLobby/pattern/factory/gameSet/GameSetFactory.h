#ifndef _GAME_SET_FACTORY_H_
#define _GAME_SET_FACTORY_H_

#include "IGameSet.h"

/*

*/
class GameSetFactory {
public:
    static IGameSet* createWithVersion();
};
#endif //_GAME_SET_FACTORY_H_