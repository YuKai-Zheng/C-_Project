
#ifndef  __GAME_MUSICEFFECT_H__
#define __GAME_MUSICEFFECT_H__

//
#include "UI/Base/HNLayer.h"
#include"cocos-ext.h"
#include "cocos2d.h"
//#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
//#include "BRNNRes.h"

USING_NS_CC;

//using namespace BRNN;
//using namespace cocostudio;
using namespace cocos2d::extension;

using namespace ui;

class MusicEffect : public HNLayer
{

private:
	ControlSwitch *			_controlSwitchMusic;
	ControlSwitch *			_controlSwitchEffect;
public:
	MusicEffect();
	~MusicEffect();

	void loadUI();
	ControlSwitch* createSwitch(Vec2 vPos, int nType);
	CREATE_FUNC(MusicEffect);

	void callbackSwitchMusic(Ref* pSender, Control::EventType event);
	void callbackSwitchMusicEffect(Ref* pSender, Control::EventType event);
private:
	virtual bool init() override;

	void show(Node* parent, int zorder, int tag);

	void close();
};



#endif
