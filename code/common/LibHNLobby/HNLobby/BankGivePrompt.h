#ifndef __BANKGIVE_PROMPT_H__
#define __BANKGIVE_PROMPT_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UI/Base/AUIButton.h"

USING_NS_CC;
using namespace cocostudio;
using namespace HN;

typedef std::function<void()> OkCallBack;

class BankGivePrompt : public HNLayer {
private:
	Node*				 _loader;
	Layout*				 _layout;

	Button*				 _btnContinue;
	Button*				 _btnQuit;

	ImageView *		 _backGround;
	
	Text*					 _targetId;
	Text*					 _targetMoney;
	Text*					 _time;
	Text*					 _money;
	Text*					 _id;
private:
	void closePrompt();
public:
	virtual bool init() override;
	void ButtonCallBack(Ref* pSender, Widget::TouchEventType  type);
	void callBackNo();
	CREATE_FUNC(BankGivePrompt);
public:
	void updateInfo(std::string nickName, LLONG targetMoney, std::string  time, INT id);

	OkCallBack _okCallBack;
};

#endif // __GAMEXIT_LAYER_H__