#ifndef _BACKGROUND_FACTORY_H_
#define _BACKGROUND_FACTORY_H_

#include "IBackground.h"

/*

*/
class BackgroundFactory {
public:
    static IBackground* create(std::string filename);
    static IBackground* createWithVersion();
};
#endif //_BACKGROUND_FACTORY_H_