#include "GoldenFlowerGameDealerUIV88.h"
//#include "GoldenFlowerGameChipUI.h"
#include "GoldenFlowerPokerCardV88.h"
#include "GoldenFlowerGameTableUIV88.h"
#include "GoldenFlowerDrawEllipseV88.h"
#include "GoldenFlowerResourceMgr.h"

#include "Sound/HNAudioEngine.h"

USING_NS_CC;
using namespace HN;

namespace goldenflower {
static const char* CHIP_FILENAME = "v66/chouma/%d.png";		//0-4		100 1000 1,0000 10,0000 100,0000

enum {
    eZCommunityCard = 10,//公共牌
};

enum {
    eDealerNode,
    eDealerStart,
    eDealerFlop = 10,
    eDealerTurn,
    eDealerRiver,
    eDealerEnd,
};

GameDelearUIV88* GameDelearUIV88::create() {
    GameDelearUIV88* dealer = new GameDelearUIV88();
    if (dealer->init()) {
        dealer->autorelease();
        return dealer;
    } else {
        CC_SAFE_DELETE(dealer);
        return nullptr;
    }
}

void GameDelearUIV88::betMoney(BYTE seatNo, int moneyType) {
    //if (!seatNoOk(seatNo))
    //{
    //	return;
    //}

    Sprite* chip = Sprite::create(StringUtils::format(CHIP_FILENAME, moneyType + 7));
    chip->setScale(0.5f);
    if (seatNoOk(seatNo)) {
        chip->setTag(seatNo);
        chip->setPosition(getBetFlyToPoint(seatNo));
    } else {
        //chip->setTag(0);
        chip->setPosition(WIN_SIZE / 2);
    }

    _chipsPool->addChild(chip);

    Vec2 chipPoint;
    bool flag;
    do {
        chipPoint = _ellipse->getEllipseOrigin();
        auto xOffset = fmod(rand()*(rand() % 2 ? -1 : 1), _ellipse->getEllipseX());
        auto yOffset = fmod(rand()*(rand() % 2 ? -1 : 1), _ellipse->getEllipseY());

        chipPoint.x += xOffset;
        chipPoint.y += yOffset;

        flag = _ellipse->isInEllipse(chipPoint);
    } while (!flag);

    chip->runAction(MoveTo::create(0.1f, chipPoint));
    HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);
}

//给玩家发手牌
void GameDelearUIV88::dealHandCard(const std::vector<THandCard>& handCard) {
    _handCard = handCard;

    // counter for the cards dealed
    _curCount = 0;

    //schedule(schedule_selector(GameDelearUIV88::dispatchHandCard), 0.2f, 3 * handCard.size() -1, 0);
    dispatchHandCard();
}

//void GameDelearUIV88::dispatchHandCard(float delta)
void GameDelearUIV88::dispatchHandCard() {
    ////////////////////////////////////////////////////////////////

    // shuffle cards

    // radius
    float radius = 250;

    // Total angle
    float alpha = 40;
    float y_adjustment = 230;

    float beta = (180 - alpha) / 2;

    // angle value TO radians value
    auto r_alpha = alpha * M_PI / 180;									//所有牌的弧度
    auto r_beta = beta * M_PI / 180;									//第一张牌与水平轴的弧度
    auto r_alphaSplit = r_alpha / (TOTAL_CARDS - 1);                    //每一张牌的弧度

    int delaycounter = 0;
    auto dealerPosition = _dealerPoint;

    for (int i = 0; i < TOTAL_CARDS; i++, delaycounter++) {
        auto x_offset = radius*cos(r_alphaSplit*i + r_beta);

        auto y_offset = (-1)*radius*sin(r_alphaSplit*i + r_beta) + y_adjustment;
        auto rotateAngle = (r_alphaSplit*i + r_beta) * 180 / M_PI - 90;

        _cardSuite[i]->setPosition(Vec2(dealerPosition.x + x_offset, dealerPosition.y + y_offset));
        _cardSuite[i]->runAction(Sequence::create(
                                     RotateBy::create(0.01f, rotateAngle),
                                     DelayTime::create(0.01f*delaycounter),
        CallFunc::create([=]() {
            _cardSuite[i]->setVisible(true);
            //_cardSuite[i]->setScale(0.86);
        })
        , nullptr));
    }

    ////////////////////////////////////////////////////////////////
    // dear cards
    //发牌之前重新调整牌的位置 防止中途改变
    GameTableUIV88* table = (GameTableUIV88*)this->getParent();
    table->getPlayer(0)->setHandCardPosition();

    schedule(schedule_selector(GameDelearUIV88::showUserHandCard), 0.3f, 3 * _handCard.size() - 1, 0);

}

void GameDelearUIV88::showUserHandCard(float delta) {
    int playercount = _handCard.size();

    _curCount++;
    int cardIndex = (_curCount - 1) / playercount;
    int playerIndex = (_curCount - 1) % playercount;

    int theTopIndexOfCardSuite = TOTAL_CARDS - _curCount;

    GameTableUIV88* table = (GameTableUIV88*)this->getParent();
    auto player = table->getPlayer(_handCard[playerIndex].bySeatNo);
    //auto playerposition = player->getPosition();

    auto move = MoveTo::create(0.5f, getSeatPoint(_handCard[playerIndex].bySeatNo));
    auto ease = EaseSineOut::create(move);
    auto rotate = RotateTo::create(0.5f, 0);
    auto spwn = Spawn::create(ease, rotate, nullptr);

    _cardSuite[theTopIndexOfCardSuite]->runAction(Sequence::create(
                spwn,
    CallFunc::create([=]() {
        HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
        _cardSuite[theTopIndexOfCardSuite]->setVisible(false);
        //显示手牌
        player->setHandCard(cardIndex, 0x00);
    }),
    nullptr));

    if (_curCount >= _handCard.size() * 3) {
        //回收发的牌
        schedule(schedule_selector(GameDelearUIV88::recycleCardSuite), 0.5f, 0, 0.6f);
        auto CurrentPlayer = table->getPlayer(0);

        //改变当前玩家牌的大小及位置
        for (int i = 0; i < MAX_CARD_COUNT; i++) {
            auto moveby = MoveBy::create(0.5f,Vec2((i-1)*10,0));
            auto scaleto = ScaleTo::create(0.5f, 1.2);
            auto spawn = Spawn::create(moveby, scaleto,nullptr);
            CurrentPlayer->_handCard[i]->runAction(Sequence::create(
                    DelayTime::create(1.0),
                    spawn,
                    nullptr));
        }
    }
}

// recycle card suite
void GameDelearUIV88::recycleCardSuite(float delta) {
    auto delaycounter = 0;
    for (int i = TOTAL_CARDS - 1 - _curCount; i >= 0; i--) {
        if (_cardSuite[i]->isVisible() == true) {
            _cardSuite[i]->runAction(Sequence::create(
                                         DelayTime::create(0.005f*delaycounter++),
            CallFunc::create([=]() {
                _cardSuite[i]->setVisible(false);
                _cardSuite[i]->setRotation(0);
            }),
            nullptr));
        } else {
            _cardSuite[i]->setRotation(0);
        }

    }
}

void GameDelearUIV88::recycleChips(BYTE winnerSeatNo) {
    auto winnerpos = getBetFlyToPoint(winnerSeatNo);
    for (auto chip : _chipsPool->getChildren()) {
        auto scaleBig = ScaleTo::create(0.8f, 1.5f);
        auto scaleSmall = ScaleTo::create(0.4f, 0);
        auto move = MoveTo::create(0.8f, winnerpos);
        auto spaw = Spawn::create(scaleBig, move, nullptr);
        chip->runAction(Sequence::create(
                            spaw,
                            scaleSmall,
                            DelayTime::create(0.4f),
        CallFunc::create([=]() {
            chip->removeFromParent();
        }),
        nullptr));
        auto orbit = OrbitCamera::create(0.4f, 1, 0, 0, 360, 0, 0);
        chip->runAction(Repeat::create(orbit,2));

    }
    return;
}

//发翻牌
void GameDelearUIV88::dealFlopCard(const std::vector<BYTE>& flopCard) {
    if (flopCard.size() != 3) {
        return;
    }

    for (unsigned int i = 0; i < flopCard.size(); i++) {
        HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
		PokerCardV88* pCard = (PokerCardV88*)_communityCard[i];
        pCard->setCardValue(flopCard[i]);
        //pCard->setCardFrameName(pCard->getCardFileName());
        pCard->setPosition(_communityCardPoints[0]);
        pCard->setVisible(false);
    }

    dispatchCommunityCard(eDealerFlop);
}

void GameDelearUIV88::moveFlopCard() {
    for (unsigned int i = 1; i < 3; i++) {
        _communityCard[i]->runAction(MoveTo::create(0.1 * i, _communityCardPoints[i]));
    }
}

//发转牌
void GameDelearUIV88::dealTurnCard(BYTE turnCard) {
    HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
	PokerCardV88* card = (PokerCardV88*)_communityCard[3];
    card->setCardValue(turnCard);
    //card->setCardFrameName(card->getCardFileName());
    card->setPosition(_communityCardPoints[3]);
    card->setVisible(false);

    dispatchCommunityCard(eDealerTurn);
}

//发河牌
void GameDelearUIV88::dealRiverCard(BYTE riverCard) {
    HNAudioEngine::getInstance()->playEffect(GAME_FAPAI);
	PokerCardV88* card = (PokerCardV88*)_communityCard[4];
    card->setCardValue(riverCard);
    //card->setCardFrameName(card->getCardFileName());
    card->setPosition(_communityCardPoints[4]);
    card->setVisible(false);

    dispatchCommunityCard(eDealerRiver);
}

void GameDelearUIV88::dispatchCommunityCard(int type) {
    Action* action = nullptr;
    if (type == eDealerFlop) {
        action = getFlopAction();
    } else if (type == eDealerTurn) {
        action = getTurnAction();
    } else if (type == eDealerRiver) {
        action = getRiverAction();
    } else {

    }

    if (action == nullptr) {
        return;
    }

	PokerCardV88* tmpCard = PokerCardV88::create(0x00);
    tmpCard->setPosition(_dealerPoint);
    tmpCard->setScale(0.8f);
    tmpCard->setCardValue(0);
    //tmpCard->setCardFrameName(tmpCard->getCardFileName());
    tmpCard->setTag(type);
    this->addChild(tmpCard, eZCommunityCard);
    tmpCard->runAction(action);
}

void GameDelearUIV88::openCommunityCard(cocos2d::Node* pNode) {
    int tag = pNode->getTag();
    if (tag == eDealerFlop) {
        _communityCard[0]->setVisible(true);
        _communityCard[1]->setVisible(true);
        _communityCard[2]->setVisible(true);
    } else if (tag == eDealerTurn) {
        _communityCard[3]->setVisible(true);
    } else if (tag == eDealerRiver) {
        _communityCard[4]->setVisible(true);
    }
}

cocos2d::Action* GameDelearUIV88::getFlopAction() {
    Action* action = nullptr;
    auto moveTo = MoveTo::create(0.5f, _communityCardPoints[0]);
    auto scale = ScaleTo::create(0.5f, 1.5f);
    auto openCard = CallFuncN::create(CC_CALLBACK_1(GameDelearUIV88::openCommunityCard, this));
    auto moveFlop = CallFunc::create(CC_CALLBACK_0(GameDelearUIV88::moveFlopCard, this));
    action = Sequence::create(Spawn::create(moveTo, scale, nullptr),
                              RemoveSelf::create(true),
                              openCard,
                              moveFlop,
                              nullptr);
    return action;
}

cocos2d::Action* GameDelearUIV88::getTurnAction() {
    Action* action = nullptr;
    auto moveTo = MoveTo::create(0.5f, _communityCardPoints[3]);
    auto scale = ScaleTo::create(0.5f, 1.5f);
    auto openCard = CallFuncN::create(CC_CALLBACK_1(GameDelearUIV88::openCommunityCard, this));
    action = Sequence::create(Spawn::create(moveTo, scale, nullptr),
                              RemoveSelf::create(true),
                              openCard,
                              nullptr);
    return action;
}

cocos2d::Action* GameDelearUIV88::getRiverAction() {
    Action* action = nullptr;
    auto moveTo = MoveTo::create(0.5f, _communityCardPoints[4]);
    auto scale = ScaleTo::create(0.5f, 1.5f);
    auto openCard = CallFuncN::create(CC_CALLBACK_1(GameDelearUIV88::openCommunityCard, this));
    action = Sequence::create(Spawn::create(moveTo, scale, nullptr),
                              RemoveSelf::create(true),
                              openCard,
                              nullptr);
    return action;
}

//收牌
void GameDelearUIV88::dealCollectCard() {

}

//给玩家派奖（先收筹码，再派奖）
void GameDelearUIV88::dealWinPool(const std::vector<std::vector<LLONG>> &winPool) {
    _winPool = winPool;
    HNAudioEngine::getInstance()->playEffect(GAME_SHOUCHOUMA);

    this->runAction(Sequence::create(
                        CallFunc::create(CC_CALLBACK_0(GameDelearUIV88::collectChip, this)),
                        DelayTime::create(0.6f),
                        CallFunc::create(CC_CALLBACK_0(GameDelearUIV88::sendPrize, this)),
                        nullptr));
}

bool GameDelearUIV88::init() {
    if (!HNLayer::init()) {
        return false;
    }
    _tableChip.resize(PLAY_COUNT);

    //添加5张公共牌，以后一直缓存使用
    _communityCardPoints.resize(5);
    _communityCard.resize(5);
    for (int i = 0; i < 5; i++) {
		PokerCardV88* card = PokerCardV88::create(0x00);
        this->addChild(card, eZCommunityCard + i);
        card->setVisible(false);
        card->setScale(0.8f);
        _communityCard[i] = card;
    }


    //庄家
    _dealer = Sprite::create("Games/12101106/zhuang.png");
    this->addChild(_dealer, 50);
    _dealer->setVisible(false);
    _dealer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _dealer->setPosition(Vec2::ZERO);

    // init card suite with back image, total 52 cards in a suite
    for (int i = 0; i < TOTAL_CARDS; i++) {
		_cardSuite[i] = PokerCardV88::create(0x00);
        this->addChild(_cardSuite[i], i);
        //_cardSuite[i]->setScale(1.2);
        _cardSuite[i]->setVisible(false);
    }

    // init the ellipse area chip pool
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto e_origin = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    _ellipse = new DrawEllipseV88(e_origin, 150, 50);

    // init the chip pool
    _chipsPool = Layer::create();
    _chipsPool->setPosition(origin);
    this->addChild(_chipsPool);

    return true;
}

GameDelearUIV88::GameDelearUIV88() :_buttonStation(INVALID_DESKNO) {


}


GameDelearUIV88::~GameDelearUIV88() {

}

Vec2 GameDelearUIV88::getButtonPoint() {
    if (_buttonStation != INVALID_DESKNO) {
        return _tablePoints[_buttonStation] + Vec2(-54 / 2.0f, 0);
    } else {
        return Vec2::ZERO;
    }
}

cocos2d::Vec2 GameDelearUIV88::getSeatPoint(BYTE seatNo) {
    return (seatNoOk(seatNo)) ? _seatPoints[seatNo] : Vec2::ZERO;
}

cocos2d::Vec2 GameDelearUIV88::getBetFlyToPoint(BYTE seatNo) {
    return (seatNoOk(seatNo)) ? _BetFlyToPoints[seatNo] : Vec2::ZERO;
}

cocos2d::Vec2 GameDelearUIV88::getTablePoint(BYTE seatNo) {
    return (seatNoOk(seatNo)) ? _tablePoints[seatNo] : Vec2::ZERO;
}

void GameDelearUIV88::collectChip() {
    //for (int i = 0; i < PLAY_COUNT; i++)
    //{
    //	if (_tableChip[i] != nullptr)
    //	{
    //		_tableChip[i]->runAction(Sequence::create(
    //			EaseOut::create(MoveTo::create(0.4f, _dealerPoint), 2.5f),
    //			RemoveSelf::create(true),
    //			nullptr));
    //		_tableChip[i] = nullptr;
    //	}
    //}
}

void GameDelearUIV88::sendPrize() {
    //for(int poolIndex = 0; poolIndex < PLAY_COUNT; poolIndex++)
    //{
    //	for(int seatNo = 0; seatNo < PLAY_COUNT; seatNo++)
    //	{
    //		//按照奖池来分配
    //		if(_winPool[seatNo][poolIndex] > 0)
    //		{
    //			auto chip = GameChipUI::create(CHIP_FILENAME);
    //			chip->setValue(_winPool[seatNo][poolIndex]);
    //			chip->setAnchorPoint(Vec2(0.5f, 0.5f));
    //			chip->setPosition(_dealerPoint);
    //			chip->setScale(0.5f);
    //			this->addChild(chip);
    //			chip->runAction(Sequence::create(
    //				EaseIn::create(MoveTo::create(0.6f, getSeatPoint(seatNo)), 2.5f),
    //				RemoveSelf::create(true),
    //				nullptr));
    //		}
    //	}
    //}
}


void GameDelearUIV88::setDealerPoint(cocos2d::Vec2 point) {
    _dealerPoint = point;
    if (_dealer != nullptr) {
        _dealer->setPosition(point);
    }
}

void GameDelearUIV88::setTablePoint(std::vector<cocos2d::Vec2>& points) {
    _tablePoints = points;
}

void GameDelearUIV88::setSeatPoint(std::vector<cocos2d::Vec2>& points) {
    _seatPoints = points;
}

void GameDelearUIV88::setBetFlyToPoint(std::vector<cocos2d::Vec2>& points) {
    _BetFlyToPoints = points;
}

void GameDelearUIV88::setCommunityPoint(const std::vector<cocos2d::Vec2>& points) {
    if (points.size() != 5) {
        return;
    }
    _communityCardPoints = points;
    for (unsigned int i = 0; i < points.size(); i++) {
        _communityCard[i]->setPosition(points[i]);
    }
}

void GameDelearUIV88::setButtonStation(BYTE buttonStation) {
    _buttonStation = buttonStation;
}

bool GameDelearUIV88::seatNoOk(BYTE seatNo) {
    return (seatNo >= 0 && seatNo < PLAY_COUNT);
}

void GameDelearUIV88::clear() {
    for (int i = 0; i < 5; i++) {
        _communityCard[i]->setVisible(false);
    }
    _dealer->setVisible(false);
}


}
