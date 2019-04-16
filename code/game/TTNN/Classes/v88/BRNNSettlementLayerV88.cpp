#include "BRNNSettlementLayerV88.h"
#include "HNLobby/PlatformDefine.h"
#include "BRNNResV88.h"
#include "BRNNActionsV88.h"
#include "VersionInfo/VersionInfo.h"

namespace BRNN {

void BRNNSettlementLayerV88::closeSet() {
    if (this->getChildByTag(20)) {
        this->removeChildByTag(20);
    }
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    this->runAction(Sequence::create(RemoveSelf::create(), nullptr));
}

void BRNNSettlementLayerV88::onEnter() {
    HNLayer::onEnter();
    this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([&]() {
        closeSet();
    }), nullptr));
}

bool BRNNSettlementLayerV88::init() {
    if (!HNLayer::init()) {
        return false;
    }
    this->setContentSize(WIN_SIZE);
    this->setIgnoreAnchorPointForPosition(false);

    LayerColor* color = LayerColor::create();
    color->setColor(Color3B::BLACK);
    color->setOpacity(200);
//    color->setPosition(WIN_SIZE/2);
    addChild(color);
    //
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(BRNNSettlementLayerV88::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(BRNNSettlementLayerV88::onTouchEnded, this);
    //listener->onTouchMoved = CC_CALLBACK_2(BRNNHelpLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //
	loader = CSLoader::createNode(BRNN_SETTLEMENT_CSB_V88);
    loader->setPosition(WIN_SIZE/2);
    addChild(loader, 13);
    //
    return true;
}

bool BRNNSettlementLayerV88::onTouchBegan(Touch *touch, Event *unused_event) {
    Point point = touch->getLocation();
    if (!this->getBoundingBox().containsPoint(point)) {
        HNAudioEngine::getInstance()->playEffect("platform/sound/sound_close.mp3");
        //Actions::actionOut(this);
    }
    return true;
}

void BRNNSettlementLayerV88::onTouchEnded(Touch *touch, Event *unused_event) {
}

void BRNNSettlementLayerV88::setBankerMoney(const char* money) {
    dynamic_cast<Text*> (loader->getChildByTag(12))->setString(money);
}

void BRNNSettlementLayerV88::setMyselfMoney(const char* money) {
    dynamic_cast<Text*> (loader->getChildByTag(11))->setString(money);
    isWin= Value(money).asInt() >= 0;
    string filename("");
    string ani_name("");
    Vec2 pos = WIN_SIZE/2;
    if (isWin) {
        filename = "v88/animation/settle/shengli/shengli.csb";
        ani_name = "shengli";
        auto antAction = CSLoader::createTimeline(filename);
		antAction->play(ani_name, false);
        auto winCSB = CSLoader::createNode(filename);
        winCSB->runAction(antAction);
        winCSB->setTag(20);
//        winCSB->setPositionY(-70);
        winCSB->setPosition(pos);
        addChild(winCSB, 10);

		auto pGameWin = Sprite::create(BRNN_SETTLEMENT_WIN_BG_V88);
//        pGameWin->setPosition(Vec2(this->getContentSize().width / 2 - 384, this->getContentSize().height / 2 - 325));
        pGameWin->setPosition(pos + Vec2(0, -20));
		//m_pGameWin->setVisible(false);
		addChild(pGameWin, 9);

    } else {
        filename = "v88/animation/settle/shibai/shibai.csb";
        ani_name = "shibai";
        auto antAction = CSLoader::createTimeline(filename);
		antAction->play(ani_name, false);
        auto loseCSB = CSLoader::createNode(filename);
        loseCSB->runAction(antAction);
        loseCSB->setTag(20);
//        loseCSB->setPositionY(-70);
        loseCSB->setPosition(pos);
        addChild(loseCSB, 10);

		auto pGameLose = Sprite::create(BRNN_SETTLEMENT_LOSE_BG_V88);
//        pGameLose->setPosition(Vec2(this->getContentSize().width / 2 - 384, this->getContentSize().height / 2 - 325));
        pGameLose->setPosition(pos + -Vec2(0, -20));
		//m_pGameLose->setVisible(false);
		addChild(pGameLose, 9);
    }
}

}
