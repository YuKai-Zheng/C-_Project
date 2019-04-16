#ifndef _GoldenFlowerGAME_V88_CARD_TYPE_
#define _GoldenFlowerGAME_V88_CARD_TYPE_

#include "cocos2d.h"
#include "HNNetExport.h"
USING_NS_CC;

namespace goldenflower
{
	class GameCardTypeV88 :public HNLayer
	{
	public:
		GameCardTypeV88();
		~GameCardTypeV88();
	public:
		static GameCardTypeV88* create();
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	private:
		void closeButtonCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void animationHide();
	private:
		bool init();
	};
}



#endif // !_GAME_CARD_TYPE_
