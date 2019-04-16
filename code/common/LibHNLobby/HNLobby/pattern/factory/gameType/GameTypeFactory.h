#ifndef _GAMETYPE_FACTORY_H_
#define _GAMETYPE_FACTORY_H_

#include "IGameType.h"

/*

*/
class GameTypeFactory {
public:
    static IGameType* createWithVersion();
};
#endif //_GAMETYPE_FACTORY_H_