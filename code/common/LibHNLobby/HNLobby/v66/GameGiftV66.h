
#ifndef  __GAME_GameGiftV66_H__
#define __GAME_GameGiftV66_H__
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"

#include "UI/Base/HNLayer.h"
#include"cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HNLobby/pattern/factory/gameGift/IGameGift.h"

USING_NS_CC;
#define ASS_GP_GET_GIFT 10
#define HC_GP_GETGIFT_SUC 1
#define HC_GP_GETGIFT_ERR -1
using namespace cocos2d::extension;
using namespace ui;

struct GameGiftInfo
{
	int dwUserID;
	LLONG lMoney;
};

class GameGiftV66 : public IGameGift
{

private:
	//ControlSwitch *			_controlSwitchMusic;
	//ControlSwitch *			_controlSwitchEffect;

	Node* _rootNode;
	bool	m_bMusicOnOff;
	bool	m_bEffectOnOff;
	
    
    std::string             m_bgPath;
public:
	GameGiftV66();
	~GameGiftV66();

    virtual bool init() override;
	CREATE_FUNC(GameGiftV66);
    
    //ControlSwitch* createSwitch(Vec2 vPos, int nType);
	//void callbackSwitchMusic(Ref* pSender, Control::EventType event);
	//void callbackSwitchGameGiftV66(Ref* pSender, Control::EventType event);
	void GetMoneyCallBack(Ref* pSender, Widget::TouchEventType type);
    //void openWindow(Ref* pSender, Widget::TouchEventType touchtype);
public:
	virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
	virtual void close() override;
    virtual void setBgPath(std::string path) override;

private:
	//领取金币处理函数
	bool getEventSelector(HNSocketMessage* socketMessage);
};



#endif
