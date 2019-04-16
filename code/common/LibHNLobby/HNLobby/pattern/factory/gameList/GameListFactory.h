#include "IGameList.h"
#ifndef _GAMELIST_FACTORY_H_
#define _GAMELIST_FACTORY_H_


/*

*/
class GameListFactory {
public:
    static IGameList* createWithVersion();
};
#endif //_GAMELIST_FACTORY_H_