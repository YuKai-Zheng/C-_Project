#ifndef _GAME_SHOP_FACTORY_H_
#define _GAME_SHOP_FACTORY_H_

#include "IGameShop.h"

/*

*/
class GameShopFactory {
public:
    static IGameShop* createWithVersion();
};
#endif //_GAME_SHOP_FACTORY_H_