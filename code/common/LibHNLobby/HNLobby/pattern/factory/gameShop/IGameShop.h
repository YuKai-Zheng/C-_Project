#ifndef _IGAMESHOP_H_
#define _IGAMESHOP_H_

#include "cocos2d.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/component/delegate/BaseDelegate.h"
#include "HNLobby/component/delegate/MenuDelegate.h"
#include "UI/Base/HNLayer.h"
USING_NS_CC;

/*

*/
class GameShopDelegate : public BaseDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

class IGameShop :public HNLayer, public GameShopDelegate {
public:
    virtual void setMenuDelegate(MenuDelegate* delegate) = 0;

    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}

protected:
    MenuDelegate* _delegate;
};

#endif //_IGAMESHOP_H_
