#ifndef  _BOTTOM_MENU_COMPOSITE_H_
#define _BOTTOM_MENU_COMPOSITE_H_

#include "cocos2d.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformMessage.h"
#include "../pattern/factory/menu/IMenu.h"

/*

*/
class BottomMenuComposite : public IMenu, public MoneyChangeNotify {
public:
    BottomMenuComposite();
    virtual bool init()override;
    CREATE_FUNC(BottomMenuComposite);

public:
    virtual void moveOut(bool isOut)override;
    virtual void autoMove()override;
    virtual void walletChanged(LLONG money)override;

public:
    bool switchToLayoutControl(Layout *layout, const std::string &controlName, Node *newControl);
    virtual void update(float dt)override;
private:
    LOBBY_BOTTOM				_lobbyBotttom;

    bool						_isTouch;

    LLONG m_money;

    //下方按钮回调
    void menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type);
    //截屏保存
    void screenShoot();
    // 友盟
    void openUMengShare();
    //卸载奖励监听
    void unSocketCallBack();


};

#endif  //_BOTTOM_MENU_COMPOSITE_H_
