#ifndef _TOPMENU_FACTORY_H_
#define _TOPMENU_FACTORY_H_

#include "IMenu.h"

/*

*/
class TopMenuFactory {
public:
    static IMenu* createWithVersion();
};
#endif //_TOPMENU_FACTORY_H_