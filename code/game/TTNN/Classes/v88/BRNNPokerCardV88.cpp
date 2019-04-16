#include "BRNNPokerCardV88.h"

namespace BRNN {

PokerCardV88::PokerCardV88(Point startPos)
    : _startPos(startPos)
    ,_Value(0) {
}

PokerCardV88::~PokerCardV88() {
}

PokerCardV88* PokerCardV88::create(Point startPos, Point curPos, BYTE val) {
    PokerCardV88* card = new PokerCardV88(startPos);
    if (card->init()) {
        card->setPosition(curPos);
        card->setCardValue(val);
		card->setCardFileName(StringUtils::format(BRNN_TABLERES_POKER_V88, card->getCardValue()));
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

PokerCardV88* PokerCardV88::create(Point startPos) {
    PokerCardV88* card = new PokerCardV88(startPos);
    if (card->init()) {
        card->setPosition(startPos);
        card->setScale(0.2f);
		card->setCardFileName(StringUtils::format(BRNN_TABLERES_POKER_V88, card->getCardValue()));
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

void PokerCardV88::standUP() {
    Vec2 pos = getPosition();
    pos.y += 20;
    setPosition(pos);
}

void PokerCardV88::sitDown() {
    Vec2 pos = getPosition();
    pos.y -= 20;
    setPosition(pos);
}

void PokerCardV88::setCardFileName(const std::string& filename) {
    initWithFile(filename);
}

void PokerCardV88::setCardValue(BYTE val) {
    _Value = val;
}

BYTE PokerCardV88::getCardValue() {
    return _Value;
}

void PokerCardV88::FaMingPai(Point pos) {
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_SEND_CARD_V88);
    auto move = Spawn::create(MoveTo::create(0.3f, pos),ScaleTo::create(0.2f, 1.0f), nullptr);
    this->runAction(Sequence::create(move, CallFunc::create(CC_CALLBACK_0(PokerCardV88::FlipPoke, this)), nullptr));

}

void PokerCardV88::FaAnPai(Point pos) {
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_SEND_CARD_V88);
    auto move = Spawn::create(MoveTo::create(0.3f, pos),ScaleTo::create(0.2f, 1.0f), nullptr);
    this->runAction(move);
}

void PokerCardV88::FlipPoke() {
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_SEND_CARD_V88);
    this->runAction( Sequence::create( OrbitCamera::create(0.3f, 1.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.0f),
                                       CallFunc::create(CC_CALLBACK_0(PokerCardV88::ChangePoke, this)),
                                       OrbitCamera::create(0.3f, 1.0f, 0.0f, 90.0f, -90.0f, 0.0f, 0.0f), NULL) );
}

void PokerCardV88::reverseFlipPokeAndBack(float delay) {
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_SEND_CARD_V88);
    auto move = Spawn::create(MoveTo::create(0.5, _startPos),ScaleTo::create(0.5f, 0.0f), FadeOut::create(1), nullptr);
    this->runAction(Sequence::create(DelayTime::create(delay), OrbitCamera::create(0.3f, 1.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.0f),
                                     CallFunc::create(CC_CALLBACK_0(PokerCardV88::ChangePoke, this)),
                                     OrbitCamera::create(0.3f, 1.0f, 0.0f, 90.0f, -90.0f, 0.0f, 0.0f), move, CallFunc::create(CC_CALLBACK_0(PokerCardV88::removeCallback, this)), RemoveSelf::create(), NULL));
}

void PokerCardV88::removeCallback() {
    if (_removeCallback != nullptr) {
        _removeCallback();
    }
}

void PokerCardV88::ChangePoke() {
	this->setCardFileName(StringUtils::format(BRNN_TABLERES_POKER_V88, _Value));
}
}