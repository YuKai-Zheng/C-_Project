#include "D21BuyDialogV99.h"
//#include "D21UpgradeMessage.h"

namespace D21
{
	BuyDialogV99* BuyDialogV99::create(Node* parent, LLONG max, LLONG min, LLONG plan, LLONG total, BYTE deskStation)
	{
		BuyDialogV99* dialog = new BuyDialogV99();
		if(dialog->init(parent, max, min, plan, total, deskStation))
		{
			dialog->ignoreAnchorPointForPosition(false);
			dialog->autorelease();
			return dialog;
		}
		else
		{
			CC_SAFE_DELETE(dialog);
			return nullptr;
		}
	}

	bool BuyDialogV99::init(Node* parent, LLONG max, LLONG min, LLONG plan, LLONG total, BYTE deskStation)
	{
		if(!HNDialogBase::init(parent))
		{
			return false;
		}

		auto distpatch = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(BuyDialogV99::onTouchBegan,this);
		listener->setSwallowTouches(true);
		distpatch->addEventListenerWithSceneGraphPriority(listener,this);

		_max = max;
		_min = min;
		_plan = plan > total ? total : plan;
		_total = total;
		_deskStation = deskStation;

		//_widget = GUIReader::getInstance()->widgetFromJsonFile("dzpoker/BuyDialogV99/BuyDialogV99.json");
		_widget = GUIReader::getInstance()->widgetFromJsonFile("");

		locateMid(_widget);
		this->addChild(_widget);

		resetLayout();

		return true;
	}

	bool BuyDialogV99::onTouchBegan(Touch *touch, Event *unused_event)
	{
		return false;
	}

	void  BuyDialogV99::sliderChangedCallback(Ref* pRef,ui::Slider::EventType eventType)
	{
		if(eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			LLONG min = _min;
			LLONG max = _total > _max ? _max : _total;
			int percent = _slider->getPercent();
			_plan = min + LLONG(percent / 100.0 * (max - min));
			char str[50] = {0};
			sprintf(str,"%lld",_plan);
			_planTextAtlas->setString(str);
		}
	}

	void BuyDialogV99::sendMoneyPlan()
	{
		//TSubMoney cmd;
		//cmd.bDeskStation = _deskStation;
		//cmd.nMoney[_deskStation] = _plan;
		//RoomLogic()->sendData(MDM_GM_GAME_NOTIFY,ASS_SUB_MONEY, (CHAR*)&cmd, sizeof(cmd));
	}

	void BuyDialogV99::sendStandUp()
	{
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
	}

	void  BuyDialogV99::submitCallback(Ref* pSender, Widget::TouchEventType type)
	{
		if (ui::Widget::TouchEventType::ENDED != type)
		{
			return ;
		}
		auto btn = (ui::Button*)pSender;
		if (btn == _btnMax)				//直接按最大携带
		{
			if (_total >= _max)
			{
				_plan = _max;
				sendMoneyPlan();
				hide();
			}
		}
		else if (btn == _btnMin)		//直接按最小携带
		{
			if (_total >= _min)
			{
				_plan = _min;
				sendMoneyPlan();
				hide();
			}

		}
		else if (btn == _btnOk)			//提交按钮
		{
			sendMoneyPlan();
			hide();
		}
		else if (btn == _btnCancle)		//取消
		{
			sendStandUp();
			hide();
		}
	}


	void BuyDialogV99::resetLayout()
	{
		LLONG min = _min;
		LLONG max = _total > _max ? _max : _total;
		char str[50] = {0};

		_allMoney = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(_widget, "currentmoney"));

		_slider = dynamic_cast<Slider*>(Helper::seekWidgetByName(_widget, "slider"));
		_slider->addEventListener(CC_CALLBACK_2(BuyDialogV99::sliderChangedCallback,this));

		_btnMax = dynamic_cast<Button*>(Helper::seekWidgetByName(_widget, "max"));
		_btnMax->addTouchEventListener(CC_CALLBACK_2(BuyDialogV99::submitCallback,this));
		sprintf(str,"%lld", max);
		_btnMax->setTitleText(str);

		_btnMin = dynamic_cast<Button*>(Helper::seekWidgetByName(_widget, "min"));
		_btnMin->addTouchEventListener(CC_CALLBACK_2(BuyDialogV99::submitCallback,this));
		sprintf(str,"%lld",min);
		_btnMin->setTitleText(str);

		_btnOk = dynamic_cast<Button*>(Helper::seekWidgetByName(_widget, "ok"));
		_btnOk->addTouchEventListener(CC_CALLBACK_2(BuyDialogV99::submitCallback,this));

		_btnCancle = dynamic_cast<Button*>(Helper::seekWidgetByName(_widget, "cancel"));
		_btnCancle->addTouchEventListener(CC_CALLBACK_2(BuyDialogV99::submitCallback,this));

		_planTextAtlas = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(_widget, "money"));


		sprintf(str,"%lld",_total);
		_allMoney->setString(str);

		sprintf(str,"%lld",min);
		_planTextAtlas->setString(str);

		_slider->setPercent((_plan - min) * 1.0f / (max - min));
	}
}
