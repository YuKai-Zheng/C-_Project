#ifndef  _BOTTOM_MENU_COMPOSITE_V88_H_
#define _BOTTOM_MENU_COMPOSITE_V88_H_

#include "cocos2d.h"
#include "HNLobby/PlatformDefine.h"
#include "../pattern/factory/menu/IMenu.h"
#include "../component/delegate/BaseDelegate.h"
#include "../component/delegate/MenuDelegate.h"

#define btn_join_tag        "quickJoin"
#define btn_bank_tag        "bank"
#define btn_rank_tag        "rank"
#define btn_activity_tag    "lucky"
#define btn_set_tag         "set"
#define btn_charge_tag      "shopBtn"
#define btn_invite_tag		"yaoqing"
#define btn_back_tag		"tuichu"
/*

*/
class BottomMenuCompositeV88 : public IMenu, MoneyChangeNotify {
public:
	BottomMenuCompositeV88();
	~BottomMenuCompositeV88();
	virtual bool init()override;
	CREATE_FUNC(BottomMenuCompositeV88);

public:
    virtual void walletChanged(LLONG money)override;
    virtual void moveOut(bool isOut)override;
    virtual void autoMove()override {}
	virtual void openNewYearActivity();

	void activityTime(float dt);

private:
    bool						_isTouch;
	Node*					_csbRoot;
    BaseDelegate* layerDelegate;
	Button*			_turnTable;
    std::vector<Button*>    m_bottomButton;

	Text* l_onlinePeople;
    
    bool                    m_needTipAnimation;
    
private:
    //下方按钮回调
    void menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type);

	//转盘是否显示消息回调
	bool bankerForwardSelector(HNSocketMessage* socketMessage);
	//银行到账提示标记
	bool updateBankTipTag(HNSocketMessage* socketMessage);
	//更新在线人数
	bool updateOnlineUserCount(HNSocketMessage* socketMessage);
	//更新推荐人功能
	bool updateShowTuiJianRenModule(HNSocketMessage* socketMessage);
};

#endif  //_BOTTOM_MENU_COMPOSITE_V88_H_
