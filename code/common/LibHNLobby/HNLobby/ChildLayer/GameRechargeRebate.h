#ifndef __GAMEREBATE_LAYER_H__
#define __GAMEREBATE_LAYER_H__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameRechargeRebate : public HNLayer
{
	struct RebateUi
	{
		ImageView*		ImageView_RebateBG;
		Text*			Text_RechMoney;
		Text*			Text_RebateMoney;

		Button*			Button_Get;

	}_RebateUi;

public:
	typedef std::function<void (LLONG money)> UpdataUserMoney;
	UpdataUserMoney onUpdataUserMoney;

	typedef std::function<void ()> CloseCallBack;
	CloseCallBack	onCloseCallBack;

public:
	GameRechargeRebate();
	virtual ~GameRechargeRebate();

public:
	virtual bool init() override;

	void closeRebate();

	//鎸夐挳鍥炶皟
	void buttonEventCallBack(Ref* pSender, Widget::TouchEventType type);

private:
	//鏌ヨ濂栧姳淇℃伅
	void checkRebateData();
	//鐢宠鏌ヨ濂栧姳淇℃伅鍥炶皟
	bool checkRebateDataEventSelector(HNSocketMessage* socketMessage);

private:
	//鐢宠棰嗗彇杩斿埄濂栧姳鍥炶皟
	bool getRebateEventSelector(HNSocketMessage* socketMessage);

public:
	CREATE_FUNC(GameRechargeRebate);
};

#endif // __REBATE_LAYER_H__

