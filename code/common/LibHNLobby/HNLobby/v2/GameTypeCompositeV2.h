#ifndef _GAME_TYPE_COMPOSITE_V2_H_
#define _GAME_TYPE_COMPOSITE_V2_H_

//#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameType/IGameType.h"
#include "../component/delegate/MenuDelegate.h"
#include "../component/delegate/PlatformDelegate.h"

/*

*/
class GameTypeCompositeV2 : public IGameType {
    CC_SYNTHESIZE(MenuDelegate*, topMenudelegate, TopMenuDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);

public:
    virtual bool init()override;
    CREATE_FUNC(GameTypeCompositeV2);

private:
    void createTypeListLayer();
};

#endif //_GAME_TYPE_COMPOSITE_V2_H_
