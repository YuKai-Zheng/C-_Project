#ifndef _GAME_LIST_COMPOSITE_V3_H_
#define _GAME_LIST_COMPOSITE_V3_H_

#include "../pattern/factory/gameList/IGameList.h"
#include "GameListsV3.h"
/*

*/
class GameListCompositeV3 : public IGameList {
public:
    GameListCompositeV3();
    virtual bool init()override;
    CREATE_FUNC(GameListCompositeV3);

public:
    virtual void updateGameUserCount(UINT Id, UINT count)override;
    virtual void closeGameList()override;

private:
    GameListsV3* gamesLayer;
};

#endif //_GAME_LIST_COMPOSITE_V3_H_
