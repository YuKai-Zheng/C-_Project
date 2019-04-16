#include "NNPokerCardv88.h"
#include "Sound/HNAudioEngine.h"

namespace NN {

static const char* BRNN_GAME_TABLERES_POKER			= "v66/common/card/%02x.png";
static const char* BRNN_GAME_TABLERES_POKERBACK		= "v66/common/card/00.png";


PokerCardV88::PokerCardV88(Point startPos)
    :_startPos(startPos)
    ,_Value(0)
    , _bIsMove(false) {
}

PokerCardV88::~PokerCardV88() {
}

PokerCardV88* PokerCardV88::create(Point startPos, Point curPos, BYTE val) {
    PokerCardV88* card = new PokerCardV88(startPos);
    if (card->init()) {
        card->setPosition(curPos);
        card->setCardValue(val);
        card->setCardFileName(StringUtils::format(BRNN_GAME_TABLERES_POKER, card->getCardValue()));
        card->autorelease();
        card->setScale(0.8);
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

PokerCardV88* PokerCardV88::create(Point startPos) {
    PokerCardV88* card = new PokerCardV88(startPos);
    if (card->init()) {
        card->setPosition(startPos);
        card->setScale(0.2);
        card->setCardFileName(StringUtils::format(BRNN_GAME_TABLERES_POKER, card->getCardValue()));
        card->autorelease();

        Size cardSize = card->getContentSize();
        auto clickBtn = ImageView::create(BRNN_GAME_TABLERES_POKERBACK);
        clickBtn->setPosition(Vec2(cardSize.width / 2, cardSize.height / 2));
        card->addChild(clickBtn);
        clickBtn->setOpacity(0);
        clickBtn->setName("ClickBtn");
        clickBtn->setTag(1);
        clickBtn->setTouchEnabled(false);
        clickBtn->setScale(0.7);
        clickBtn->addTouchEventListener(CC_CALLBACK_2(PokerCardV88::clickCallback, card));

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
    HNAudioEngine::getInstance()->playEffect("hallimg/niuniu/sound/send_card.wav");
    auto move = Spawn::create(MoveTo::create(0.25, pos),ScaleTo::create(0.25, 1.0), nullptr);
    this->runAction(Sequence::create(move, CallFunc::create(CC_CALLBACK_0(PokerCardV88::FlipPoke, this)), nullptr));
}

void PokerCardV88::FaAnPai(Point pos) {
    HNAudioEngine::getInstance()->playEffect("hallimg/niuniu/sound/send_card.wav");
    auto move = Spawn::create(MoveTo::create(0.25, pos),ScaleTo::create(0.25, 0.7), nullptr);
    this->runAction(move);
}

void PokerCardV88::FlipPoke() {
    HNAudioEngine::getInstance()->playEffect("hallimg/niuniu/sound/end_card.wav");
    this->runAction(Sequence::create(OrbitCamera::create(0.2, 1, 0, 0, -90, 0, 0), CallFunc::create(CC_CALLBACK_0(PokerCardV88::ChangePoke, this)),
                                     OrbitCamera::create(0.2 , 1 , 0 , 90, -90 , 0, 0), NULL) );
}

void PokerCardV88::reverseFlipPokeAndBack(float delay) {
    HNAudioEngine::getInstance()->playEffect("hallimg/niuniu/sound/send_card.wav");
    auto move = Spawn::create(MoveTo::create(0.5, _startPos),ScaleTo::create(0.5, 0), FadeOut::create(1), nullptr);
    this->runAction( Sequence::create(DelayTime::create(delay), OrbitCamera::create(0.3 , 1 , 0 , 0 , -90 , 0, 0),
                                      CallFunc::create(CC_CALLBACK_0(PokerCardV88::ChangePoke, this)),
                                      OrbitCamera::create(0.3, 1, 0, 90, -90, 0, 0), move, CallFunc::create(CC_CALLBACK_0(PokerCardV88::removeCallback, this)), RemoveSelf::create(), NULL));
}

void PokerCardV88::removeCallback() {
    if (_removeCallback != nullptr) {
        _removeCallback();
    }
}

void PokerCardV88::ChangePoke() {
    this->setCardFileName(StringUtils::format(BRNN_GAME_TABLERES_POKER, _Value));
}

void PokerCardV88::clickCallback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    if (_bIsMove) return;

    auto image = (ImageView*)pSender;
    int iTag = image->getTag();

    float fMoveY = 30*iTag;
    image->setTag(-iTag);

    _bIsMove = true;
    auto move = MoveBy::create(0.2, Vec2(0, fMoveY));
    image->getParent()->runAction(Sequence::create(move, CallFunc::create([&]() {
        _bIsMove = false;
    }), nullptr));
}

void PokerCardV88::setCardTouchEnable(bool bTouch) {
    auto btn = dynamic_cast<ImageView*>(getChildByName("ClickBtn"));
    btn->setTouchEnabled(bTouch);
}

}
