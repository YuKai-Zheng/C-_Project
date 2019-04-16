#ifndef _IGAMEGIFT_H_
#define _IGAMEGIFT_H_

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"

#include "HNLobby/component/delegate/BaseDelegate.h"
USING_NS_CC;
/*

*/
class GameGiftDelegate : public BaseDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

class IGameGift :public HNLayer, public GameGiftDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void setBgPath(std::string path) {}
    virtual void close() {}
};

#endif //_IGAMESET_H_
