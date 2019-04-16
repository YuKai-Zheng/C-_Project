#ifndef  JNI_TEST_H
#define  JNI_TEST_H

#include  "cocos2d.h"
using namespace cocos2d;

void setPackageName(const char *packageName) {
    CCLog("packageName: %s", packageName);
}

void setUUID(const char *UUID) {
    CCLog("UUID: %s", UUID);
    UserDefault::getInstance()->setStringForKey("UUID", UUID);
}

#endif
