#ifndef __HN_GoldenToad_TOOLBAR_V88_H__
#define __HN_GoldenToad_TOOLBAR_V88_H__

//#include "HNNetExport.h"
#include "UI/Base/HNLayer.h"
#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"

namespace GoldenToad
{

	class GoldenToadGameExchangeScore;

	class GoldenToadToolbarV88 : public HN::HNLayer
	{
	public:
		static GoldenToadToolbarV88* create(Layout* toolBarBG);
		virtual bool init(Layout* toolBarBG);

		void controlTouchAction(bool canTouch);

	public:
		bool isOut() const { return _isOut; }

	protected:
		ImageView* _Toolbar;

	protected:
		GoldenToadToolbarV88();
		virtual ~GoldenToadToolbarV88();

	public:
		void menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);

	public:
		void controlChangeButtonAnimation(bool showAnimation);

	private:
		bool			_isOut;
		Layout*			_toolBarBG;
		bool			_isTouchEnd;	//
		Button*			_btnAutoFire;//这个变量仅用于长按 上分 按钮时传参数使用，其他地方不会使用到此按钮(不用时将其置空)
		Button*			_changeButton;
		Button*			_lockButton;
		bool			_hasButtonPress;//若有按钮被点击，则需要强制显示按钮，仅当没有按钮再被点击的时候，才允许启动 5s 自动隐藏的逻辑
		bool			_canTouchLockButton;//防止锁定、解锁按钮短时间内被高频切换
		bool			_canTouch;//当其为 true 时才接受点击事件

		typedef std::function<void(Button* autoBtn)> AutoFireCallBack;
		typedef std::function<void(Button* lockBtn, bool bLock, bool bNormal)> LockFishCallBack;
		typedef std::function<void(Button* changeLockFishButton)> ChangeLockFishCallBack;

	public:
		AutoFireCallBack				_onAutoFire;
		LockFishCallBack				_onLock;
		ChangeLockFishCallBack			_onChangeLockFish;

	};
}



#endif
