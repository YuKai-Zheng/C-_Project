#include"BankGivePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "SimpleAudioEngine.h"
#include "PlatformDefine.h"
#include "HNLobby/globel.h"
#include "VersionInfo/VersionInfo.h"
#include "Tool/Tools.h"
#include "Sound/HNAudioEngine.h"
#include "v66/PlatformResV66.h"

//static const char* EXIT_GO_ON_D = "platform/exitUi/bird2.png";                      //goonBtn

bool BankGivePrompt::init() {
    if (!HNLayer::init()) return false;

    /*
    if (VERSION_EIGHTY_EIGHT) {
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
        addChild(colorLayer);

        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            this->closePrompt();
            return true;
        };
        touchListener->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

        auto winSize = Director::getInstance()->getWinSize();
		_loader = CSLoader::createNode("csb/v66/bankLayer/givePrompt.csb");
		_loader->setPosition(WIN_SIZE / 2);
		_loader->setScale(0);
		addChild(_loader, 2);
		_loader->runAction(ScaleTo::create(0.2f, 1.0f));
		
		auto btn = (Button*)_loader->getChildByName("Button_ok");
		btn->setVisible(false);
		_btnContinue = AUIButton::create(BTN_YELLOW1_NORMAL_BG_V66, BTN_YELLOW1_SELECT_BG_V66, BTN_YELLOW1_ENABLED_BG_V66);
		_btnContinue->setTitle("v66/button/yellow1/11.png");
		_btnContinue->setPosition(btn->getPosition());
		_loader->addChild(_btnContinue);
		_btnContinue->setScale(btn->getScale());
		_btnContinue->setName("Button_ok1");
		_btnContinue->addTouchEventListener(CC_CALLBACK_2(BankGivePrompt::ButtonCallBack, this));
		
// 		auto curcularExit = (Button*)_loader->getChildByName("exitBtn");
// 		curcularExit->addTouchEventListener(CC_CALLBACK_2(BankGivePrompt::ButtonCallBack, this));


        _targetId = (Text*)_loader->getChildByName("Text_id");
        _targetMoney = (Text*)_loader->getChildByName("Text_money");
        _targetMoney->setVisible(false);
        _time = (Text*)_loader->getChildByName("Text_giveTime");
        _id = (Text*)_loader->getChildByName("Text_nickName");
        _money = (Text*)_loader->getChildByName("Text_moneyNum");
    } else
        */
    {
        //auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
        //addChild(colorLayer);

        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            this->closePrompt();
            return true;
        };
        touchListener->setSwallowTouches(true);
        

        auto winSize = Director::getInstance()->getWinSize();
        
        _loader = CSLoader::createNode("csb/newplatform/bank/givePrompt.csb");
        _loader->setPosition(winSize / 2);
        _loader->setScale(0);
        addChild(_loader, 2);
        _loader->runAction(ScaleTo::create(0.2f, 1.0f));
        _layout = (Layout*)_loader->getChildByName("Panel_1");
		auto blacknode = _loader->getChildByName("Panel_9");
		blacknode->setScaleX(WIN_SIZE.width / 1280.0);
		blacknode->setScaleY(WIN_SIZE.height / 720.0);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _loader);
		//auto btnTitle = (Sprite*)_layout->getChildByName("redOk_6");
		//btnTitle->setVisible(false);
		//auto btn = (Button*)_layout->getChildByName("button_ok");
		//btn->setVisible(false);
		_btnContinue = (Button*)_layout->getChildByName("button_ok"); //AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
		//_btnContinue->setTitle("button/redOk.png");
		//_btnContinue->setTitleOffset(Vec2(0, 4));
		//_btnContinue->setPosition(btn->getPosition());
		//_layout->addChild(_btnContinue);
		_btnContinue->setName("Button_ok1");
		_btnContinue->addTouchEventListener(CC_CALLBACK_2(BankGivePrompt::ButtonCallBack, this));

		auto curcularExit = (Button*)_layout->getChildByName("exitBtn");
		curcularExit->addTouchEventListener(CC_CALLBACK_2(BankGivePrompt::ButtonCallBack, this));


        _targetId = (Text*)_layout->getChildByName("target_id");
        _targetMoney = (Text*)_layout->getChildByName("target_money");
		if (_targetMoney) _targetMoney->setVisible(false);
        _time = (Text*)_layout->getChildByName("give_time");
        _id = (Text*)_layout->getChildByName("nickName");
        _money = (Text*)_layout->getChildByName("money");
    }

    return true;
}

void BankGivePrompt::updateInfo(std::string nickName, LLONG targetMoney, std::string  time,INT id) {
    _targetId->setString(to_string(id));
    _id->setString(ForceGBKToUtf8(nickName.c_str()));
    _money->setString(ForceGBKToUtf8(Tools::numberToChinese(targetMoney).c_str()));
    _targetMoney->setString(ForceGBKToUtf8(to_string(targetMoney).c_str()));
    _time->setString(time);
}

void BankGivePrompt::closePrompt() {
    _loader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]() {
        this->removeFromParent();
    }), nullptr));
}

void BankGivePrompt::ButtonCallBack(Ref* pSender, Widget::TouchEventType  type) {
    auto btn = (Button*)pSender;
    auto name = btn->getName();
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
		btn->setScale(0.8f);
        break;
	//case cocos2d::ui::Widget::TouchEventType::MOVED:
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		btn->setScale(1.0f);
		break;

    case cocos2d::ui::Widget::TouchEventType::ENDED:
		btn->setScale(1.0f);
        if (name.compare("Button_ok1") == 0) {
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            this->closePrompt();
        } else if (name.compare("exitBtn") == 0) {
            HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
            this->closePrompt();
        }
        break;
    default:
        break;
    }
}

void BankGivePrompt::callBackNo() {
    _loader->runAction(Sequence::create(ScaleTo::create(0.2f, 0),
    CCCallFunc::create([&]() {
        this->removeFromParent();
    }), nullptr));
}
