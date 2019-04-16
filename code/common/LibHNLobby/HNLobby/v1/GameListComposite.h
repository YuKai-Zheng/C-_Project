#ifndef _GAME_LIST_COMPOSITE_H_
#define _GAME_LIST_COMPOSITE_H_

#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameList/IGameList.h"

/*

*/
class GameListComposite : public IGameList {
public:
    GameListComposite();
    virtual bool init()override;
    CREATE_FUNC(GameListComposite);

public:
    virtual void updateGameUserCount(UINT Id, UINT count)override;

public:
    bool enterTimeMatch();
    void fastEnterRoom();
    void checkTimeMatchStart(float dt);
    //请求房间列表定时器回调
    void requestGameRoomTimerCallBack(float dt);


private:
    INT						_iValue;
    bool					_isTouch;
    GameLists* gamesLayer;
};

#endif //_GAME_LIST_COMPOSITE_H_
