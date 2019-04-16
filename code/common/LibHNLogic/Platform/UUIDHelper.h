#pragma once
#include "cocos2d.h"
#include <string>

namespace HN {
class UUIDHelper {
public:
    UUIDHelper();
    ~UUIDHelper();

    static std::string getUUID();
};
}
