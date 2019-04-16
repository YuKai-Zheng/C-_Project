#include "GoldenFlowerGameDashboardV88.h"
#include "cocostudio/CocoStudio.h"
#include "GoldenFlowerGameTableUIV88.h"
#include "GoldenFlowerMessageHead.h"

USING_NS_CC;

namespace goldenflower {
DashboardV88* DashboardV88::create(GameTableUIV88* parent) {
	DashboardV88* dashboard = new DashboardV88();
	if (dashboard->init(parent)) {
		dashboard->setIgnoreAnchorPointForPosition(false);
		dashboard->autorelease();
		return dashboard;
	}
	CC_SAFE_DELETE(dashboard);
	return nullptr;
}

bool DashboardV88::init(GameTableUIV88* parent) {
    if(! HN::HNLayer::init()) {
        return false;
    }
    _tableUI = parent;

    Size size = Director::getInstance()->getWinSize();
    _visibleWidth = size.width;
    _visibleHeight = size.height;

    _DashboardV88_node = CSLoader::createNode("Games/12101106/dashboard_V66.csb");
    _DashboardV88_node->setAnchorPoint(Vec2(0.5, 0.5));
    _DashboardV88_node->setPosition(Vec2(_visibleWidth*0.5, _visibleHeight*0.05));
    this->addChild(_DashboardV88_node, 100);

    _lookButton    = dynamic_cast<ui::Button*>(_DashboardV88_node->getChildByName("look_button"));
    _compareButton = dynamic_cast<ui::Button*>(_DashboardV88_node->getChildByName("compare_button"));
    _followButton  = dynamic_cast<ui::Button*>(_DashboardV88_node->getChildByName("follow_button"));
    _giveUpButton  = dynamic_cast<ui::Button*>(_DashboardV88_node->getChildByName("giveup_button"));
    _addButton     = dynamic_cast<ui::Button*>(_DashboardV88_node->getChildByName("add_button"));
    _ButtonBeg	   = dynamic_cast<ui::ImageView*>(_DashboardV88_node->getChildByName("Button_beg"));
    _BetBegPosition = _ButtonBeg->getPosition();
    _betButtons[0] = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_0"));
    _betButtons[1] = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_1"));
    _betButtons[2] = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_2"));
    _betButtons[3] = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_3"));
//		_betButton5 = dynamic_cast<ui::Button*>(_ButtonBeg->getChildByName("Bet_4"));
    for (int i = 0; i < 4; i++) {
        if (_betButtons[i]) {
            _betButtons[i]->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback, this));
            _betButtonLabel[i] = dynamic_cast<Text*>(_betButtons[i]->getChildByName("value"));
        }
    }

    _lookButton->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback,this));
    _compareButton->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback,this));
    _followButton->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback,this));
    _giveUpButton->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback,this));
    _addButton->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback,this));

//		_betButton5->addTouchEventListener(CC_CALLBACK_2(DashboardV88::menClickCallback, this));

    //_slider    = dynamic_cast<ui::Slider*>(_DashboardV88_node->getChildByName("slider"));
    //_sliderBg  = dynamic_cast<ui::ImageView*>(_DashboardV88_node->getChildByName("slider_bg"));
    //_noteLabel = dynamic_cast<ui::TextAtlas*>(_DashboardV88_node->getChildByName("money"));
    //_slider->addEventListener(CC_CALLBACK_2(DashboardV88::sliderChangedCallback,this));

    return true;
}

void DashboardV88::setLookVisible(bool visible) {
    _CanLook = visible;
    setButtonEnabled(_lookButton, visible);
}

void DashboardV88::setFollowVisible(bool visible) {
    _CanFollow = visible;
    setButtonEnabled(_followButton, visible);
}

void DashboardV88::setAddVisible(bool visible) {
    _CanAdd = visible;
    setButtonEnabled(_addButton, visible);
}

void DashboardV88::setAddBetVisible(bool visible) {
    if (visible) {
        auto moveto = MoveTo::create(0.5f, _BetBegPosition + Vec2(0.f, 200.f));
        auto sequence = Sequence::create(EaseBackOut::create(moveto), nullptr);
        _ButtonBeg->runAction(sequence);

        setButtonEnabled(_betButtons[0], _CanAddOfAll[0]);
        setButtonEnabled(_betButtons[1], _CanAddOfAll[1]);
        setButtonEnabled(_betButtons[2], _CanAddOfAll[2]);
        setButtonEnabled(_betButtons[3], _CanAddOfAll[3]);
    } else {
        auto moveto = MoveTo::create(0.1f, _BetBegPosition);
        auto sequence = Sequence::create(EaseBackOut::create(moveto), nullptr);
        _ButtonBeg->runAction(sequence);

        setLookVisible(_CanLook);
        setFollowVisible(_CanFollow);
        setAddVisible(_CanAdd);
        setOpenVisible(_CanOpen);
        setGiveUpVisible(_CanGiveUp);

    }
}

void DashboardV88::setOpenVisible(bool visible) {
    _CanOpen = visible;
    setButtonEnabled(_compareButton, visible);
}

void DashboardV88::setGiveUpVisible(bool visible) {
    _CanGiveUp = visible;
    setButtonEnabled(_giveUpButton, visible);
}

Size DashboardV88::getVisibleSize() {
    Size size = Size::ZERO;
    //if (_visibleWidth > 0 && _visibleWidth <= WIN_SIZE.width && _visibleHeight > 0 && _visibleHeight <= WIN_SIZE.height)
    //{
    //	size.width = _visibleWidth;
    //	size.height = _visibleHeight;
    //}
    //else
    {
        size.width = 960;
        size.height = 80;
    }
    return size;
}

//void DashboardV88::setAddRange(LLONG min, LLONG max)
//{
//	_min = min;
//	_max = max;
//	_addValue = min;
//	setAddValue(min);
//	_slider->setPercent(0);
//}

//LLONG DashboardV88::getAddValue()
//{
//	return _addValue;
//}

DashboardV88::DashboardV88()
    :_lookButton(nullptr)
    ,_compareButton(nullptr)
    ,_followButton(nullptr)
    ,_giveUpButton(nullptr)
    ,_addButton(nullptr)
     //,_slider(nullptr)
     //,_sliderBg(nullptr)
     //,_noteLabel(nullptr)
    ,_tableUI(nullptr)
    , _ButtonBeg(nullptr)
     //,_betButton1(nullptr)
     //,_betButton2(nullptr)
     //,_betButton3(nullptr)
     //, _betButton4(nullptr)
//		, _betButton5(nullptr)
     //,_addValue(0)
     //,_min(0)
     //,_max(0)
    ,_CanLook(false)
    ,_CanFollow(false)
    ,_CanAdd(false)
    ,_CanOpen(false)
    ,_CanGiveUp(false) {
    memset(_CanAddOfAll, 0, sizeof(_CanAddOfAll));
    //memset(_betButtons, 0, sizeof(_betButtons));
    _betButtons.clear();
    _betButtons.resize(4);
    //
    _betButtonLabel.clear();
    _betButtonLabel.resize(4);
}

DashboardV88::~DashboardV88() {

}

//void DashboardV88::setAddValue(LLONG addValue)
//{
//	char tmp[100] = {0};
//	sprintf(tmp, "%lld", addValue);
//	_noteLabel->setString(tmp);
//}

void DashboardV88::setButtonEnabled(ui::Button* pButton, bool enabled) {
    if(pButton != nullptr) {
        GLubyte opacity = enabled ? 255 : 125;
        pButton->setEnabled(enabled);
        pButton->setOpacity(opacity);
    }
}

void DashboardV88::setAllButtonEnabled(bool enabled) {
    setButtonEnabled(_lookButton, enabled);
    setButtonEnabled(_compareButton, enabled);
    setButtonEnabled(_followButton, enabled);
    setButtonEnabled(_giveUpButton, enabled);
    setButtonEnabled(_addButton, enabled);
}

void DashboardV88::menClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchtype) {
    if(touchtype != cocos2d::ui::Widget::TouchEventType::ENDED) {
        return;
    }

    if(pSender == _lookButton) {
        _tableUI->lookCard();
        setAddBetVisible(false);
    } else if(pSender == _compareButton) {
        _tableUI->compareCardReq();
        hide();
        setAddBetVisible(false);
    } else if(pSender == _followButton) {
        _tableUI->followNote();
        _tableUI->IStartTimer(0);
        setAddBetVisible(false);
        setFollowVisible(false);

        setAllButtonEnabled();
        if (_DashboardV88_node) {
            _DashboardV88_node->setVisible(false);
        }
    } else if(pSender == _giveUpButton) {
        _tableUI->giveUpNote();
        _tableUI->IStartTimer(0);
        setAddBetVisible(false);

        setAllButtonEnabled();
        if (_DashboardV88_node) {
            _DashboardV88_node->setVisible(false);
        }
    } else if(pSender == _addButton) {
        _tableUI->addNote(0);
        setAllButtonEnabled();
    } else if (pSender == _betButtons[0]) {
        //选择1k
        _tableUI->addBet(0);
        _tableUI->IStartTimer(0);
        setAddBetVisible(false);
        if (_DashboardV88_node) {
            _DashboardV88_node->setVisible(false);
        }
    } else if (pSender == _betButtons[1]) {
        //选择10k
        _tableUI->addBet(1);
        _tableUI->IStartTimer(0);
        setAddBetVisible(false);
        if (_DashboardV88_node) {
            _DashboardV88_node->setVisible(false);
        }
    } else if (pSender == _betButtons[2]) {
        //选择100k
        _tableUI->addBet(2);
        _tableUI->IStartTimer(0);
        setAddBetVisible(false);
        if (_DashboardV88_node) {
            _DashboardV88_node->setVisible(false);
        }
    } else if (pSender == _betButtons[3]) {
        //选择1000k
        _tableUI->addBet(3);
        _tableUI->IStartTimer(0);
        setAddBetVisible(false);
        if (_DashboardV88_node) {
            _DashboardV88_node->setVisible(false);
        }
    }
}

//void DashboardV88::sliderChangedCallback(cocos2d::Ref* pSender,cocos2d::ui::Slider::EventType type)
//{
//	if(type != ui::Slider::EventType::ON_PERCENTAGE_CHANGED || pSender != _slider)
//	{
//		return;
//	}

//	cocos2d::ui::Slider* pSlider = (cocos2d::ui::Slider*)(pSender);
//	int percent = pSlider->getPercent();
//	_addValue = _min + (_max - _min) * (percent / 100.0f);
//	setAddValue(_addValue);
//}

void DashboardV88::show() {
    this->setVisible(true);
    if (_DashboardV88_node) {
        _DashboardV88_node->setVisible(true);
    }
}

void DashboardV88::hide() {
    this->setVisible(false);
}

void DashboardV88::setBetButtonValue(int values[4]) {
    ostringstream ss;

    for (int i = 0; i < 4; i++) {
        int val = values[i];
        //因为筹码金额相对固定，所以此处特殊处理
        if (val < 1000) {
            ss << val;
        } else if (val >= 1000 && val < 10000) {
            ss << (val / 1000) << "千";
        } else if (val >= 10000 && val < 100000000) {
            ss << (val / 10000) << "万";
        } else if (val >= 100000000) {
            ss << (val / 100000000) << "亿";
        } else {
            ss << val;
        }

		_betButtonLabel[i]->setString(ss.str().c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		_betButtonLabel[i]->setString(ForceGBKToUtf8(ss.str().c_str()));
#else
		_betButtonLabel[i]->setString(GBKToUtf8(ss.str().c_str()));
#endif

		ss.str("");
    }
}

}

