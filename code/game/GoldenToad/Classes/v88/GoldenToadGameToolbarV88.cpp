#include "GoldenToadGameToolbarV88.h"
#include "GoldenToadGameAudio.h"
//#include "GoldenToadGameTableUI.h"
#include "Tool/Tools.h"

using namespace ui;

namespace GoldenToad {
    
const static int	TouchLockButtonTag = 885;

const char* Name_AutoFire_Menu_V88	= "Button_Auto";//自动
const char* Name_Lock_Menu_V88		= "Button_Lock";
const char* Name_Change_Menu_V88 = "Button_change";

    GoldenToadToolbarV88::GoldenToadToolbarV88()
        : _isOut(false)
        , _isTouchEnd(true)
        , _onLock(nullptr)
        , _onAutoFire(nullptr)
        , _Toolbar(nullptr)
        , _btnAutoFire(nullptr)
        , _hasButtonPress(true)
        , _canTouchLockButton(true)
        , _canTouch(false)
        , _changeButton(nullptr)
        , _onChangeLockFish(nullptr)
        , _lockButton(nullptr)
    {

    }

GoldenToadToolbarV88::~GoldenToadToolbarV88() {
    stopAllActions();
    removeAllChildrenWithCleanup(true);
}

GoldenToadToolbarV88* GoldenToadToolbarV88::create(Layout* toolBarBG) {
    GoldenToadToolbarV88* toolbar = new GoldenToadToolbarV88();
    if (toolbar->init(toolBarBG)) {
        toolbar->setIgnoreAnchorPointForPosition(false);
        toolbar->autorelease();
        return toolbar;
    }
    CC_SAFE_DELETE(toolbar);
    return nullptr;
}

bool GoldenToadToolbarV88::init(Layout* toolBarBG) {
    if (!HN::HNLayer::init()) return false;
    _toolBarBG = toolBarBG;

    _btnAutoFire = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_AutoFire_Menu_V88));
	_lockButton = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Lock_Menu_V88));
	_changeButton = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Change_Menu_V88));

    _btnAutoFire->setVisible(true);
    _lockButton->setVisible(true);
    _changeButton->setVisible(false);

    _btnAutoFire->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbarV88::menuClickCallback, this));
    _changeButton->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbarV88::menuClickCallback, this));

    _lockButton->addTouchEventListener(CC_CALLBACK_2(GoldenToadToolbarV88::menuClickCallback, this));

    return true;
}

void GoldenToadToolbarV88::menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype) {
    if (!_canTouch) {
        return;
    }

    Button* pMenu = dynamic_cast<Button*>(pSender);
    std::string name = pMenu->getName();
  
    //下述代码逻辑仅在按钮离开的时候执行
    if (Widget::TouchEventType::ENDED != touchtype) {
        return;//长按时屏蔽下面的代码逻辑
    }
    //Button_Menu4
    if (name.compare(Name_AutoFire_Menu_V88) == 0) {
        if (_onAutoFire) {
            _onAutoFire(pMenu);
        }
    }
    //lock menu
    if (name.compare(Name_Lock_Menu_V88) == 0) {
        if (_onLock) {
            this->stopActionByTag(TouchLockButtonTag);
            if (_canTouchLockButton) {
                _onLock(pMenu, true, true);//TODO: 参数对于不同状态锁定鱼的影响  ----  by HBC
                _canTouchLockButton = false;//点击响应之后把锁定状态切换，切换后0.3s内不允许再次切换
            }
            auto seq = Sequence::create(DelayTime::create(0.2f), CallFunc::create([this]() {
                _canTouchLockButton = true;
            }), nullptr);
            seq->setTag(TouchLockButtonTag);
            this->runAction(seq);
        }
    }
    //切换按钮
    if (name.compare(Name_Change_Menu_V88) == 0) {
        if (_onChangeLockFish) {
            _onChangeLockFish(pMenu);
        }
    }
}


void GoldenToadToolbarV88::controlTouchAction(bool canTouch) {
    _canTouch = canTouch;
}


void GoldenToadToolbarV88::controlChangeButtonAnimation(bool showAnimation) {
    _changeButton->setVisible(showAnimation);
    if (showAnimation) {
        //显示的动画
        _changeButton->setVisible(true);
        _changeButton->setPosition(_lockButton->getPosition());
        ccBezierConfig bezierConfig;
        bezierConfig.controlPoint_1 = Vec2(_lockButton->getPositionX() - _lockButton->getContentSize().width * 1.3f, _lockButton->getPositionY());
        bezierConfig.controlPoint_2 = Vec2(_lockButton->getPositionX() - _lockButton->getContentSize().width * 1.3f, _lockButton->getPositionY() + _lockButton->getContentSize().height * 1.3f);
        bezierConfig.endPosition = Vec2(_lockButton->getPositionX(), _lockButton->getPositionY() + _lockButton->getContentSize().height * 1.2f);
        _changeButton->runAction(Spawn::create(BezierTo::create(0.3f, bezierConfig), FadeIn::create(0.2f), nullptr));
    } else {
        //隐藏的动画
        ccBezierConfig bezierConfig;
        bezierConfig.controlPoint_1 = Vec2(_lockButton->getPositionX() - _lockButton->getContentSize().width * 1.3f, _lockButton->getPositionY() + _lockButton->getContentSize().height * 1.3f);
        bezierConfig.controlPoint_2 = Vec2(_lockButton->getPositionX() - _lockButton->getContentSize().width * 1.3f, _lockButton->getPositionY());
        bezierConfig.endPosition = Vec2(_lockButton->getPosition());
        _changeButton->runAction(Spawn::create(BezierTo::create(0.3f, bezierConfig), FadeOut::create(0.2f), nullptr));
        _changeButton->setPosition(_lockButton->getPosition());
        _changeButton->setVisible(false);
    }
}

}
