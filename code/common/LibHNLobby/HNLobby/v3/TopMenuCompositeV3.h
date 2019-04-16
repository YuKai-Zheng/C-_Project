#ifndef _TOP_MENU_COMPOSITE_V3_H_
#define _TOP_MENU_COMPOSITE_V3_H_

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
//#include "../v1/TopMenuComposite.h"

USING_NS_CC;
using namespace cocos2d::ui;

/*

*/
class TopMenuCompositeV3 : public IMenu, public MoneyChangeNotify, public HNSpreadLogic::Delegate {

public:
    TopMenuCompositeV3();
    virtual bool init()override;
    CREATE_FUNC(TopMenuCompositeV3);

public:
    virtual void moveOut(bool isOut) override;
    virtual void setType(MenuType type)override;
    virtual void autoMove()override;
    virtual void setLogoTexture(std::string filename);
    virtual void setShowBank(bool isShow)override;
    virtual void setGameDeskLayer(GameDesk* gameDesk)override;
    virtual void setRoomInfo(ComRoomInfo* roomInfo)override;
    //是否显示银行、充值按钮
    virtual void controlChargeAndBankBtnPosition(bool openCharge, bool openBank, bool openrank, bool forceHide = false)override;
    virtual void setBacksTageOpen(BacksTageOpen open);
    //更新钱包金币
    virtual void walletChanged(LLONG money)override;

public:
    //上方按钮回调
    void menuLobbyTopCallBack(Ref *pSender, Widget::TouchEventType type);
    void canDisplayButton(Node *node, bool isDisplay);
    void getRoomSpriteByRoomName(char name[], char fishName[], char* gameName);
    //新春活动
    void newYearActivityCallBack(Ref* pSender, Widget::TouchEventType type);

    GameRoom*           _gameRoom;
    GameDesk*           _gameDesk;
    ComRoomInfo *    roomInfo;

private:
    // 推广逻辑类
    HNSpreadLogic       _spreadLogic;

    Vec2 _orignalChargePosition;
    Vec2 _orignalBankPosition;

    bool				_isTouch;
    bool isShowBank;

    void	checkGiftMessages();

    //struct TOP
    //{
    //	Layout*				Panel_top;

    //	Button*				Button_return;
    //	Button*				Button_bank;
    //	Button*				Button_charge;
    //	Button*				Button_exit;
    //	Button*				Button_set;
    //	Button*				Button_quickly;
    //	Button*				Button_newYear;
    //	Button*				Button_return;
    //	Button*				Button_rankList;

    //	Sprite*				Sprite_LogoSpr;
    //	Sprite*				Sprite_platformLogo;
    //	Sprite*				Sprite_topTitle;
    //	Sprite*				Sprite_TypeSpr;
    //	Sprite*				Sprite_FishBiLi;
    //
    //}_lobbyTop;

    BacksTageOpen _backsTageOpen;
    //LOBBY_TOP _lobbyTop;

protected:
    LOBBY_TOP _lobbyTop;
};

#endif //_TOP_MENU_COMPOSITE_V3_H_
