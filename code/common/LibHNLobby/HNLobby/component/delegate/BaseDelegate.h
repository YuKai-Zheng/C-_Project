#ifndef _BASE_DELEGATE_H_
#define _BASE_DELEGATE_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;

/*

*/
class BaseDelegate {
public:
    virtual void platformDisConnect(const std::string& message) {}
    //virtual void platformNews(const std::string& message) {}
	virtual void platformNews(int iType, const std::string &message, int iCount) {}
    virtual void platformNoticeList(const std::string message) {}
    
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

#endif //_BASE_DELEGATE_H_
