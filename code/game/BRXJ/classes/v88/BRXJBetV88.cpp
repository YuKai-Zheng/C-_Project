
#include "BRXJBetV88.h"
#include "HNLobby/globel.h"
#include "cocostudio/CocoStudio.h"
#include "Sound/HNAudioEngine.h"

namespace BRXJ {

#define MAX_BET_COUNT_V99		6;

static char* ROOT = "Games/11100600/bet_btn_node_v88.csb";

BetV88::BetV88() {
    qian = 1000;
    wan = 10000;
    shiwan = 100000;
    baiwan = 1000000;
    qianwan = 5000000;

    curValue = 0;
    _curSeleteBtnTag = -1;
}

BetV88::~BetV88() {

}

bool BetV88::init() {
    if (!HNNode::create())
        return false;

    auto root = CSLoader::createNode(ROOT);
    this->addChild(root);

    _bet.Panel = static_cast<Layout*>(root->getChildByName("Panel"));

    initBet();

    return true;
}

void BetV88::initBet() {
    _bet.one = static_cast<Button*>(_bet.Panel->getChildByName("btn0"));
    _bet.two = static_cast<Button*>(_bet.Panel->getChildByName("btn1"));
    _bet.three = static_cast<Button*>(_bet.Panel->getChildByName("btn2"));
    _bet.four = static_cast<Button*>(_bet.Panel->getChildByName("btn3"));
    _bet.five = static_cast<Button*>(_bet.Panel->getChildByName("btn4"));

    for (int i = 0; i < 5; i++) {
        auto effect = static_cast<Sprite*>(_bet.Panel->getChildByName(to_string(i)));
        _vecEffects.push_back(effect);
    }
    _bet.oneEffect = static_cast<Sprite*>(_bet.Panel->getChildByName("0"));
    _bet.twoEffect = static_cast<Sprite*>(_bet.Panel->getChildByName("1"));
    _bet.threeEffect = static_cast<Sprite*>(_bet.Panel->getChildByName("2"));
    _bet.fourEffect = static_cast<Sprite*>(_bet.Panel->getChildByName("3"));
    _bet.fiveEffect = static_cast<Sprite*>(_bet.Panel->getChildByName("4"));

    _bet.one->setTag(0);
    _bet.two->setTag(1);
    _bet.three->setTag(2);
    _bet.four->setTag(3);
    _bet.five->setTag(4);

	_bet.one->addTouchEventListener(CC_CALLBACK_2(BetV88::btnCallBack, this));
	_bet.two->addTouchEventListener(CC_CALLBACK_2(BetV88::btnCallBack, this));
	_bet.three->addTouchEventListener(CC_CALLBACK_2(BetV88::btnCallBack, this));
	_bet.four->addTouchEventListener(CC_CALLBACK_2(BetV88::btnCallBack, this));
	_bet.five->addTouchEventListener(CC_CALLBACK_2(BetV88::btnCallBack, this));
}
void BetV88::grayEffectBet(bool flag) {
	_bet.one->setBright(flag);
    _bet.one->setTouchEnabled(flag);

    _bet.two->setBright(flag);
    _bet.two->setTouchEnabled(flag);

    _bet.three->setBright(flag);
    _bet.three->setTouchEnabled(flag);

	_bet.four->setBright(flag);
    _bet.four->setTouchEnabled(flag);

    _bet.five->setBright(flag);
    _bet.five->setTouchEnabled(flag);
	
    if (flag != true) {
        for (auto effect : _vecEffects) {
            effect->setVisible(false);
        }
    }
}


void BetV88::btnCallBack(Ref* pShender, Widget::TouchEventType  type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    HNAudioEngine::getInstance()->playEffect("music/btnEffect/button_click_sound.mp3");
    auto curSelectBet = static_cast<Button*>(pShender);
    _curSeleteBtnTag = curSelectBet->getTag();
    auto name = curSelectBet->getName();
    _vecEffects.at(_curSeleteBtnTag)->setVisible(true);
    for (auto effect : _vecEffects) {
        if (effect->getName() != to_string(_curSeleteBtnTag))
            effect->setVisible(false);
    }
    if (name == "btn0") {
        curValue = qian;
    } else if (name == "btn1") {
        curValue = wan;
    } else if (name == "btn2") {
        curValue = shiwan;
    } else if (name == "btn3") {
        curValue = baiwan;
    } else if (name == "btn4") {
        curValue = qianwan;
    }
}
}
