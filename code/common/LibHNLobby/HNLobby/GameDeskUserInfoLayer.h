
#ifndef  __GameDeskUSERINFO_LAYER_H__
#define __GameDeskUSERINFO_LAYER_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/Base/HNNode.h"
using namespace HN;
USING_NS_CC;

class DeskUserInfoLayer :public HNNode {

private:
    struct USEINFO {
        Text *	name;
        Text*		id;
        Text*		money;
        Text*		win;
        Text*		lose;
        Sprite*	head;
        Sprite*	bg;
    } _userInfo;

    EventListenerTouchOneByOne*		touchListener;
private:
    void closePrompt();


public:

    DeskUserInfoLayer();
    ~DeskUserInfoLayer();

    virtual bool init() ;
    CREATE_FUNC(DeskUserInfoLayer);

public:

    void openLayer(Node* parent,Vec2 point , int Tag);
    void closelayer();
    float getWidth() {
        return _userInfo.bg->getContentSize().width;
    }
    float getHight() {
        return _userInfo.bg->getContentSize().height;
    }
    void updateUserData(INT deskIndex, INT bDeskStation);

private:

    Node*			_root;

};

#endif
