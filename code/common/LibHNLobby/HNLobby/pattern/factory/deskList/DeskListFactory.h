#ifndef _DESKLIST_FACTORY_H_
#define _DESKLIST_FACTORY_H_

#include "IDeskList.h"
#include "HNNetProtocol/HNComStruct.h"

/*

*/
class DeskListFactory {
public:
    static IDeskList* createWithVersion(ComRoomInfo *roomInfo);
};
#endif //_DESKLIST_FACTORY_H_
