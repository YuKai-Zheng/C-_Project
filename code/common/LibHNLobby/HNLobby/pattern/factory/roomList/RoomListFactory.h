#ifndef _ROOMLIST_FACTORY_H_
#define _ROOMLIST_FACTORY_H_

#include "IRoomList.h"

/*

*/
class RoomListFactory {
public:
    static IRoomList* createWithVersion();
};
#endif //_ROOMLIST_FACTORY_H_