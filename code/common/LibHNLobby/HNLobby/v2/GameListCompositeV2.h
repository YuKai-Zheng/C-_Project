#ifndef _GAME_LIST_COMPOSITE_V2_H_
#define _GAME_LIST_COMPOSITE_V2_H_

#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameList/IGameList.h"

/*

*/
class GameListCompositeV2 : public IGameList {
public:
    GameListCompositeV2();
    virtual bool init()override;
    CREATE_FUNC(GameListCompositeV2);

public:
    virtual void updateGameUserCount(UINT Id, UINT count)override;
    virtual void closeGameList()override;

private:
    GameLists* gamesLayer;
};

#endif //_GAME_LIST_COMPOSITE_V2_H_
