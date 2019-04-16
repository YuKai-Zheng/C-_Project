#ifndef _IGAMERANK_H_
#define _IGAMERANK_H_

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/component/delegate/BaseDelegate.h"

#include "UI/Base/HNLayer.h"
USING_NS_CC;
/*

*/
class GameRankDelegate : public BaseDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

class IGameRank :public HNLayer, public GameRankDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}

protected:
};

#endif //_IGAMERANK_H_