
#ifndef  __GAME_MusicEffectV88_H__
#define __GAME_MusicEffectV88_H__

#include "UI/Base/HNLayer.h"
#include"cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HNLobby/pattern/factory/gameSet/IGameSet.h"

USING_NS_CC;

using namespace cocos2d::extension;
using namespace ui;

class MusicEffectV88 : public IGameSet
{

private:
	ControlSwitch *			_controlSwitchMusic;
	ControlSwitch *			_controlSwitchEffect;
    
    std::string             m_bgPath;
public:
	MusicEffectV88();
	~MusicEffectV88();

    virtual bool init() override;
	CREATE_FUNC(MusicEffectV88);
    
    ControlSwitch* createSwitch(Vec2 vPos, int nType);
	void callbackSwitchMusic(Ref* pSender, Control::EventType event);
	void callbackSwitchMusicEffectV88(Ref* pSender, Control::EventType event);
    
    void openWindow(Ref* pSender, Widget::TouchEventType touchtype);
public:
	virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
	virtual void close() override;
    virtual void setBgPath(std::string path) override;
};



#endif
