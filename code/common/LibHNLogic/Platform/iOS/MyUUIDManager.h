//#import <Foundation/Foundation.h>
//
//@interface MyUUIDManager : NSObject
//
//+(NSString *)getUUID;
//
//@end

#ifndef MyUUIDManager_hpp
#define MyUUIDManager_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"

class MyUUIDManager {
    
public:
    static std::string getUUID(std::string str);
};

#endif /* MyUUIDManager_hpp */
