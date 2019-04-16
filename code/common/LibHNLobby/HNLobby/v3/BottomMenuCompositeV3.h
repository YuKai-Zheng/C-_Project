#ifndef  _BOTTOM_MENU_COMPOSITE_V3_H_
#define _BOTTOM_MENU_COMPOSITE_V3_H_

#include "cocos2d.h"
#include "HNLobby/PlatformDefine.h"
#include "../pattern/factory/menu/IMenu.h"

/*

*/
class BottomMenuCompositeV3 : public IMenu, public MoneyChangeNotify {
public:
    BottomMenuCompositeV3();
    ~BottomMenuCompositeV3();
    virtual bool init()override;
    CREATE_FUNC(BottomMenuCompositeV3);
public:
    virtual void walletChanged(LLONG money)override;
    void upDatePlatformMoneyFunc(LLONG money);
    virtual void moveOut(bool isOut)override;
    void autoMove()override;
    virtual void setType(MenuType type)override;
    virtual void setLogoTexture(std::string filename);
    virtual void setShowBank(bool isShow)override;
    virtual void setGameDeskLayer(GameDesk* gameDesk)override;
    virtual void setRoomInfo(ComRoomInfo* roomInfo)override;
    //是否显示银行、充值按钮
    virtual void controlChargeAndBankBtnPosition(bool openCharge, bool openBank, bool openrank, bool forceHide = false)override;
//    virtual void setBacksTageOpen(BacksTageOpen open)override;
    void update(float dt)override;
public:
    bool switchToLayoutControl(Layout *layout, const std::string &controlName, Node *newControl);

private:

    bool						_isTouch;
    LOBBY_BOTTOM _lobbyBotttom;
    //存储 大厅个人金币值
    LLONG m_money;
//    BacksTageOpen _backsTageOpen;

    //下方按钮回调
    void menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type);
    ////截屏保存
    //void screenShoot();
    //// 友盟
    //void openUMengShare();
    ////卸载奖励监听
    //void unSocketCallBack();
};

#endif  //_BOTTOM_MENU_COMPOSITE_V3_H_
