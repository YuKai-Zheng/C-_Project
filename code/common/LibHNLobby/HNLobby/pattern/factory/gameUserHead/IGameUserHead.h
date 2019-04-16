#ifndef _IGAMEUSERHEAD_H_
#define _IGAMEUSERHEAD_H_

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include <functional>
#include <string>
#include "HNLobby/component/delegate/BaseDelegate.h"

USING_NS_CC;
/*

*/
class GameUserHeadDelegate : public BaseDelegate {
public:
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
};

class IGameUserHead :public HNLayer, public GameUserHeadDelegate {
    typedef std::function<void()> UpdateHeadCallBack;
    typedef std::function<void(std::string nickName)> UpdateNicknameCallBack;

public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
    virtual void open(Node* parent, Vec2 vec, int zorder, int tag) {}
    virtual void close() {}
	
public:

    UpdateHeadCallBack		 updateHeadCallBack;

};

#endif //_IGAMEUSER_H_
