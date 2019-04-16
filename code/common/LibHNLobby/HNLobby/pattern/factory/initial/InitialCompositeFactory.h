#include "IInitialComposite.h"
#ifndef _INITIAL_COMPOSITE_FACTORY_H_
#define _INITIAL_COMPOSITE_FACTORY_H_


/*

*/
class InitialCompositeFactory {
public:
    static IInitialComposite* createWithVersion();
};
#endif //_INITIAL_COMPOSITE_FACTORY_H_