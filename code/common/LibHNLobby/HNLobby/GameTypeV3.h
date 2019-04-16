#ifndef _GAMETYPEV3_H__
#define _GAMETYPEV3_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GUI/CCScrollView/CCScrollView.h"
#include "UI/RotateMenu.h"
#include "UI/Base/HNLayer.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace cocos2d::network;


class GameTypeV3 : public HNLayer {
public:
    typedef std::function<void()> EnterGameCallBack;
    EnterGameCallBack onEnterGameCallBack;
    EnterGameCallBack quickEnterGameCallBack;
    EnterGameCallBack onRunActionCallBack;
    EnterGameCallBack onCloseCallBack;

public:
    GameTypeV3();
    virtual ~GameTypeV3();
    CREATE_FUNC(GameTypeV3);
    virtual bool init() override;

public:
    void closeGameTypeLayer();

private:
    // 选择游戏类型回调
    void enterGameTypeEventCallBack(Ref* pSender, Widget::TouchEventType type);
    void  enterGameCallBack(Ref* pSender, Widget::TouchEventType type);
    void animationMove(float dt);

    void addAnimationFrame(const char* str, Node* parent, const std::string name);
    vector<Sprite*>			_vecSprite;
    void initUI();
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuItem1Callback(cocos2d::Ref* pSender);
    void menuItem2Callback(cocos2d::Ref* pSender);
    void menuItem3Callback(cocos2d::Ref* pSender);
    void menuItem4Callback(cocos2d::Ref* pSender);
    void menuItem5Callback(cocos2d::Ref* pSender);
    void hideAllSprite();


    void reqOpenFuncByVersionAndPlatformq();
    void onHttpOpenFuncResponse(network::HttpClient *client,
                                network::HttpResponse *response);
private:
    vector<Button*>		_vecTypeButton;
    RotateMenu*				menu;
    bool							_isMove;
    int								m_idxCount;
    bool							_isTouch;
    int								m_typeCount;
    vector<int>				_vecGameType;
    vector<int>				_vecHotGameType;
};
#endif // _GAMETYPEV3_H__
