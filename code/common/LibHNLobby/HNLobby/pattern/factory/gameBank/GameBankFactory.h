#ifndef _GAME_BANK_FACTORY_H_
#define _GAME_BANK_FACTORY_H_

#include "IGameBank.h"

/*

*/
class GameBankFactory {
public:
    static IGameBank* createWithVersion();
};
#endif //_GAME_BANK_FACTORY_H_