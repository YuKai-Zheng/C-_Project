
#ifndef  __GAME_MusicEffectV66_H__
#define __GAME_MusicEffectV66_H__

#include "UI/Base/HNLayer.h"
#include"cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HNLobby/pattern/factory/gameSet/IGameSet.h"

USING_NS_CC;

using namespace cocos2d::extension;
using namespace ui;

class MusicEffectV66 : public IGameSet
{

private:
	//ControlSwitch *			_controlSwitchMusic;
	//ControlSwitch *			_controlSwitchEffect;

	Button *			_musicBtn;
	Button *			_effectBtn;
	bool	m_bMusicOnOff;
	bool	m_bEffectOnOff;
	
    
    std::string             m_bgPath;
public:
	MusicEffectV66();
	~MusicEffectV66();

    virtual bool init() override;
	CREATE_FUNC(MusicEffectV66);
    
    ControlSwitch* createSwitch(Vec2 vPos, int nType);
	void callbackSwitchMusic(Ref* pSender, Control::EventType event);
	void callbackSwitchMusicEffectV66(Ref* pSender, Control::EventType event);
	void onSwitchMusic();
	void onSwitchEffect();
    void openWindow(Ref* pSender, Widget::TouchEventType touchtype);
public:
	virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
	virtual void close() override;
    virtual void setBgPath(std::string path) override;
};



#endif
