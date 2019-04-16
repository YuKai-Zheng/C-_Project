#ifndef _TOP_MENU_COMPOSITE_H_
#define _TOP_MENU_COMPOSITE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UserHead.h"
#include "HNLobby/pattern/Composite.h"
#include "HNPlatformLogic/HNPlatformSpreadLogic.h"
#include "HNLobby/PlatformMessage.h"
#include "HNNetProtocol/HNComStruct.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GameDesk.h"
#include "../pattern/factory/menu/IMenu.h"
#include "../component/delegate/PlatformDelegate.h"

USING_NS_CC;
using namespace cocos2d::ui;

/*

*/
class TopMenuComposite : public IMenu,public MoneyChangeNotify, public HNSpreadLogic::Delegate {


public:
    TopMenuComposite();
    virtual bool init()override;

    CREATE_FUNC(TopMenuComposite);

public:
    virtual void moveOut(bool isOut)override ;
    virtual void autoMove()override;
    virtual void setType(MenuType type)override;
    virtual void setShowBank(bool isShow)override;
    virtual void setGameDeskLayer(GameDesk* gameDesk)override;
    virtual void setRoomInfo(ComRoomInfo* roomInfo)override;
    //是否显示银行、充值按钮
    virtual void controlChargeAndBankBtnPosition(bool openCharge, bool openBank, bool openrank, bool forceHide = false)override;
    //更新钱包金币
    virtual void walletChanged(LLONG money)override;

public:
    //上方按钮回调
    void menuLobbyTopCallBack(Ref *pSender, Widget::TouchEventType type);
    void canDisplayButton(Node *node, bool isDisplay);
    void getRoomSpriteByRoomName(char name[], char fishName[], char* gameName);

    GameRoom*           _gameRoom;
    GameDesk*           _gameDesk;
    ComRoomInfo *		roomInfo;

private:
    // 推广逻辑类
    HNSpreadLogic       _spreadLogic;

    Vec2 _orignalChargePosition;
    Vec2 _orignalBankPosition;

    bool				_isTouch;
    bool isShowBank;

    void	checkGiftMessages();
    void	newYearActivityCallBack(Ref* pSender, Widget::TouchEventType type);

protected:
    LOBBY_TOP _lobbyTop;
};

#endif //_TOP_MENU_COMPOSITE_H_
