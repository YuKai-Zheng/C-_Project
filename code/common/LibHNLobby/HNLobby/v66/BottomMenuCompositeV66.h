#ifndef  _BOTTOM_MENU_COMPOSITE_V66_H_
#define _BOTTOM_MENU_COMPOSITE_V66_H_

#include "cocos2d.h"
#include "HNLobby/PlatformDefine.h"
#include "../pattern/factory/menu/IMenu.h"
#include "../component/delegate/BaseDelegate.h"
#include "../component/delegate/MenuDelegate.h"

/*

*/
class BottomMenuCompositeV66 : public IMenu, MoneyChangeNotify {
public:
	BottomMenuCompositeV66();
	~BottomMenuCompositeV66();
	virtual bool init()override;
	CREATE_FUNC(BottomMenuCompositeV66);

public:
    virtual void walletChanged(LLONG money)override;
    virtual void moveOut(bool isOut)override;
    virtual void autoMove()override {}
	virtual void openNewYearActivity();    
	virtual void update(float dt)override;

    virtual void setQuickBtn(bool show); //设置快速加入是否显示
	void activityTime(float dt);
    void menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type);
    virtual void setOpenBankFunc()override;
private:
    bool                        _isQuickTouch;
    bool						_isTouch;
	Node*					_csbRoot;
    BaseDelegate* layerDelegate;
	Button*			_turnTable;
    std::vector<Button*>    m_bottomButton;

	Text* l_onlinePeople;
    
    bool                    m_needTipAnimation;
	LLONG m_money;

	UserHead* userHead;
	Text* l_userID;
	Text* l_userName;
	Button*copy;
	Button*tuijian;
	Sprite* m_MoneyBg;
	Text* l_userMoney;
	Sprite* m_NameBg;
	Sprite* m_GameTipSprite;

	vector<Vec2> m_vOldPos;
private:
    //下方按钮回调
   // void menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type);

	//转盘是否显示消息回调
	bool turnplateForwardSelector(HNSocketMessage* socketMessage);
	//更新在线人数
	bool updateOnlineUserCount(HNSocketMessage* socketMessage);
	//更新推荐人功能
	bool updateShowTuiJianRenModule(HNSocketMessage* socketMessage);
	void updateTopMenuShow(MenuType type);
	void headClickCallback(Ref *pSender, Widget::TouchEventType type);
	void canTouchHead(bool isTouch);
};

#endif  //_BOTTOM_MENU_COMPOSITE_V66_H_
