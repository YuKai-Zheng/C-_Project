#ifndef _GAME_GIFT_FACTORY_H_
#define _GAME_GIFT_FACTORY_H_

#include "IGameGift.h"

/*

*/
class GameGiftFactory {
public:
    static IGameGift* createWithVersion();
};
#endif //_GAME_SET_FACTORY_H_