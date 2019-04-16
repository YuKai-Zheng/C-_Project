#ifndef _BOTTOMMENU_FACTORY_H_
#define _BOTTOMMENU_FACTORY_H_

#include "IMenu.h"

/*

*/
class BottomMenuFactory {
public:
    static IMenu* createWithVersion();
};
#endif //_BOTTOMMENU_FACTORY_H_