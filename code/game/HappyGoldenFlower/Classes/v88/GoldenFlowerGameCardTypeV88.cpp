#include "GoldenFlowerGameCardTypeV88.h"
//#include "FontConfig.h"

namespace goldenflower
{
	GameCardTypeV88::GameCardTypeV88()
	{
		init();
	}

	GameCardTypeV88::~GameCardTypeV88()
	{

	}

	GameCardTypeV88* GameCardTypeV88::create()
	{
		auto* ptr = new GameCardTypeV88();
		if (nullptr != ptr)
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}


	bool GameCardTypeV88::init()
	{
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameCardTypeV88::onTouchBegan,this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

		//change size.
		//const float SCALE = 0.75f;
		auto bg = Sprite::create("v66/common/help/huanle3zhang.png");
		//bg->setScale(SCALE);
		auto size = bg->getContentSize();// *SCALE;
		bg->setGlobalZOrder(5);

		this->setContentSize(size);
		bg->setPosition(size.width/2,size.height/2);
		bg->setIgnoreAnchorPointForPosition(false);
		this->addChild(bg,1);

		//Button* closeButton = Button::create("goldenflower/table/display_card_type_close.png");
		//if (closeButton)
		//{
		//	closeButton->addTouchEventListener(CC_CALLBACK_2(GameCardTypeV88::closeButtonCallback, this));
		//	closeButton->setPosition(Vec2(size.width * 0.95f - closeButton->getContentSize().width / 2,
		//		size.height * 0.8f - closeButton->getContentSize().height / 2));
		//	this->addChild(closeButton, 2);
		//}

		return true;
	}

	bool GameCardTypeV88::onTouchBegan(Touch *touch, Event *unused_event)
	{
		auto pos = touch->getLocation();
		pos = this->convertToNodeSpace(pos);
		Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
		if(!rect.containsPoint(pos))
		{
			animationHide();
		}
		return true;
	}

	void GameCardTypeV88::closeButtonCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return;
		}

		Button* btn = (Button*)pSender;
		if (btn)
		{
			animationHide();
		}
	}

	void GameCardTypeV88::animationHide()
	{
		auto winSize = Director::getInstance()->getWinSize();
		auto size = this->getContentSize();
		auto destPos = Vec2(winSize.width + size.width, 0);
		this->stopAllActions();
		//auto moveTo = MoveTo::create(0.2f, destPos);
		auto fadeOut = FadeOut::create(0.2f);
		auto seq = Sequence::create(fadeOut, RemoveSelf::create(true), nullptr);
		this->runAction(seq);
	}

}


