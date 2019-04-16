#ifndef _GAMEMMENU_FACTORY_H_
#define _GAMEMMENU_FACTORY_H_

#include "IGameMenu.h"
/*

*/
class GameMenuFactory {
public:
    static IGameMenu* createWithVersion();
};
#endif //_GAMEMMENU_FACTORY_H_