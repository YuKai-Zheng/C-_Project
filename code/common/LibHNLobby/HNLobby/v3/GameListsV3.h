#ifndef _GameListsV3_V3_H__
#define _GameListsV3_V3_H__

#include "../GameLists.h"

class GameListsV3 : public GameLists {
public:
    CREATE_FUNC(GameListsV3);
    virtual bool init() override;

protected:
    virtual void addAnimation(INT gameId, Button* parent) override;

};
#endif // GameListsV3_h__
