#ifndef _GAME_TYPE_COMPOSITE_V66_H_
#define _GAME_TYPE_COMPOSITE_V66_H_

//#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameType/IGameType.h"
#include "../component/delegate/MenuDelegate.h"
#include "../component/delegate/PlatformDelegate.h"

/*

*/
class GameTypeCompositeV66 : public IGameType {
    CC_SYNTHESIZE(MenuDelegate*, topMenudelegate, TopMenuDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);

public:
    virtual bool init()override;
    CREATE_FUNC(GameTypeCompositeV66);

private:
    void createTypeListLayer();
};

#endif //_GAME_TYPE_COMPOSITE_V2_H_
