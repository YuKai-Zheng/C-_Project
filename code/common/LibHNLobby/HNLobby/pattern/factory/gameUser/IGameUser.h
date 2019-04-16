#ifndef _IGAMEUSER_H_
#define _IGAMEUSER_H_

#include "cocos2d.h"
#include <functional>
#include <string>
#include "HNLobby/component/delegate/BaseDelegate.h"
#include "UI/Base/HNLayer.h"

USING_NS_CC;
/*

*/
class GameUserDelegate : public BaseDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

class IGameUser :public HNLayer, public GameUserDelegate {
    typedef std::function<void()> UpdateHeadCallBack;
    typedef std::function<void(std::string nickName)> UpdateNicknameCallBack;

public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}

public:
    UpdateHeadCallBack		 updateHeadCallBack;
    UpdateNicknameCallBack	 updateNicknameCallBack;

};

#endif //_IGAMEUSER_H_
