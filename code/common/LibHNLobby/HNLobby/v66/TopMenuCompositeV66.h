#ifndef _TOP_MENU_COMPOSITE_V66_H_
#define _TOP_MENU_COMPOSITE_V66_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "UI/UserHead.h"
#include "HNLobby/pattern/Composite.h"
#include "HNPlatformLogic/HNPlatformSpreadLogic.h"
#include "HNLobby/PlatformMessage.h"
#include "HNNetProtocol/HNComStruct.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GameDesk.h"
#include "../pattern/factory/menu/IMenu.h"

USING_NS_CC;
using namespace cocos2d::ui;

/*

*/
class TopMenuCompositeV66 : public IMenu, public MoneyChangeNotify, public HNSpreadLogic::Delegate {

public:
	TopMenuCompositeV66();
	~TopMenuCompositeV66();
	virtual bool init()override;
	CREATE_FUNC(TopMenuCompositeV66);

public:
    virtual void setType(MenuType type)override;
    //更新钱包金币
    virtual void walletChanged(LLONG money)override;
    virtual void update(float dt)override;
    virtual void autoMove()override;
    virtual void moveRight(bool isRight)override;
    virtual void closeChildLayer()override;

public:


    //上方按钮回调
    void menuLobbyTopCallBack(Ref *pSender, Widget::TouchEventType type);
    void canTouchHead(bool isTouch);
    void getRoomSpriteByRoomName(char name[], char fishName[], char* gameName);

	

	//银行到账提示标记
	bool updateBankTipTag(HNSocketMessage* socketMessage);
	//下方按钮回调
	void menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type);
private:
	// 推广逻辑类
	HNSpreadLogic       _spreadLogic;


	bool				_isTouch;

	Node* csb;
	std::vector<Button*>    m_bottomButton;
    
    LLONG m_money;
    
    UserHead* userHead;
    Text* l_userID;
    Text* l_userName;
    Button*copy;
    Text* l_userMoney;
    Text* l_userDimond;

private:
	bool                    m_needTipAnimation;
};

#endif //_TOP_MENU_COMPOSITE_V99_H_
