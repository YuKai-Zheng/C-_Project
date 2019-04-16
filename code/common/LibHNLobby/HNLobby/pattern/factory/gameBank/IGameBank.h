#ifndef _IGAMEBANK_H_
#define _IGAMEBANK_H_

#include "HNLobby/PlatformDefine.h"
#include "UI/Base/HNLayer.h"
#include "HNLobby/component/delegate/BaseDelegate.h"
#include "HNLobby/component/delegate/MenuDelegate.h"

/*

*/
class GameBankDelegate : public BaseDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

class IGameBank :public HNLayer, public GameBankDelegate {
public:
    virtual void setMenuDelegate(MenuDelegate* delegate) = 0;
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}

protected:
    MenuDelegate* _delegate;
};

#endif //_IGAMEBANK_H_
