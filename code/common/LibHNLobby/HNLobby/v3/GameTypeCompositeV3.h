#ifndef _GAME_TYPE_COMPOSITE_V3_H_
#define _GAME_TYPE_COMPOSITE_V3_H_

#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameType/IGameType.h"

/*

*/
class GameTypeCompositeV3 : public IGameType {
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);

public:
    GameTypeCompositeV3();
    virtual bool init()override;
    void createTypeListLayer();
    CREATE_FUNC(GameTypeCompositeV3);

private:
    void createGameType();
    void createGameNotice();
    void handleButtonEvent(Ref* pSender, Widget::TouchEventType type);
};

#endif //_GAME_TYPE_COMPOSITE_V3_H_
