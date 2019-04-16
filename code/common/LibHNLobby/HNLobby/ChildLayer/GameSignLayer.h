#ifndef __GAMESIGN_LAYER_H__
#define __GAMESIGN_LAYER_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameSignLayer : public HNLayer
{
	struct signUi
	{
		ImageView*		ImageView_SignBG;
		ImageView*		ImageView_Light;
		Text*			Text_Prompt;
		Text*			Text_Money;

		Button*			Button_Get;

	}_SignUi;

public:
	typedef std::function<void (LLONG money)> UpdataUserMoney;
	UpdataUserMoney onUpdataUserMoney;

	typedef std::function<void ()> CloseCallBack;
	CloseCallBack	onCloseCallBack;

public:
	GameSignLayer();
	virtual ~GameSignLayer();

public:
	virtual bool init() override;

	void closeSign();

	//鎸夐挳鍥炶皟
	void buttonEventCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	//鏌ヨ绛惧埌淇℃伅
	void checkSignData();
	//鐢宠鏌ヨ绛惧埌淇℃伅鍥炶皟
	bool checkSignDataEventSelector(HNSocketMessage* socketMessage);

private:
	//鐢宠棰嗗彇绛惧埌濂栧姳鍥炶皟
	bool getSignRewardEventSelector(HNSocketMessage* socketMessage);

public:
	CREATE_FUNC(GameSignLayer);
};

#endif // __GAMESIGN_LAYER_H__