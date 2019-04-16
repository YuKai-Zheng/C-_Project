#ifndef _IBACKGROUND_H_
#define _IBACKGROUND_H_

#include "cocos2d.h"
#include <string>

/*

*/
class BackgroundDelegate {
public:
    virtual void setTexture(std::string filename) {}
};

class IBackground :public cocos2d::Node, public BackgroundDelegate {
public:
    virtual void setTexture(std::string filename) {}
};

#endif //_IBACKGROUND_H_
