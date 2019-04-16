#include "ThirteenZhangCardBoardv88.h"
#include "ThirteenZhangCardSpritev88.h"
#include <algorithm>
#include "ThirteenZhangSoundHelper.h"

using namespace HN;

/*
 * 玩家出牌操作面板
 * Tag用来记录牌是否被点击
 * Name用来记录牌是否被提起
 */

#define UP_OFFSET				40.0f
#define SELF_CARD_MIN_INTERVAL  35.0f		// 两张牌间最小间隔
#define SELF_CARD_MAX_INTERVAL  45.0f		// 两张牌间最大间隔
#define OTHER_CARD_INTERVAL     15.0f

#define UP        "up"
#define DOWN      "down"

#define TAG_UNSELECTED  0
#define TAG_SELECTED    1
#define TAG_MOVEING		2

#define ZORDER_CARD_BEGAN		1

#define SEND_CARD_TIME			0.15f

#define SEND_CARD_SCHEDULE		"send_card_schedule"

namespace ThirteenZhang {
CardBoardV88* CardBoardV88::create(bool self) {
    CardBoardV88* board = new CardBoardV88();
    if (board->init(self)) {
        board->autorelease();
        return board;
    }
    CC_SAFE_DELETE(board);
    return nullptr;
}

CardBoardV88::CardBoardV88()
    : _enableCardTouch(true)
    , _multiSelect(false)
    , _isDrag(false)
    , _startPos(Vec2::ZERO)
    , _endPos(Vec2::ZERO) {

}

CardBoardV88::~CardBoardV88() {

}

bool CardBoardV88::init(bool self) {
    if(!HNLayer::init()) {
        return false;
    }

    this->setIgnoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->setContentSize(Size::ZERO);

    _self = self;

    if(_self) {
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(CardBoardV88::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(CardBoardV88::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(CardBoardV88::onTouchEnded, this);
        listener->setSwallowTouches(true);
        this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    return true;
}

void CardBoardV88::sendCardOneByOne(const std::vector<BYTE>& cards, float interval) {
    if (cards.empty()) return;

    unschedule(SEND_CARD_SCHEDULE);
    {
        std::queue<BYTE>().swap(_tmpCardValues);
    }

    removeCard(cards);

    for (auto card : cards) _tmpCardValues.push(card);

    if (interval < SEND_CARD_TIME) interval = SEND_CARD_TIME;

    schedule(CC_CALLBACK_1(CardBoardV88::scheduleSendCardStart, this), interval, SEND_CARD_SCHEDULE);
}

void CardBoardV88::sendCard(const std::vector<BYTE>& cards) {
    if (cards.empty()) return;

    for (BYTE card : cards)
        addCard(card);

    sortCard();
    resizeCardBoard();
}

std::vector<BYTE> CardBoardV88::getUpCards() {
    std::vector<BYTE> cards = searchCards(std::string(UP));
    return cards;
}

std::vector<Vec2> CardBoardV88::getUpCardsPosition() {
    std::vector<Vec2> vcPos;
    for (auto poker : _pokers) {
        if (poker->getName().compare(UP) == 0) {
            Vec2 pos = poker->getPosition();
            pos = this->convertToWorldSpace(pos);
            vcPos.push_back(pos);
        }
    }
    return vcPos;
}

std::vector<BYTE> CardBoardV88::getCards() {
    std::vector<BYTE> cards = searchCards(std::string(""));
    return cards;
}

unsigned int CardBoardV88::getCardSize() const {
    return _pokers.size();
}

bool CardBoardV88::empty() const {
    return _pokers.empty();
}

void CardBoardV88::scheduleSendCardStart(float dt) {
    do {
        if (_tmpCardValues.empty()) {
            unschedule(SEND_CARD_SCHEDULE);
            break;
        }

        SoundHepler::getInstance()->playerEffects("ThirteenZhang/sound/sendcard.mp3");

        BYTE val = _tmpCardValues.front();
        addCard(val);
        sortCard();
        resizeCardBoard();
        _tmpCardValues.pop();

    } while (0);
}

bool CardBoardV88::onTouchBegan(Touch* touch, Event *event) {
    if (!_enableCardTouch) return false;

    _startPos = Vec2::ZERO;
    _endPos = Vec2::ZERO;
    _isDrag = false;

    Vec2 pos = this->convertToNodeSpace(touch->getLocation());

    Rect rect(0,0, this->getContentSize().width, this->getContentSize().height);

    if(rect.containsPoint(pos)) {
        scheduleOnce(schedule_selector(CardBoardV88::updateDragState), 0.5f);
        _startPos = pos;
        return true;
    } else {
        return false;
    }
}

void CardBoardV88::onTouchMoved(Touch* touch, Event *event) {
    if (!_enableCardTouch) return;
    Vec2 pos = this->convertToNodeSpace(touch->getLocation());
}

void CardBoardV88::onTouchEnded(Touch* touch, Event *event) {
    if (!_enableCardTouch) return;

    Vec2 pos = this->convertToNodeSpace(touch->getLocation());
    _endPos = pos;

//        if (_isDrag)
//        {
//            touchDragCheck(_startPos, _endPos);
//        }
//        else
    {
        Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(pos)) {
            touchCheck(pos);
        }
    }
}

void CardBoardV88::touchCheck(const Vec2& pos) {
    if (_multiSelect) {
        for (size_t i = 0; i < _pokers.size(); i++) {
            int index = _pokers.size() - i - 1;
            Rect rect = _pokers[index]->getBoundingBox();
            if (rect.containsPoint(pos)) {
                if (_pokers[index]->getTag() == TAG_SELECTED) {
                    _pokers[index]->setTag(TAG_UNSELECTED);
                    _pokers[index]->setName(DOWN);
                    downCard(_pokers[index]);
                } else if (_pokers[index]->getTag() == TAG_UNSELECTED) {
                    _pokers[index]->setTag(TAG_SELECTED);
                    _pokers[index]->setName(UP);
                    upCard(_pokers[index]);
                }
                return;
            }
        }
    } else {
        int selectedIndex = 0;
        for (size_t i = 0; i < _pokers.size(); i++) {
            int index = _pokers.size() - i - 1;

            Rect rect = _pokers[index]->getBoundingBox();
            if (rect.containsPoint(pos)) {
                if (_pokers[index]->getTag() == TAG_SELECTED) {
                    _pokers[index]->setTag(TAG_UNSELECTED);
                    _pokers[index]->setName(DOWN);
                    downCard(_pokers[index]);
                } else if (_pokers[index]->getTag() == TAG_UNSELECTED) {
                    _pokers[index]->setTag(TAG_SELECTED);
                    _pokers[index]->setName(UP);
                    upCard(_pokers[index]);
                }
                selectedIndex = index;
                break;
            }
        }

        for (size_t j = 0; j < _pokers.size(); j++) {
            int index = _pokers.size() - j - 1;
            if (index == selectedIndex) continue;

            _pokers[index]->setTag(TAG_UNSELECTED);
            _pokers[index]->setName(DOWN);
            downCard(_pokers[index]);
        }
    }
}

void CardBoardV88::touchDragCheck(const Vec2& startPos, const Vec2& endPos) {
    return;
//        if (_multiSelect)
//        {
//            for (size_t i = 0; i < _pokers.size(); i++)
//            {
//                int index = _pokers.size() - i - 1;
//                Rect rect = _pokers[index]->getBoundingBox();
//                if (rect.containsPoint(pos))
//                {
//                    if (_pokers[index]->getTag() == TAG_SELECTED)
//                    {
//                        _pokers[index]->setTag(TAG_UNSELECTED);
//                        _pokers[index]->setName(DOWN);
//                        downCard(_pokers[index]);
//                    }
//                    else if (_pokers[index]->getTag() == TAG_UNSELECTED)
//                    {
//                        _pokers[index]->setTag(TAG_SELECTED);
//                        _pokers[index]->setName(UP);
//                        upCard(_pokers[index]);
//                    }
//                    return;
//                }
//            }
//        }
//        else
//        {
//            int selectedIndex = 0;
//            for (size_t i = 0; i < _pokers.size(); i++)
//            {
//                int index = _pokers.size() - i - 1;
//
//                Rect rect = _pokers[index]->getBoundingBox();
//                if (rect.containsPoint(pos))
//                {
//                    if (_pokers[index]->getTag() == TAG_SELECTED)
//                    {
//                        _pokers[index]->setTag(TAG_UNSELECTED);
//                        _pokers[index]->setName(DOWN);
//                        downCard(_pokers[index]);
//                    }
//                    else if (_pokers[index]->getTag() == TAG_UNSELECTED)
//                    {
//                        _pokers[index]->setTag(TAG_SELECTED);
//                        _pokers[index]->setName(UP);
//                        upCard(_pokers[index]);
//                    }
//                    selectedIndex = index;
//                    break;
//                }
//            }
//
//            for (size_t j = 0; j < _pokers.size(); j++)
//            {
//                int index = _pokers.size() - j - 1;
//                if (index == selectedIndex) continue;
//
//                _pokers[index]->setTag(TAG_UNSELECTED);
//                _pokers[index]->setName(DOWN);
//                downCard(_pokers[index]);
//            }
//        }
}

void CardBoardV88::enableCardTouch(bool enableTouch) {
    _enableCardTouch = enableTouch;
}

void CardBoardV88::clear() {
    unschedule(schedule_selector(CardBoardV88::scheduleSendCardStart));

    {
        std::queue<BYTE>().swap(_tmpCardValues);
    }

    for(auto poker : _pokers) {
        if (poker != nullptr) poker->removeFromParent();
    }
    _pokers.clear();
}

void CardBoardV88::addCard(BYTE cardValue) {
	CardSpriteV88* card = CardSpriteV88::create(cardValue);
    card->setScale(_self ? 1.6f : 1.0f);
    card->setName(DOWN);
    card->setTag(TAG_UNSELECTED);
    this->addChild(card);
    _pokers.push_back(card);
}

void CardBoardV88::removeCard(BYTE cardValue) {
	auto iterCard = find_if(_pokers.begin(), _pokers.end(), [cardValue](CardSpriteV88* card) {
        return (card->getValue() == cardValue);
    });
    if (iterCard != _pokers.end()) {
        CardSpriteV88* card = (*iterCard);
        card->removeFromParent();
        _pokers.erase(iterCard);
    }
}

void CardBoardV88::setMultSelected(bool multiSelect) {
    _multiSelect = multiSelect;
}

void CardBoardV88::removeCard(const std::vector<BYTE>& cards) {
    if (cards.empty()) return;

    for (BYTE card : cards) removeCard(card);

    resizeCardBoard();
}

void CardBoardV88::sortCard() {
	std::sort(_pokers.begin(), _pokers.end(), [](CardSpriteV88* left, CardSpriteV88* right) -> bool {
        BYTE lCard = left->getValue();
        BYTE rCard = right->getValue();

        BYTE lColor = lCard & 0xf0;
        BYTE rColor = rCard & 0xf0;
        BYTE lValue = lCard & 0x0f;
        BYTE rValue = rCard & 0x0f;

        //2
        if(lValue == 0x0d) {
            lValue = 0x0e;
        }

        if(rValue == 0x0d) {
            rValue = 0x0e;
        }

        return ((rValue < lValue) || (rValue == lValue && rColor < lColor));
    });
}

std::vector<BYTE> CardBoardV88::searchCards(const std::string& cond) {
    std::vector<BYTE> cards;
    if (cond.empty()) {
        for (auto poker : _pokers) {
            cards.push_back(poker->getValue());
        }
    } else {
        for (auto poker : _pokers) {
            if (poker->getName().compare(cond) == 0) {
                cards.push_back(poker->getValue());
            }
        }
    }

    return cards;
}

void CardBoardV88::swtichUpDown() {
    for (auto poker : _pokers) {
        if (poker->getTag() == TAG_UNSELECTED) continue;

        if (poker->getName().compare(UP) == 0) {
            downCard(poker);
        } else {
            upCard(poker);
        }
        poker->setTag(TAG_UNSELECTED);
    }
}

void CardBoardV88::updateDragState(float delta) {
    _isDrag = true;
}

void CardBoardV88::downCards() {
    for (auto poker : _pokers) {
        poker->setTag(TAG_UNSELECTED);
        poker->setName(DOWN);
        downCard(poker);
    }
}

void CardBoardV88::upCards(const std::vector<BYTE>& cards) {
    for(auto poker : _pokers) {
        auto iter = find_if(cards.begin(), cards.end(), [=](BYTE val) {
            return poker->getValue() == val;
        });

        if (iter != cards.end()) {
            poker->setTag(TAG_SELECTED);
            poker->setName(UP);
            upCard(poker);
        } else {
            poker->setTag(TAG_UNSELECTED);
            poker->setName(DOWN);
            downCard(poker);
        }
    }
}

void CardBoardV88::upCard(CardSpriteV88* card) {
    if (card->getName().compare(DOWN) == 0) return;

    card->stopAllActions();
    card->setName(UP);
    card->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(card->getPositionX(), UP_OFFSET)), nullptr));
}

void CardBoardV88::downCard(CardSpriteV88* card) {
    if (card->getName().compare(UP) == 0) return;

    card->stopAllActions();
    card->setName(DOWN);
    card->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(card->getPositionX(), 0)), nullptr));
}

void CardBoardV88::resizeCardBoard() {
    if(_pokers.empty()) return;

    Size cardSize = _pokers[0]->getContentSize() * _pokers[0]->getScale();

    float interval = OTHER_CARD_INTERVAL;
    if(_self) {
        //70 * 94
        float maxLength = cardSize.width + (20 - 1) * SELF_CARD_MIN_INTERVAL;
        interval = maxLength / _pokers.size();
        if(interval > SELF_CARD_MAX_INTERVAL) {
            interval = SELF_CARD_MAX_INTERVAL;
        }
    }

    float width  = cardSize.width + (_pokers.size() - 1) * interval;
    float height = cardSize.height;

    this->setContentSize(Size(width, height));

    for(size_t i = 0; i < _pokers.size(); i++) {
        _pokers[i]->setAnchorPoint(Vec2(0,0));
        _pokers[i]->setPosition(interval * i, _pokers[i]->getPositionY());
        _pokers[i]->setLocalZOrder(ZORDER_CARD_BEGAN + i);
    }

    if(!_self) {
        downCards();
    }
}
}
