#ifndef __HN_HNGameUIBase_H__
#define __HN_HNGameUIBase_H__

#include "HNBaseType.h"
#include "UI/Base/HNLayer.h"

namespace HN {
class HNGameUIBase: public HNLayer {
public:
    HNGameUIBase();
    virtual ~HNGameUIBase();

public:
    void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

};
}

#endif