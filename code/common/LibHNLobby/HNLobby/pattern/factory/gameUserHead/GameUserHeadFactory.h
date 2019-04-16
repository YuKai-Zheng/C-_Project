#ifndef _GAME_USER_HEAD_FACTORY_H_
#define _GAME_USER_HEAD_FACTORY_H_

#include "IGameUserHead.h"

/*

*/
class GameUserHeadFactory {
public:
    static IGameUserHead* createWithVersion();
};
#endif //_GAME_USER_FACTORY_H_