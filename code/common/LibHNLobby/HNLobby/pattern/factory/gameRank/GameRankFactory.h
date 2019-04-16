#ifndef _GAME_RANK_FACTORY_H_
#define _GAME_RANK_FACTORY_H_

#include "IGameRank.h"

/*

*/
class GameRankFactory {
public:
    static IGameRank* createWithVersion();
};
#endif //_GAME_RANK_FACTORY_H_